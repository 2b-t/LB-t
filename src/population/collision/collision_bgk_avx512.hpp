#ifndef COLLISION_BGK_AVX512_HPP_INCLUDED
#define COLLISION_BGK_AVX512_HPP_INCLUDED

/**
 * \file     collision_bgk_avx512.hpp
 * \mainpage BGK collision operator with AVX512 intrinsics
 * \warning  Requires AVX512 and cache-aligned arrays
*/

#include <algorithm>
#include <cmath>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "../../general/memory_alignment.hpp"
#include "../../continuum/continuum.hpp"
#include "../population.hpp"


#ifdef __AVX512CD__

#if __has_include (<zmmintrin.h>)
    #include <zmmintrin.h>
#else
    #include <immintrin.h>
#endif

// size of the intrinsic (AVX512: 512/8=64) and corresponding number of values of type INTR
#define AVX512_INTR_SIZE     sizeof(__m512d)
#define AVX512_REG_SIZE      sizeof(__m512d)/sizeof(double)


#ifndef __INTEL_COMPILER
/**\fn        _mm512_reduce_add_pd
 * \brief     Horizontal add function of all four numbers in a 512bit AVX2 double intrinsic
 *
 * \param[in] _a   a 512bit AVX512 intrinsic with 8 double numbers
 * \return    The horizontally added intrinsic as a double number
*/
static inline double _mm512_reduce_add_pd(__m512d const _a)
{
    __m256d const _b = _mm256_add_pd(_mm512_castpd512_pd256(_a), _mm512_extractf64x4_pd(_a, 1));
    __m128d const _c = _mm_add_pd(_mm256_castpd256_pd128(_b), _mm256_extractf128_pd(_b, 1));
    double const *f = (double*)(&_c);
    return _mm_cvtsd_f64(_c) + f[1];
}
#endif

/**\fn            CollideStreamBGK_AVX512
 * \brief         BGK collision operator for arbitrary cache-aligned lattices with AVX512 intrinsics
 * \note          "A Model for Collision Processes in Gases. I. Small Amplitude Processes in Charged
 *                and Neutral One-Component Systems"
 *                P.L. Bhatnagar, E.P. Gross, M. Krook
 *                Physical Review 94 (1954)
 *                DOI: 10.1103/PhysRev.94.511
 *
 * \tparam        odd   even (0, false) or odd (1, true) time step
 * \tparam        NX    simulation domain resolution in x-direction
 * \tparam        NY    simulation domain resolution in y-direction
 * \tparam        NZ    simulation domain resolution in z-direction
 * \tparam        LT    static lattice::DdQq class containing discretisation parameters
 * \tparam        T     floating data type used for simulation
 * \param[out]    con    continuum object holding macroscopic variables
 * \param[in,out] pop    population object holding microscopic variables
 * \param[in]     save   save current macroscopic values to disk (Boolean true/false)
 * \param[in]     p      relevant population (default = 0)
*/
template <bool odd, unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
void CollideStreamBGK_AVX512(Continuum<NX,NY,NZ,T>& con, Population<NX,NY,NZ,LT>& pop, bool const save = false, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(con, pop) firstprivate(save,p) schedule(static,1)
    for(unsigned int block = 0; block < pop.NUM_BLOCKS_; ++block)
    {
        unsigned int const z_start = pop.BLOCK_SIZE_ * (block / (pop.NUM_BLOCKS_X_*pop.NUM_BLOCKS_Y_));
        unsigned int const   z_end = std::min(z_start + pop.BLOCK_SIZE_, NZ);

        for(unsigned int z = z_start; z < z_end; ++z)
        {
            unsigned int const z_n[3] = { (NZ + z - 1) % NZ, z, (z + 1) % NZ };

            unsigned int const y_start = pop.BLOCK_SIZE_*((block % (pop.NUM_BLOCKS_X_*pop.NUM_BLOCKS_Y_)) / pop.NUM_BLOCKS_X_);
            unsigned int const   y_end = std::min(y_start + pop.BLOCK_SIZE_, NY);

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                unsigned int const y_n[3] = { (NY + y - 1) % NY, y, (y + 1) % NY };

                unsigned int const x_start = pop.BLOCK_SIZE_*(block % pop.NUM_BLOCKS_X_);
                unsigned int const   x_end = std::min(x_start + pop.BLOCK_SIZE_, NX);

                for(unsigned int x = x_start; x < x_end; ++x)
                {
                    unsigned int const x_n[3] = { (NX + x - 1) % NX, x, (x + 1) % NX };

                    /// load distributions
                    alignas(CACHE_LINE) double f[LT::ND] = {0.0};

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = 0; d < LT::OFF; ++d)
                        {
                            f[n*LT::OFF + d] = pop.F_[pop. template AA_IndexRead<odd>(x_n,y_n,z_n,n,d,p)];
                        }
                    }

                    /// macroscopic values
                    __m512d _rho = _mm512_setzero_pd();
                    __m512d _u   = _mm512_setzero_pd();
                    __m512d _v   = _mm512_setzero_pd();
                    __m512d _w   = _mm512_setzero_pd();

                    for (size_t i = 0; i < LT::ND; i += AVX512_REG_SIZE)
                    {
                        _rho = _mm512_add_pd(_mm512_load_pd(&f[i]), _rho);
                        _u   = _mm512_fmadd_pd(_mm512_load_pd(&LT::DX[i]), _mm512_load_pd(&f[i]), _u);
                        _v   = _mm512_fmadd_pd(_mm512_load_pd(&LT::DY[i]), _mm512_load_pd(&f[i]), _v);
                        _w   = _mm512_fmadd_pd(_mm512_load_pd(&LT::DZ[i]), _mm512_load_pd(&f[i]), _w);
                    }

                    double const rho = _mm512_reduce_add_pd(_rho);
                    double const u   = _mm512_reduce_add_pd(_u)/rho;
                    double const v   = _mm512_reduce_add_pd(_v)/rho;
                    double const w   = _mm512_reduce_add_pd(_w)/rho;

                    if (save == true)
                    {
                        con(x, y, z, 0) = rho;
                        con(x, y, z, 1) = u;
                        con(x, y, z, 2) = v;
                        con(x, y, z, 3) = w;
                    }

                    /// equilibrium distributions
                    alignas(CACHE_LINE) double feq[LT::ND] = {0.0};

                    __m512d const _uu = _mm512_set1_pd(-1.0/(2.0*LT::CS*LT::CS)*(u*u + v*v + w*w));
                    _rho = _mm512_set1_pd(rho);
                    _u   = _mm512_set1_pd(u);
                    _v   = _mm512_set1_pd(v);
                    _w   = _mm512_set1_pd(w);

                    for (size_t i = 0; i < LT::ND; i += AVX512_REG_SIZE)
                    {
                        __m512d _cu = _mm512_mul_pd(_mm512_load_pd(&LT::DX[i]), _u);
                        _cu = _mm512_fmadd_pd(_mm512_load_pd(&LT::DY[i]), _v, _cu);
                        _cu = _mm512_fmadd_pd(_mm512_load_pd(&LT::DZ[i]), _w, _cu);
                        _cu = _mm512_mul_pd(_cu, _mm512_set1_pd(1.0/(LT::CS*LT::CS)));

                        __m512d _res = _mm512_fmadd_pd(_mm512_set1_pd(0.5), _cu, _mm512_set1_pd(1.0));
                        _res = _mm512_fmadd_pd(_cu, _res, _uu);

                        _res = _mm512_fmadd_pd(_res, _rho, _rho);
                        _res = _mm512_mul_pd(_mm512_load_pd(&LT::W[i]), _res);
                        _mm512_store_pd(&feq[i], _res);
                    }

                    /// collision
                    for (size_t i = 0; i < LT::ND; i += AVX512_REG_SIZE)
                    {
                        __m512d _res = _mm512_sub_pd(_mm512_load_pd(&feq[i]), _mm512_load_pd(&f[i]));
                        _res = _mm512_fmadd_pd(_mm512_set1_pd(pop.OMEGA_), _res, _mm512_load_pd(&f[i]));
                        _mm512_store_pd(&f[i], _mm512_mul_pd(_mm512_load_pd(&LT::MASK[i]), _res));
                    }

                    /// streaming
                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = 0; d < LT::OFF; ++d)
                        {
                            size_t const curr = n*LT::OFF + d;
                            pop.F_[pop. template AA_IndexWrite<odd>(x_n,y_n,z_n,n,d,p)] = f[curr];
                        }
                    }
                }
            }
        }
    }
}

#endif // __AVX512__

#endif // COLLISION_BGK_AVX512_HPP_INCLUDED
