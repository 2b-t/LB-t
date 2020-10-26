#ifndef LBT_COLLISION_BGK_AVX512
#define LBT_COLLISION_BGK_AVX512

/**
 * \file     collision_bgk_avx512.hpp
 * \mainpage BGK collision operator with AVX512 intrinsics
 * \author   Tobit Flatscher (github.com/2b-t)
 *
 * \warning  Requires AVX512 and cache-aligned arrays
*/

#include <algorithm>
#include <cassert>
#include <cmath>
#include <type_traits>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "../../continuum/continuum.hpp"
#include "../../general/memory_alignment.hpp"
#include "../population.hpp"
#include "collision.hpp"


#ifdef __AVX512CD__

#if __has_include (<zmmintrin.h>)
    #include <zmmintrin.h>
#else
    #include <immintrin.h>
#endif

/// size of the intrinsic (AVX512: 512/8=64) and corresponding number of values of type INTR
#define AVX512_INTR_SIZE     sizeof(__m512d)
#define AVX512_REG_SIZE      sizeof(__m512d)/sizeof(double)


#ifndef __INTEL_COMPILER
/**\fn        _mm512_reduce_add_pd
 * \brief     Horizontal add function of all four numbers in a 512bit AVX2 double intrinsic
 *
 * \param[in] _a   A 512bit AVX512 intrinsic with 8 double numbers
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


/**\class  BGK_AVX512
 * \brief  BGK collision operator for arbitrary cache-aligned lattices with AVX512 intrinsics
 *
 * \note   "A Model for Collision Processes in Gases. I. Small Amplitude Processes in Charged
 *         and Neutral One-Component Systems"
 *         P.L. Bhatnagar, E.P. Gross, M. Krook
 *         Physical Review 94 (1954)
 *         DOI: 10.1103/PhysRev.94.511
 *
 * \tparam NX     Simulation domain resolution in x-direction
 * \tparam NY     Simulation domain resolution in y-direction
 * \tparam NZ     Simulation domain resolution in z-direction
 * \tparam LT     Static lattice::DdQq class containing discretisation parameters
 * \tparam T      Floating data type used for simulation
 * \tparam NPOP   Number of populations stored side by side in a single merged grid
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP>
class BGK_AVX512: public CollisionOperator<NX,NY,NZ,LT,T,NPOP,BGK_AVX512<NX,NY,NZ,LT,T,NPOP>>
{
    using CO = CollisionOperator<NX,NY,NZ,LT,T,NPOP,BGK_AVX512<NX,NY,NZ,LT,T,NPOP>>;

    public:
        BGK_AVX512() = delete;
        BGK_AVX512& operator = (BGK_AVX512&) = delete;
        BGK_AVX512(BGK_AVX512&&) = delete;
        BGK_AVX512& operator = (BGK_AVX512&&) = delete;
        BGK_AVX512(BGK_AVX512 const&) = delete;

        /**\brief     Constructor
         *
         * \param[in] population   Population object holding microscopic distributions
         * \param[in] continuum    Continuum object holding macroscopic variables
         * \param[in] Re           The Reynolds number
         * \param[in] U            The macroscopic velocity
         * \param[in] L            The characteristic length
         * \param[in] p            Index of relevant population
        */
        BGK_AVX512(std::shared_ptr<Population<NX,NY,NZ,LT,T,NPOP>> population, std::shared_ptr<Continuum<NX,NY,NZ,T>> continuum,
            T const Re, T const U, unsigned int const L, unsigned int const p = 0):
            CO(population, continuum, p), population_(population), continuum_(continuum), p_(p),
            nu_(U*static_cast<T>(L) / Re),
            tau_(nu_/(LT<T>::CS*LT<T>::CS) + 1.0/ 2.0), omega_(1.0/tau_)
        {
            static_assert(LT<T>::ND % 8 == 0);
            static_assert(std::is_same<T, double>::value == true);

            return;
        }

        /**\fn        implementation
         * \brief     Implementation of the BGK collision operator
         *
         * \tparam    TS       Even or odd timestep
         * \param[in] isSave   Boolean parameter whether the macroscopic values should be saved or not
        */
        template<timestep TS>
        void implementation(bool const isSave);

    protected:
        std::shared_ptr<Population<NX,NY,NZ,LT,T,NPOP>> population_;
        std::shared_ptr<Continuum<NX,NY,NZ,T>>          continuum_;
        unsigned int const p_;

        T const nu_;
        T const tau_;
        T const omega_;
};


template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP> template<timestep TS>
void BGK_AVX512<NX,NY,NZ,LT,T,NPOP>::implementation(bool const isSave)
{
    #pragma omp parallel for default(none) shared(continuum_,population_) firstprivate(isSave,p_) schedule(static,1)
    for(unsigned int block = 0; block < CO::NUM_BLOCKS_; ++block)
    {
        unsigned int const z_start = CO::BLOCK_SIZE_ * (block / (CO::NUM_BLOCKS_X_*CO::NUM_BLOCKS_Y_));
        unsigned int const   z_end = std::min(z_start + CO::BLOCK_SIZE_, NZ);

        for(unsigned int z = z_start; z < z_end; ++z)
        {
            unsigned int const z_n[3] = { (NZ + z - 1) % NZ, z, (z + 1) % NZ };

            unsigned int const y_start = CO::BLOCK_SIZE_*((block % (CO::NUM_BLOCKS_X_*CO::NUM_BLOCKS_Y_)) / CO::NUM_BLOCKS_X_);
            unsigned int const   y_end = std::min(y_start + CO::BLOCK_SIZE_, NY);

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                unsigned int const y_n[3] = { (NY + y - 1) % NY, y, (y + 1) % NY };

                unsigned int const x_start = CO::BLOCK_SIZE_*(block % CO::NUM_BLOCKS_X_);
                unsigned int const   x_end = std::min(x_start + CO::BLOCK_SIZE_, NX);

                for(unsigned int x = x_start; x < x_end; ++x)
                {
                    unsigned int const x_n[3] = { (NX + x - 1) % NX, x, (x + 1) % NX };

                    /// load distributions
                    alignas(CACHE_LINE) double f[LT<T>::ND] = {0.0};

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #if defined(__ICC) || defined(__ICL)
                        #pragma unroll (LT<T>::OFF)
                        #else
                        #pragma GCC unroll (16)
                        #endif
                        for(unsigned int d = 0; d < LT<T>::OFF; ++d)
                        {
                            f[n*LT<T>::OFF + d] = population_->A[population_-> template indexRead<TS>(x_n,y_n,z_n,n,d,p_)];
                        }
                    }

                    /// macroscopic values
                    __m512d _rho = _mm512_setzero_pd();
                    __m512d _u   = _mm512_setzero_pd();
                    __m512d _v   = _mm512_setzero_pd();
                    __m512d _w   = _mm512_setzero_pd();

                    for (size_t i = 0; i < LT<T>::ND; i += AVX512_REG_SIZE)
                    {
                        _rho = _mm512_add_pd(_mm512_load_pd(&f[i]), _rho);
                        _u   = _mm512_fmadd_pd(_mm512_load_pd(&LT<T>::DX[i]), _mm512_load_pd(&f[i]), _u);
                        _v   = _mm512_fmadd_pd(_mm512_load_pd(&LT<T>::DY[i]), _mm512_load_pd(&f[i]), _v);
                        _w   = _mm512_fmadd_pd(_mm512_load_pd(&LT<T>::DZ[i]), _mm512_load_pd(&f[i]), _w);
                    }

                    double const rho = _mm512_reduce_add_pd(_rho);
                    double const u   = _mm512_reduce_add_pd(_u)/rho;
                    double const v   = _mm512_reduce_add_pd(_v)/rho;
                    double const w   = _mm512_reduce_add_pd(_w)/rho;

                    if (isSave == true)
                    {
                        continuum_->operator()(x, y, z, 0) = rho;
                        continuum_->operator()(x, y, z, 1) = u;
                        continuum_->operator()(x, y, z, 2) = v;
                        continuum_->operator()(x, y, z, 3) = w;
                    }

                    /// equilibrium distributions
                    alignas(CACHE_LINE) double feq[LT<T>::ND] = {0.0};

                    __m512d const _uu = _mm512_set1_pd(-1.0/(2.0*LT<T>::CS*LT<T>::CS)*(u*u + v*v + w*w));
                    _rho = _mm512_set1_pd(rho);
                    _u   = _mm512_set1_pd(u);
                    _v   = _mm512_set1_pd(v);
                    _w   = _mm512_set1_pd(w);

                    for (size_t i = 0; i < LT<T>::ND; i += AVX512_REG_SIZE)
                    {
                        __m512d _cu = _mm512_mul_pd(_mm512_load_pd(&LT<T>::DX[i]), _u);
                        _cu = _mm512_fmadd_pd(_mm512_load_pd(&LT<T>::DY[i]), _v, _cu);
                        _cu = _mm512_fmadd_pd(_mm512_load_pd(&LT<T>::DZ[i]), _w, _cu);
                        _cu = _mm512_mul_pd(_cu, _mm512_set1_pd(1.0/(LT<T>::CS*LT<T>::CS)));

                        __m512d _res = _mm512_fmadd_pd(_mm512_set1_pd(0.5), _cu, _mm512_set1_pd(1.0));
                        _res = _mm512_fmadd_pd(_cu, _res, _uu);

                        _res = _mm512_fmadd_pd(_res, _rho, _rho);
                        _res = _mm512_mul_pd(_mm512_load_pd(&LT<T>::W[i]), _res);
                        _mm512_store_pd(&feq[i], _res);
                    }

                    /// collision
                    for (size_t i = 0; i < LT<T>::ND; i += AVX512_REG_SIZE)
                    {
                        __m512d _res = _mm512_sub_pd(_mm512_load_pd(&feq[i]), _mm512_load_pd(&f[i]));
                        _res = _mm512_fmadd_pd(_mm512_set1_pd(omega_), _res, _mm512_load_pd(&f[i]));
                        _mm512_store_pd(&f[i], _mm512_mul_pd(_mm512_load_pd(&LT<T>::MASK[i]), _res));
                    }

                    /// streaming
                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #if defined(__ICC) || defined(__ICL)
                        #pragma unroll (LT<T>::OFF)
                        #else
                        #pragma GCC unroll (16)
                        #endif
                        for(unsigned int d = 0; d < LT<T>::OFF; ++d)
                        {
                            size_t const curr = n*LT<T>::OFF + d;
                            population_->A[population_-> template indexWrite<TS>(x_n,y_n,z_n,n,d,p_)] = f[curr];
                        }
                    }
                }
            }
        }
    }

    return;
}

#endif // __AVX512__

#endif // LBT_COLLISION_BGK_AVX512
