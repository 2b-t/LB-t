#ifndef LBT_COLLISION_TRT
#define LBT_COLLISION_TRT

/**
 * \file     collision_trt.hpp
 * \mainpage TRT collision operator
*/

#include <algorithm>
#include <cmath>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "../../continuum/continuum.hpp"
#include "../../general/memory_alignment.hpp"
#include "../population.hpp"
#include "collision.hpp"


/**\class  TRT
 * \brief  TRT collision operator for arbitrary lattice
 * \note   "A Model for Collision Processes in Gases. I. Small Amplitude Processes in Charged
 *          and Neutral One-Component Systems"
 *         P.L. Bhatnagar, E.P. Gross, M. Krook
 *         Physical Review 94 (1954)
 *         DOI: 10.1103/PhysRev.94.511
 * 
 * \tparam NX     Simulation domain resolution in x-direction
 * \tparam NY     Simulation domain resolution in y-direction
 * \tparam NZ     Simulation domain resolution in z-direction
 * \tparam LT     Static lattice::DdQq class containing discretisation parameters
 * \tparam NPOP   Number of populations stored side by side in a single merged grid
 * \tparam T      Floating data type used for simulation
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP, typename T>
class TRT: public CollisionOperator<NX,NY,NZ,LT,NPOP,T,TRT<NX,NY,NZ,LT,NPOP,T>>
{
    using CO = CollisionOperator<NX,NY,NZ,LT,NPOP,T,TRT<NX,NY,NZ,LT,NPOP,T>>; 

    public:
        /**\brief     Constructor
         * 
         * \param[in] population   Population object holding microscopic distributions
         * \param[in] continuum    Continuum object holding macroscopic variables
         * \param[in] Re           The Reynolds number
         * \param[in] U            The macroscopic velocity
         * \param[in] L            The characteristic length
         * \param[in] p            Index of relevant population
         * \param[in] lambda       Magic parameter of TRT collision operator
        */
        TRT(std::shared_ptr<Population<NX,NY,NZ,LT,NPOP>> population, std::shared_ptr<Continuum<NX,NY,NZ,T>> continuum, 
            T const Re, T const U, unsigned int const L, unsigned int const p = 0, T const lambda = 0.25):
            CO(population, continuum, p), population_(population), continuum_(continuum), p_(p),
            nu_(U*static_cast<T>(L) / Re), 
            lambda_(lambda),
            tau_p_(nu_/(LT::CS*LT::CS) + 1.0/ 2.0), omega_p_(1.0/tau_p_),
            omega_m_((tau_p_ - 1.0/2.0) / (lambda_ + 1.0/2.0*( tau_p_ - 1.0/2.0)))
        {
            return;
        }

        /**\fn        implementation
         * \brief     Implementation of the TRT collision operator
         * 
         * \tparam    AA       The timestep in the AA-pattern
         * \param[in] isSave   Boolean parameter whether the macroscopic values should be saved or not
        */
        template<timestep AA>
        void implementation(bool const isSave);

    protected:
        std::shared_ptr<Population<NX,NY,NZ,LT,NPOP>> population_;
        std::shared_ptr<Continuum<NX,NY,NZ,T>>        continuum_;
        unsigned int const p_;

        T const nu_;
        T const lambda_;
        T const tau_p_;
        T const omega_p_;
        T const omega_m_;
};


template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP, typename T> template<timestep AA>
void TRT<NX,NY,NZ,LT,NPOP,T>::implementation(bool const isSave)
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
                    alignas(CACHE_LINE) T f[LT::ND] = {0.0};

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = n; d < LT::HSPEED; ++d)
                        {
                            f[n*LT::OFF + d] = population_->F_[population_-> template AA_IndexRead<AA>(x_n,y_n,z_n,n,d,p_)];
                        }
                    }

                    /// macroscopic values
                    T rho = 0.0;
                    T u   = 0.0;
                    T v   = 0.0;
                    T w   = 0.0;
                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = n; d < LT::HSPEED; ++d)
                        {
                            unsigned int const curr = n*LT::OFF + d;
                            rho += f[curr];
                            u   += f[curr]*LT::DX[curr];
                            v   += f[curr]*LT::DY[curr];
                            w   += f[curr]*LT::DZ[curr];
                        }
                    }
                    u /= rho;
                    v /= rho;
                    w /= rho;

                    if (isSave == true)
                    {
                        continuum_->operator()(x, y, z, 0) = rho;
                        continuum_->operator()(x, y, z, 1) = u;
                        continuum_->operator()(x, y, z, 2) = v;
                        continuum_->operator()(x, y, z, 3) = w;
                    }

                    /// equilibrium distributions
                    alignas(CACHE_LINE) T feq[LT::ND] = {0.0};

                    T const uu = - 1.0/(2.0*LT::CS*LT::CS)*(u*u + v*v + w*w);

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = n; d < LT::HSPEED; ++d)
                        {
                            unsigned int const curr = n*LT::OFF + d;
                            T const cu = 1.0/(LT::CS*LT::CS)*(u*LT::DX[curr] + v*LT::DY[curr] + w*LT::DZ[curr]);
                            feq[curr] = LT::W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
                        }
                    }

                    /// odd and even part
                    alignas(CACHE_LINE) T fp[LT::OFF] = {0.0};
                    alignas(CACHE_LINE) T fm[LT::OFF] = {0.0};

                    #pragma GCC unroll (15)
                    for(unsigned int d = 1; d < LT::HSPEED; ++d)
                    {
                        fp[d] = 0.5*(f[d] + f[LT::OFF + d] - (feq[d] + feq[LT::OFF + d]));
                        fm[d] = 0.5*(f[d] - f[LT::OFF + d] - (feq[d] - feq[LT::OFF + d]));
                    }

                    /// collision and streaming
                    population_->F_[population_-> template AA_IndexWrite<AA>(x_n,y_n,z_n,0,0,p_)] = f[0] + omega_p_*(feq[0] - f[0]);
                    #pragma GCC unroll (15)
                    for(unsigned int d = 1; d < LT::HSPEED; ++d)
                    {
                        population_->F_[population_-> template AA_IndexWrite<AA>(x_n,y_n,z_n,0,d,p_)] = f[d] - omega_m_*fp[d] - omega_m_*fm[d];
                    }
                    #pragma GCC unroll (15)
                    for(unsigned int d = 1; d < LT::HSPEED; ++d)
                    {
                        population_->F_[population_-> template AA_IndexWrite<AA>(x_n,y_n,z_n,1,d,p_)] = f[LT::OFF + d] - omega_p_*fp[d] + omega_m_*fm[d];
                    }
                }
            }
        }
    }

    return;
}

#endif // LBT_COLLISION_TRT
