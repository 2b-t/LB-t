#ifndef LBT_BOUNDARY_GUO
#define LBT_BOUNDARY_GUO

/**
 * \file     boundary_guo.hpp
 * \mainpage Very robust Guo interpolation boundary condition for pressure and velocity
*/

#include <array>
#include <memory>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "../population.hpp"
#include "boundary.hpp"
#include "boundary_orientation.hpp"
#include "boundary_type.hpp"


/**\class  Guo
 * \brief  Interpolation velocity and pressure boundary conditions as proposed by Guo.
 *         Most stable boundary conditions in this implementation so far.
 * \note   "Non-equilibrium extrapolation method for velocity and pressure boundary conditions in the
 *         lattice Boltzmann method"
 *         Z.L. Guo, C.G. Zheng, B.C. Shi
 *         Chinese Physics, Volume 11, Number 4 (2002)
 *         DOI: 10.1088/1009-1963/11/4/310
 * 
 * \tparam Type          type of the boundary condition (type::Pressure or type::Velocity)
 * \tparam Orientation   boundary orientation (orientation::Left, orientation::Right)
 * \tparam NX            Simulation domain resolution in x-direction
 * \tparam NY            Simulation domain resolution in y-direction
 * \tparam NZ            Simulation domain resolution in z-direction
 * \tparam LT            Static lattice::DdQq class containing discretisation parameters
 * \tparam T             Floating data type used for simulation
*/
template <template <class Orientation> class Type, class Orientation, unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
class Guo: public BoundaryCondition<NX,NY,NZ,LT,T,Guo<Type,Orientation,NX,NY,NZ,LT,T>>
{
    using BC = BoundaryCondition<NX,NY,NZ,LT,T,Guo<Type,Orientation,NX,NY,NZ,LT,T>>;

    public:
        /**\brief     Constructor
         * 
         * \param[in] population         Population object holding microscopic distributions
         * \param[in] boundaryElements   Elements making up the boundary
         * \param[in] p                  Index of relevant population
        */
        Guo(std::shared_ptr<Population<NX,NY,NZ,LT>> population, std::vector<BoundaryElement<T>> const& boundaryElements, 
            unsigned int const p = 0):
            BC(population, boundaryElements, p), population_(population), boundaryElements_(boundaryElements), p_(p)
        {
            return;
        }

        /**\fn     implementationBeforeCollisionOperator
         * \brief  Implementation of the boundary condition to be performed after the collision operator
         * 
         * \tparam AA   The timestep in the AA-pattern
        */
        template<timestep AA>
        void implementationBeforeCollisionOperator();

        /**\fn     implementationAfterCollisionOperator
         * \brief  Implementation of the boundary condition to be performed before the collision operator
         * 
         * \tparam AA   The timestep in the AA-pattern
        */
        template<timestep AA>
        void implementationAfterCollisionOperator();

    private:
        std::shared_ptr<Population<NX,NY,NZ,LT>> population_;
        std::vector<BoundaryElement<T>> const boundaryElements_;
        unsigned int const p_;
};

template <template <class Orientation> class Type, class Orientation, unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T> template<timestep AA>
void Guo<Type,Orientation,NX,NY,NZ,LT,T>::implementationBeforeCollisionOperator()
{
    #pragma omp parallel for default(none) shared(boundaryElements_,population_,p_) schedule(static,32)
    for(size_t i = 0; i < boundaryElements_.size(); ++i)
    {
        /// for neighbouring cell
        auto const& boundaryElement = boundaryElements_[i];
        unsigned int const x_n[3] = { (NX + boundaryElement.x + Orientation::x - 1) % NX,
                                            boundaryElement.x + Orientation::x,
                                           (boundaryElement.x + Orientation::x + 1) % NX };
        unsigned int const y_n[3] = { (NY + boundaryElement.y + Orientation::y - 1) % NY,
                                            boundaryElement.y + Orientation::y,
                                           (boundaryElement.y + Orientation::y + 1) % NY };
        unsigned int const z_n[3] = { (NZ + boundaryElement.z + Orientation::z - 1) % NZ,
                                            boundaryElement.z + Orientation::z,
                                           (boundaryElement.z + Orientation::z + 1) % NZ };

        // load distributions
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

        // macroscopic values
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

        // non-equilibrium part of distributions
        alignas(CACHE_LINE) T fneq[LT::ND] = {0.0};

        T uu = - 1.0/(2.0*LT::CS*LT::CS)*(u*u + v*v + w*w);

        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #pragma GCC unroll (16)
            for(unsigned int d = n; d < LT::HSPEED; ++d)
            {
                unsigned int const curr = n*LT::OFF + d;
                T const cu = 1.0/(LT::CS*LT::CS)*(u*LT::DX[curr] + v*LT::DY[curr] + w*LT::DZ[curr]);
                fneq[curr] = f[curr] - LT::W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
            }
        }

        /// write to current node
        // set new macroscopic values
        std::array<double,4> const bound  = {boundaryElement.rho,
                                             boundaryElement.u,
                                             boundaryElement.v,
                                             boundaryElement.w};
        std::array<double,4> const interp = {rho, u, v, w};
        std::array<double,4> res = Type<Orientation>::getMacroscopicValues(bound, interp);
        rho = res[0];
        u   = res[1];
        v   = res[2];
        w   = res[3];

        // equilibrium distributions
        alignas(CACHE_LINE) T feq[LT::ND] = {0.0};

        uu = - 1.0/(2.0*LT::CS*LT::CS)*(u*u + v*v + w*w);

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

        // write new population values to cell: feq + fneq
        unsigned int const x_c[3] = { (NX + boundaryElement.x - 1) % NX, boundaryElement.x, (boundaryElement.x + 1) % NX };
        unsigned int const y_c[3] = { (NY + boundaryElement.y - 1) % NY, boundaryElement.y, (boundaryElement.y + 1) % NY };
        unsigned int const z_c[3] = { (NZ + boundaryElement.z - 1) % NZ, boundaryElement.z, (boundaryElement.z + 1) % NZ };

        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #pragma GCC unroll (16)
            for(unsigned int d = n; d < LT::HSPEED; ++d)
            {
                unsigned int const curr = n*LT::OFF + d;
                population_->F_[population_-> template AA_IndexRead<AA>(x_c,y_c,z_c,n,d,p_)] = feq[curr] + fneq[curr];
            }
        }
    }

    return;
}

template <template <class Orientation> class Type, class Orientation, unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T> template<timestep AA>
void Guo<Type,Orientation,NX,NY,NZ,LT,T>::implementationAfterCollisionOperator()
{
    return;
}

#endif // LBT_BOUNDARY_GUO
