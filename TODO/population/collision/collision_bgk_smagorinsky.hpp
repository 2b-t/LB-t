#ifndef LBT_COLLISION_BGK_SMAGORINSKY
#define LBT_COLLISION_BGK_SMAGORINSKY

/**
 * \file     collision_bgk_smagorinsky.hpp
 * \mainpage BGK collision operator with Smagorinsky turbulence model
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <algorithm>
#include <array>
#include <cmath>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "../../continuum/continuum.hpp"
#include "../../general/memory_alignment.hpp"
#include "../population.hpp"
#include "collision.hpp"


/**\class  BGK_Smagorinsky
 * \brief  BGK collision operator for arbitrary lattice
 *
 * \note   "A Lattice Boltzmann Subgrid Model for High Reynolds Number Flows"
 *         S. Hou, J. Sterling, S. Chen, G.D. Doolen
 *         (1994)
 *         arXiv: arXiv:comp-gas/9401004
 *
 * \tparam NX     Simulation domain resolution in x-direction
 * \tparam NY     Simulation domain resolution in y-direction
 * \tparam NZ     Simulation domain resolution in z-direction
 * \tparam LT     Static lattice::DdQq class containing discretisation parameters
 * \tparam T      Floating data type used for simulation
 * \tparam NPOP   Number of populations stored side by side in a single merged grid
*/
template <template <typename T> class LT, typename T, unsigned int NPOP>
class BGK_Smagorinsky: public CollisionOperator<LT,T,NPOP>
{
    using CO = CollisionOperator<LT,T,NPOP>;

    public:
        BGK_Smagorinsky() = delete;
        BGK_Smagorinsky& operator = (BGK_Smagorinsky&) = delete;
        BGK_Smagorinsky(BGK_Smagorinsky&&) = delete;
        BGK_Smagorinsky& operator = (BGK_Smagorinsky&&) = delete;
        BGK_Smagorinsky(BGK_Smagorinsky const&) = delete;

        /**\brief     Constructor
         *
         * \param[in] population   Population object holding microscopic distributions
         * \param[in] continuum    Continuum object holding macroscopic variables
         * \param[in] Re           The Reynolds number
         * \param[in] U            The macroscopic velocity
         * \param[in] L            The characteristic length
         * \param[in] p            Index of relevant population
        */
        BGK_Smagorinsky(std::shared_ptr<Population<LT,T,NPOP>> population, std::shared_ptr<Continuum<T>> continuum,
                        T const Re, T const U, unsigned int const L, unsigned int const p = 0) noexcept:
            CO(population, continuum, p), 
            nu_(U*static_cast<T>(L) / Re),
            tau_(nu_/(LT<T>::CS*LT<T>::CS) + 1.0/ 2.0), omega_(1.0/tau_)
        {
            return;
        }

        /**\fn        implementation
         * \brief     Implementation of the BGK collision operator with Smagorinsky turbulence model
         *
         * \tparam    TS       Even or odd timestep
         * \param[in] isSave   Boolean parameter whether the macroscopic values should be saved or not
        */
        template<timestep TS>
        void collideStream(bool const isSave) noexcept;

    protected:
        T const nu_;
        T const tau_;
        T const omega_;

        static constexpr T CS = 0.17; ///< Smagorinsky constant
};


template <template <typename T> class LT, typename T, unsigned int NPOP> template<timestep TS>
void BGK_Smagorinsky<LT,T,NPOP>::collideStream(bool const isSave) noexcept
{
    #pragma omp parallel for
    for(std::int32_t block = 0; block < CO::NUM_BLOCKS_; ++block)
    {
        std::int32_t const z_start = CO::BLOCK_SIZE_ * (block / (CO::NUM_BLOCKS_X_*CO::NUM_BLOCKS_Y_));
        std::int32_t const   z_end = std::min(z_start + CO::BLOCK_SIZE_, NZ);

        for(std::int32_t z = z_start; z < z_end; ++z)
        {
            std::array<std::int32_t,3> const z_n = { (NZ + z - 1) % NZ, z, (z + 1) % NZ };

            std::int32_t const y_start = CO::BLOCK_SIZE_*((block % (CO::NUM_BLOCKS_X_*CO::NUM_BLOCKS_Y_)) / CO::NUM_BLOCKS_X_);
            std::int32_t const   y_end = std::min(y_start + CO::BLOCK_SIZE_, NY);

            for(std::int32_t y = y_start; y < y_end; ++y)
            {
                std::array<std::int32_t,3> const y_n = { (NY + y - 1) % NY, y, (y + 1) % NY };

                std::int32_t const x_start = CO::BLOCK_SIZE_*(block % CO::NUM_BLOCKS_X_);
                std::int32_t const   x_end = std::min(x_start + CO::BLOCK_SIZE_, NX);

                for(std::int32_t x = x_start; x < x_end; ++x)
                {
                    std::array<std::int32_t,3> const x_n = { (NX + x - 1) % NX, x, (x + 1) % NX };

                    /// load distributions
                    alignas(CACHE_LINE) T f[LT<T>::ND] = {0.0};

                    #pragma GCC unroll (2)
                    for(std::int32_t n = 0; n <= 1; ++n)
                    {
                        #if defined(__ICC) || defined(__ICL)
                        #pragma unroll (LT<T>::OFF)
                        #else
                        #pragma GCC unroll (16)
                        #endif
                        for(std::int32_t d = 0; d < LT<T>::OFF; ++d)
                        {
                            std::int32_t const curr = n*LT<T>::OFF + d;
                            f[curr] = LT<T>::MASK[curr]*CO::population_->A[CO::population_-> template indexRead<TS>(x_n,y_n,z_n,n,d,CO::p_)];
                        }
                    }

                    /// macroscopic values
                    T rho = 0.0;
                    T u   = 0.0;
                    T v   = 0.0;
                    T w   = 0.0;
                    #pragma GCC unroll (2)
                    for(std::int32_t n = 0; n <= 1; ++n)
                    {
                        #if defined(__ICC) || defined(__ICL)
                        #pragma unroll (LT<T>::OFF)
                        #else
                        #pragma GCC unroll (16)
                        #endif
                        for(std::int32_t d = 0; d < LT<T>::OFF; ++d)
                        {
                            std::int32_t const curr = n*LT<T>::OFF + d;
                            rho += f[curr];
                            u   += f[curr]*LT<T>::DX[curr];
                            v   += f[curr]*LT<T>::DY[curr];
                            w   += f[curr]*LT<T>::DZ[curr];
                        }
                    }
                    u /= rho;
                    v /= rho;
                    w /= rho;

                    if (isSave == true)
                    {
                        CO::continuum_->operator()(x, y, z, 0) = rho;
                        CO::continuum_->operator()(x, y, z, 1) = u;
                        CO::continuum_->operator()(x, y, z, 2) = v;
                        CO::continuum_->operator()(x, y, z, 3) = w;
                    }

                    /// equilibrium distributions and non-equilibrium part
                    alignas(CACHE_LINE) T feq[LT<T>::ND]  = {0.0};
                    alignas(CACHE_LINE) T fneq[LT<T>::ND] = {0.0};

                    T const uu = - 1.0/(2.0*LT<T>::CS*LT<T>::CS)*(u*u + v*v + w*w);

                    #pragma GCC unroll (2)
                    for(std::int32_t n = 0; n <= 1; ++n)
                    {
                        #if defined(__ICC) || defined(__ICL)
                        #pragma unroll (LT<T>::OFF)
                        #else
                        #pragma GCC unroll (16)
                        #endif
                        for(std::int32_t d = 0; d < LT<T>::OFF; ++d)
                        {
                            std::int32_t const curr = n*LT<T>::OFF + d;
                            T const cu = 1.0/(LT<T>::CS*LT<T>::CS)*(u*LT<T>::DX[curr] + v*LT<T>::DY[curr] + w*LT<T>::DZ[curr]);
                            feq[curr]  = LT<T>::W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
                            fneq[curr] = f[curr] - feq[curr];
                        }
                    }

                    /// strain-rate tensor
                    T p_xx = 0.0;
                    T p_yy = 0.0;
                    T p_zz = 0.0;
                    T p_xy = 0.0;
                    T p_xz = 0.0;
                    T p_yz = 0.0;
                    #pragma GCC unroll (2)
                    for(std::int32_t n = 0; n <= 1; ++n)
                    {
                        #if defined(__ICC) || defined(__ICL)
                        #pragma unroll (LT<T>::OFF)
                        #else
                        #pragma GCC unroll (16)
                        #endif
                        for(std::int32_t d = 0; d < LT<T>::OFF; ++d)
                        {
                            std::int32_t const curr = n*LT<T>::OFF + d;
                            p_xx += LT<T>::DX[curr]*LT<T>::DX[curr]*fneq[curr];
                            p_yy += LT<T>::DY[curr]*LT<T>::DY[curr]*fneq[curr];
                            p_zz += LT<T>::DZ[curr]*LT<T>::DZ[curr]*fneq[curr];

                            p_xy += LT<T>::DX[curr]*LT<T>::DY[curr]*fneq[curr];
                            p_xz += LT<T>::DX[curr]*LT<T>::DZ[curr]*fneq[curr];
                            p_yz += LT<T>::DY[curr]*LT<T>::DZ[curr]*fneq[curr];
                        }
                    }

                    // calculate overall momentum flux
                    T const p_ij = std::sqrt(p_xx*p_xx + p_yy*p_yy + p_zz*p_zz + 2*p_xy*p_xy + 2*p_xz*p_xz + 2*p_yz*p_yz);

                    // calculate turbulent relaxation
                    T const tau_t = 0.5*(std::sqrt(tau_*tau_ + 2*sqrt(2)*CS*CS*p_ij/(rho*LT<T>::CS*LT<T>::CS*LT<T>::CS*LT<T>::CS)) - tau_);
                    T const omega = 1.0/(tau_ + tau_t);

                    /// collision and streaming
                    #pragma GCC unroll (2)
                    for(std::int32_t n = 0; n <= 1; ++n)
                    {
                        #if defined(__ICC) || defined(__ICL)
                        #pragma unroll (LT<T>::OFF)
                        #else
                        #pragma GCC unroll (16)
                        #endif
                        for(std::int32_t d = 0; d < LT<T>::OFF; ++d)
                        {
                            std::int32_t const curr = n*LT<T>::OFF + d;
                            CO::population_->A[CO::population_->template indexWrite<TS>(x_n,y_n,z_n,n,d,CO::p_)] = LT<T>::MASK[curr]*
                                                                                                                   (f[curr] + omega*(feq[curr] - f[curr]));
                        }
                    }
                }
            }
        }
    }

    return;
}

#endif // LBT_COLLISION_BGK_SMAGORINSKY
