#ifndef COLLISION_BGK_AVX2_HPP_INCLUDED
#define COLLISION_BGK_AVX2_HPP_INCLUDED

/**
 * \file     collision_bgk_avx2.hpp
 * \mainpage BGK collision operator with AVX2 intrinsics
 * \warning  Requires AVX2 and cache-aligned arrays
*/

#include <algorithm>
#include <cmath>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "../../general/memory_alignment.hpp"
#include "../../continuum/continuum.hpp"
#include "../population.hpp"


#ifdef __AVX2__

#include <immintrin.h>

/**\fn        _mm256_reduce_add_pd
 * \brief     Horizontal add function of all four numbers in a 256bit AVX2 double intrinsic
 *
 * \param[in] _a: a 256bit AVX2 intrinsic with 4 double numbers
 * \return    The horizontal added intrinsic as a double number
*/
static inline double __attribute__((always_inline)) _mm256_reduce_add_pd(__m256d _a)
{
    __m256d _sum = _mm256_hadd_pd(_a, _a);
    return ((double*)&_sum)[0] + ((double*)&_sum)[2];
}

/**\fn            CollideStreamBGK_AVX2
 * \brief         BGK collision operator for arbitrary cache-aligned lattices with AVX2 intrinsics
 *
 * \param[out]    macro   continuum object holding macroscopic variables
 * \param[in,out] micro   population object holding microscopic variables
 * \param[in]     save    save current macroscopic values to disk (Boolean true/false)
 * \param[in]     p       relevant population (default 0)
*/
template <bool odd, unsigned int NX, unsigned int NY, unsigned int NZ, class LT, class POP, typename T>
void CollideStreamBGK_AVX2(Continuum<NX,NY,NZ,T>& macro, POP& micro, bool const save = false, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(macro, micro) firstprivate(save, p) schedule(static,1)
    for(unsigned int block = 0; block < micro.NUM_BLOCKS_; ++block)
    {
        unsigned int x_n[3] = {0, 0, 0};
        unsigned int y_n[3] = {0, 0, 0};
        unsigned int z_n[3] = {0, 0, 0};

        unsigned int const z_start = micro.BLOCK_SIZE_ * (block / (micro.NUM_BLOCKS_X_*micro.NUM_BLOCKS_Y_));
        unsigned int const   z_end = std::min(z_start + micro.BLOCK_SIZE_, NZ);

        for(unsigned int z = z_start; z < z_end; ++z)
        {
            z_n[0] = (NZ + z - 1) % NZ;
            z_n[1] =       z;
            z_n[2] = (     z + 1) % NZ;

            unsigned int const y_start = micro.BLOCK_SIZE_*((block % (micro.NUM_BLOCKS_X_*micro.NUM_BLOCKS_Y_)) / micro.NUM_BLOCKS_X_);
            unsigned int const   y_end = std::min(y_start + micro.BLOCK_SIZE_, NY);

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                y_n[0] = (NY + y - 1) % NY;
                y_n[1] =       y;
                y_n[2] = (     y + 1) % NY;

                unsigned int const x_start = micro.BLOCK_SIZE_*(block % micro.NUM_BLOCKS_X_);
                unsigned int const   x_end = std::min(x_start + micro.BLOCK_SIZE_, NX);

                for(unsigned int x = x_start; x < x_end; ++x)
                {
                    x_n[0] = (NX + x - 1) % NX;
                    x_n[1] =       x;
                    x_n[2] = (     x + 1) % NX;

                    /// load distributions
                    alignas(CACHE_LINE) double f[LT::ND] = {0.0};

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = 0; d < LT::HSPEED; ++d)
                        {
                            f[n*LT::OFF + d] = micro. template AA_Read<odd>(x_n, y_n, z_n, n, d, p);
                        }
                    }

                    /// determine macroscopic values by means of intrinsics
                    __m256d _rho = _mm256_setzero_pd();
                    __m256d _u   = _mm256_setzero_pd();
                    __m256d _v   = _mm256_setzero_pd();
                    __m256d _w   = _mm256_setzero_pd();

                    for (size_t i = 0; i < LT::ND; i += AVX_REG_SIZE)
                    {
                        _rho = _mm256_add_pd(_mm256_load_pd(&f[i]), _rho);
                        _u   = _mm256_fmadd_pd(_mm256_load_pd(&LT::DX[i]), _mm256_load_pd(&f[i]), _u);
                        _v   = _mm256_fmadd_pd(_mm256_load_pd(&LT::DY[i]), _mm256_load_pd(&f[i]), _v);
                        _w   = _mm256_fmadd_pd(_mm256_load_pd(&LT::DZ[i]), _mm256_load_pd(&f[i]), _w);
                    }

                    double const rho = _mm256_reduce_add_pd(_rho);
                    double const u   = _mm256_reduce_add_pd(_u)/rho;
                    double const v   = _mm256_reduce_add_pd(_v)/rho;
                    double const w   = _mm256_reduce_add_pd(_w)/rho;

                    if (save == true)
                    {
                        macro(x, y, z, 0) = rho;
                        macro(x, y, z, 1) = u;
                        macro(x, y, z, 2) = v;
                        macro(x, y, z, 3) = w;
                    }

                    /// calculate equilibrium distribution
                    alignas(CACHE_LINE) double feq[LT::ND] = {0.0};

                    __m256d const _uu = _mm256_set1_pd(-1.0/(2.0*LT::CS*LT::CS)*(u*u + v*v + w*w));
                    _rho = _mm256_set1_pd(rho);
                    _u   = _mm256_set1_pd(u);
                    _v   = _mm256_set1_pd(v);
                    _w   = _mm256_set1_pd(w);

                    for (size_t i = 0; i < LT::ND; i += AVX_REG_SIZE)
                    {
                        __m256d _cu = _mm256_mul_pd(_mm256_load_pd(&LT::DX[i]), _u);
                        _cu = _mm256_fmadd_pd(_mm256_load_pd(&LT::DY[i]), _v, _cu);
                        _cu = _mm256_fmadd_pd(_mm256_load_pd(&LT::DZ[i]), _w, _cu);
                        _cu = _mm256_mul_pd(_cu, _mm256_set1_pd(1.0/(LT::CS*LT::CS)));

                        __m256d _res = _mm256_fmadd_pd(_mm256_set1_pd(0.5), _cu, _mm256_set1_pd(1.0));
                        _res = _mm256_fmadd_pd(_cu, _res, _uu);

                        _res = _mm256_fmadd_pd(_res, _rho, _rho);
                        _res = _mm256_mul_pd(_mm256_load_pd(&LT::W[i]), _res);
                        _mm256_store_pd(&feq[i], _res);
                    }

                    /// collision
                    for (size_t i = 0; i < LT::ND; i += AVX_REG_SIZE)
                    {
                        __m256d _diff = _mm256_sub_pd(_mm256_load_pd(&feq[i]), _mm256_load_pd(&f[i]));
                        _mm256_store_pd(&f[i], _mm256_fmadd_pd(_mm256_set1_pd(micro.OMEGA_), _diff, _mm256_load_pd(&f[i])));
                    }

                    /// collision and streaming
                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = 0; d < LT::HSPEED; ++d)
                        {
                            size_t const curr = n*LT::OFF + d;
                            micro. template AA_Write<odd>(x_n,y_n,z_n,n,d,p) = f[curr];
                        }
                    }
                }
            }
        }
    }
}

#endif // __AVX2__

#endif // COLLISION_BGK_AVX2_HPP_INCLUDED
