#ifndef LBT_BOUNDARY_GUO
#define LBT_BOUNDARY_GUO

/**
 * \file     boundary_guo.hpp
 * \mainpage Very robust Guo interpolation boundary condition for pressure and velocity
 * \author   Tobit Flatscher (github.com/2b-t)
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
 *
 * \note   "Non-equilibrium extrapolation method for velocity and pressure boundary conditions in the
 *         lattice Boltzmann method"
 *         Z.L. Guo, C.G. Zheng, B.C. Shi
 *         Chinese Physics, Volume 11, Number 4 (2002)
 *         DOI: 10.1088/1009-1963/11/4/310
 *
 * \tparam TP     Type of the boundary condition (type::Pressure or type::Velocity)
 * \tparam O      Boundary orientation (orientation::Left, orientation::Right)
 * \tparam NX     Simulation domain resolution in x-direction
 * \tparam NY     Simulation domain resolution in y-direction
 * \tparam NZ     Simulation domain resolution in z-direction
 * \tparam LT     Static lattice::DdQq class containing discretisation parameters
 * \tparam NPOP   Number of populations stored side by side in a single merged grid
 * \tparam T      Floating data type used for simulation
*/
template <boundary::Type TP, boundary::Orientation O, unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT,
          typename T, unsigned int NPOP>
class Guo: public BoundaryCondition<NX,NY,NZ,LT,T,NPOP,Guo<TP,O,NX,NY,NZ,LT,T,NPOP>>
{
    using BC = BoundaryCondition<NX,NY,NZ,LT,T,NPOP,Guo<TP,O,NX,NY,NZ,LT,T,NPOP>>;

    public:
        Guo() = delete;

        /**\brief     Constructor
         *
         * \param[in] population         Population object holding microscopic distributions
         * \param[in] boundaryElements   Elements making up the boundary
         * \param[in] p                  Index of relevant population
        */
        Guo(std::shared_ptr<Population<NX,NY,NZ,LT,T,NPOP>> population, std::vector<boundary::Element<T>> const& boundaryElements,
            unsigned int const p = 0):
            BC(population, boundaryElements, p)
        {
            return;
        }

        /**\fn     implementationBeforeCollisionOperator
         * \brief  Implementation of the boundary condition to be performed after the collision operator
         *
         * \tparam TS   Even or odd timestep
        */
        template <timestep TS>
        void implementationBeforeCollisionOperator();

        /**\fn     implementationAfterCollisionOperator
         * \brief  Implementation of the boundary condition to be performed before the collision operator
         *
         * \tparam TS   Even or odd timestep
        */
        template <timestep TS>
        void implementationAfterCollisionOperator();
};

template <boundary::Type TP, boundary::Orientation O, unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT,
          typename T, unsigned int NPOP> template <timestep TS>
void Guo<TP,O,NX,NY,NZ,LT,T,NPOP>::implementationBeforeCollisionOperator()
{
    #pragma omp parallel for default(none) shared(BC::boundaryElements_,BC::population_,BC::p_) schedule(static,32)
    for(size_t i = 0; i < BC::boundaryElements_.size(); ++i)
    {
        /// for neighbouring cell
        auto const& boundaryElement = BC::boundaryElements_[i];
        std::array<unsigned int,3> const x_n = { (NX + boundaryElement.x + boundary::Normal<O>::x - 1) % NX,
                                                       boundaryElement.x + boundary::Normal<O>::x,
                                                      (boundaryElement.x + boundary::Normal<O>::x + 1) % NX };
        std::array<unsigned int,3> const y_n = { (NY + boundaryElement.y + boundary::Normal<O>::y - 1) % NY,
                                                       boundaryElement.y + boundary::Normal<O>::y,
                                                      (boundaryElement.y + boundary::Normal<O>::y + 1) % NY };
        std::array<unsigned int,3> const z_n = { (NZ + boundaryElement.z + boundary::Normal<O>::z - 1) % NZ,
                                                       boundaryElement.z + boundary::Normal<O>::z,
                                                      (boundaryElement.z + boundary::Normal<O>::z + 1) % NZ };

        // load distributions
        alignas(CACHE_LINE) T f[LT<T>::ND] = {0.0};

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
                unsigned int const curr = n*LT<T>::OFF + d;
                f[curr] = LT<T>::MASK[curr]*BC::population_->A[BC::population_->template indexRead<TS>(x_n,y_n,z_n,n,d,BC::p_)];
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
            #if defined(__ICC) || defined(__ICL)
            #pragma unroll (LT<T>::OFF)
            #else
            #pragma GCC unroll (16)
            #endif
            for(unsigned int d = 0; d < LT<T>::OFF; ++d)
            {
                unsigned int const curr = n*LT<T>::OFF + d;
                rho += f[curr];
                u   += f[curr]*LT<T>::DX[curr];
                v   += f[curr]*LT<T>::DY[curr];
                w   += f[curr]*LT<T>::DZ[curr];
            }
        }
        u /= rho;
        v /= rho;
        w /= rho;

        // non-equilibrium part of distributions
        alignas(CACHE_LINE) T fneq[LT<T>::ND] = {0.0};

        T uu = - 1.0/(2.0*LT<T>::CS*LT<T>::CS)*(u*u + v*v + w*w);

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
                unsigned int const curr = n*LT<T>::OFF + d;
                T const cu = 1.0/(LT<T>::CS*LT<T>::CS)*(u*LT<T>::DX[curr] + v*LT<T>::DY[curr] + w*LT<T>::DZ[curr]);
                fneq[curr] = f[curr] - LT<T>::W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
            }
        }

        /// write to current node
        // set new macroscopic values
        std::array<double,4> const bound  = {boundaryElement.rho,
                                             boundaryElement.u,
                                             boundaryElement.v,
                                             boundaryElement.w};
        std::array<double,4> const interp = {rho, u, v, w};
        std::array<double,4> res = boundary::MacroscopicValues<T,O,TP>::get(bound, interp);
        rho = res[0];
        u   = res[1];
        v   = res[2];
        w   = res[3];

        // equilibrium distributions
        alignas(CACHE_LINE) T feq[LT<T>::ND] = {0.0};

        uu = - 1.0/(2.0*LT<T>::CS*LT<T>::CS)*(u*u + v*v + w*w);

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
                unsigned int const curr = n*LT<T>::OFF + d;
                T const cu = 1.0/(LT<T>::CS*LT<T>::CS)*(u*LT<T>::DX[curr] + v*LT<T>::DY[curr] + w*LT<T>::DZ[curr]);
                feq[curr] = LT<T>::W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
            }
        }

        // write new population values to cell: feq + fneq
        std::array<unsigned int,3> const x_c = { (NX + boundaryElement.x - 1) % NX, boundaryElement.x, (boundaryElement.x + 1) % NX };
        std::array<unsigned int,3> const y_c = { (NY + boundaryElement.y - 1) % NY, boundaryElement.y, (boundaryElement.y + 1) % NY };
        std::array<unsigned int,3> const z_c = { (NZ + boundaryElement.z - 1) % NZ, boundaryElement.z, (boundaryElement.z + 1) % NZ };

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
                unsigned int const curr = n*LT<T>::OFF + d;
                BC::population_->A[BC::population_-> template indexRead<TS>(x_c,y_c,z_c,n,d,BC::p_)] = LT<T>::MASK[curr]*(feq[curr] + fneq[curr]);
            }
        }
    }

    return;
}

template <boundary::Type TP, boundary::Orientation O, unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT,
          typename T, unsigned int NPOP> template <timestep TS>
void Guo<TP,O,NX,NY,NZ,LT,T,NPOP>::implementationAfterCollisionOperator()
{
    return;
}

#endif // LBT_BOUNDARY_GUO
