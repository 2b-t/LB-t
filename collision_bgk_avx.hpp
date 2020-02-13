#ifndef COLLISION_BGK_AVX_HPP_INCLUDED
#define COLLISION_BGK_AVX_HPP_INCLUDED


#if __has_include (<omp.h>)
    #include <omp.h>
#endif
#include <algorithm>
#include <cmath>

#include "memory_alignment.hpp"

#include "continuum.hpp"
#include "lattice.hpp"


#ifdef __AVX2__

/// header for AVX2 intrinsics
#include <immintrin.h>

/// macros: switch between AVX2 and AVX512

/**\fn        _mm256_reduce_add_pd
 * \brief     Horizontal add function of all four numbers in a 256bit AVX2 double intrinsic
 *
 * \param[in] _a: a 256bit AVX2 intrinsic with 4 double numbers
 * \return    The horizontal added intrinsic as a double number
*/
static inline double _mm256_reduce_add_pd(__m256d _a)
{
    __m256d _sum = _mm256_hadd_pd(_a, _a);
    return ((double*)&_sum)[0] + ((double*)&_sum)[2];
}

template <bool odd>
void collision_bgk_avx(Continuum<double>& macro, Lattice<double>& lattice, bool const save = false, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(macro, lattice) firstprivate(save, p) schedule(static,1)
    for(unsigned int block = 0; block < lattice.NUM_BLOCKS; ++block)
    {
        unsigned int x_n[3] = {0, 0, 0};
        unsigned int y_n[3] = {0, 0, 0};
        unsigned int z_n[3] = {0, 0, 0};

        unsigned int const z_start = lattice.BLOCK_SIZE * (block / (lattice.NUM_BLOCKS_X*lattice.NUM_BLOCKS_Y));
        unsigned int const   z_end = std::min(z_start + lattice.BLOCK_SIZE, lattice.NZ);

        for(unsigned int z = z_start; z < z_end; ++z)
        {
            unsigned int const y_start = lattice.BLOCK_SIZE*((block % (lattice.NUM_BLOCKS_X*lattice.NUM_BLOCKS_Y)) / lattice.NUM_BLOCKS_X);
            unsigned int const   y_end = std::min(y_start + lattice.BLOCK_SIZE, lattice.NY);

            z_n[0] = (lattice.NZ + z - 1) % lattice.NZ;
            z_n[1] =               z;
            z_n[2] = (             z + 1) % lattice.NZ;

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                unsigned int const x_start = lattice.BLOCK_SIZE*(block % lattice.NUM_BLOCKS_X);
                unsigned int const   x_end = std::min(x_start + lattice.BLOCK_SIZE, lattice.NX);

                y_n[0] = (lattice.NY + y - 1) % lattice.NY;
                y_n[1] =               y;
                y_n[2] = (             y + 1) % lattice.NY;

                for(unsigned int x = x_start; x < x_end; ++x)
                {
                    x_n[0] = (lattice.NX + x - 1) % lattice.NX;
                    x_n[1] =               x;
                    x_n[2] = (             x + 1) % lattice.NX;

                    /// load
                    alignas(CACHE_LINE) double f[lattice.ND];
                    lattice.Load<odd>(x_n, y_n, z_n, f);

                    /// determine macroscopic values by means of intrinsics
                    __m256d _rho = _mm256_setzero_pd();
                    __m256d _u   = _mm256_setzero_pd();
                    __m256d _v   = _mm256_setzero_pd();
                    __m256d _w   = _mm256_setzero_pd();

                    for (size_t i = 0; i < lattice.ND; i += AVX_REG_SIZE)
                    {
                        _rho = _mm256_add_pd(_mm256_load_pd(&f[i]), _rho);
                        _u   = _mm256_fmadd_pd(_mm256_load_pd(&lattice.DX[i]), _mm256_load_pd(&f[i]), _u);
                        _v   = _mm256_fmadd_pd(_mm256_load_pd(&lattice.DY[i]), _mm256_load_pd(&f[i]), _v);
                        _w   = _mm256_fmadd_pd(_mm256_load_pd(&lattice.DZ[i]), _mm256_load_pd(&f[i]), _w);
                    }

                    double const rho = _mm256_reduce_add_pd(_rho);
                    double const u   = _mm256_reduce_add_pd(_u);
                    double const v   = _mm256_reduce_add_pd(_v);
                    double const w   = _mm256_reduce_add_pd(_w);

                    if (save == true)
                    {
                        macro(x, y, z, 0) = rho;
                        macro(x, y, z, 1) = u;
                        macro(x, y, z, 2) = v;
                        macro(x, y, z, 3) = w;
                    }

                    /// calculate equilibrium distribution
                    alignas(CACHE_LINE) double feq[lattice.ND] = {0.0};

                    __m256d const _uu = _mm256_set1_pd(-1.0/(2.0*lattice.CS*lattice.CS)*(u*u + v*v + w*w));
                    _rho = _mm256_set1_pd(rho);
                    _u   = _mm256_set1_pd(u);
                    _v   = _mm256_set1_pd(v);
                    _w   = _mm256_set1_pd(w);

                    for (size_t i = 0; i < lattice.ND; i += AVX_REG_SIZE)
                    {
                        __m256d _cu = _mm256_mul_pd(_mm256_load_pd(&lattice.DX[i]), _u);
                        _cu = _mm256_fmadd_pd(_mm256_load_pd(&lattice.DY[i]), _v, _cu);
                        _cu = _mm256_fmadd_pd(_mm256_load_pd(&lattice.DZ[i]), _w, _cu);
                        _cu = _mm256_mul_pd(_cu, _mm256_set1_pd(1.0/(lattice.CS*lattice.CS)));

                        __m256d _res = _mm256_fmadd_pd(_mm256_set1_pd(0.5), _cu, _mm256_set1_pd(1.0));
                        _res = _mm256_fmadd_pd(_cu, _res, _uu);

                        _res = _mm256_fmadd_pd(_res, _rho, _rho);
                        _res = _mm256_mul_pd(_mm256_load_pd(&lattice.W[i]), _res);
                        _mm256_store_pd(&feq[i], _res);
                    }

                    /// collision
                    for (size_t i = 0; i < lattice.ND; i += AVX_REG_SIZE)
                    {
                        __m256d _diff = _mm256_sub_pd(_mm256_load_pd(&feq[i]), _mm256_load_pd(&f[i]));
                        _mm256_store_pd(&f[i], _mm256_fmadd_pd(_mm256_set1_pd(lattice.OMEGA), _diff, _mm256_load_pd(&f[i])));
                    }

                    lattice.Store<odd>(x_n, y_n, z_n, f, p);
                }
            }
        }
    }
}

#endif

#endif //COLLISION_BGK_AVX_HPP_INCLUDED
