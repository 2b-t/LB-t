#ifndef LBT_BOUNDARY_BOUNCEBACK
#define LBT_BOUNDARY_BOUNCEBACK

/**
 * \file     boundary_bounceback.hpp
 * \mainpage Solid walls with simple bounce-back boundaries
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <array>
#include <memory>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "../population.hpp"
#include "boundary.hpp"


/**\class  HalfwayBounceBack
 * \brief  Solid wall boundary treatment with simple halfway bounce-back
 * \note   "Analytic solutions of simple flows and analysis of non-slip boundary conditions for
 *         the lattice Boltzmann BGK model"
 *         X. He, Q. Zou, L.S. Luo, M. Dembo
 *         Journal of Statistical Physics 87 (1997)
 *         DOI: 10.1007/BF02181482
 *
 * \tparam NX     Simulation domain resolution in x-direction
 * \tparam NY     Simulation domain resolution in y-direction
 * \tparam NZ     Simulation domain resolution in z-direction
 * \tparam LT     Static lattice::DdQq class containing discretisation parameters
 * \tparam NPOP   Number of populations stored side by side in a single merged grid
 * \tparam T      Floating data type used for simulation
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP>
class HalfwayBounceBack: public BoundaryCondition<NX,NY,NZ,LT,T,NPOP,HalfwayBounceBack<NX,NY,NZ,LT,T,NPOP>>
{
    using BC = BoundaryCondition<NX,NY,NZ,LT,T,NPOP,HalfwayBounceBack<NX,NY,NZ,LT,T,NPOP>>;

    public:
        HalfwayBounceBack() = delete;

        /**\brief     Constructor
         *
         * \param[in] population         Population object holding microscopic distributions
         * \param[in] boundaryElements   Elements making up the boundary
         * \param[in] p                  Index of relevant population
        */
        HalfwayBounceBack(std::shared_ptr<Population<NX,NY,NZ,LT,T,NPOP>> population, std::vector<boundary::Element<T>> const& boundaryElements,
                          unsigned int const p = 0) noexcept:
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
        void implementationBeforeCollisionOperator() noexcept;

        /**\fn     implementationAfterCollisionOperator
         * \brief  Implementation of the boundary condition to be performed before the collision operator
         *
         * \tparam TS   Even or odd timestep
        */
        template <timestep TS>
        void implementationAfterCollisionOperator() noexcept;
};

template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP> template <timestep TS>
void HalfwayBounceBack<NX,NY,NZ,LT,T,NPOP>::implementationBeforeCollisionOperator() noexcept
{
    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP> template <timestep TS>
void HalfwayBounceBack<NX,NY,NZ,LT,T,NPOP>::implementationAfterCollisionOperator() noexcept
{
    #pragma omp parallel for default(none) shared(BC::boundaryElements_,BC::population_,BC::p_) schedule(static,32)
    for(size_t i = 0; i < BC::boundaryElements_.size(); ++i)
    {
        auto const& boundaryElement = BC::boundaryElements_[i];
        std::array<unsigned int,3> const x_n = { (NX + boundaryElement.x - 1) % NX, boundaryElement.x, (boundaryElement.x + 1) % NX };
        std::array<unsigned int,3> const y_n = { (NY + boundaryElement.y - 1) % NY, boundaryElement.y, (boundaryElement.y + 1) % NY };
        std::array<unsigned int,3> const z_n = { (NZ + boundaryElement.z - 1) % NZ, boundaryElement.z, (boundaryElement.z + 1) % NZ };

        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #if defined(__ICC) || defined(__ICL)
            #pragma unroll (LT<T>::HSPEED-1)
            #else
            #pragma GCC unroll (15)
            #endif
            for(unsigned int d = 1; d < LT<T>::HSPEED; ++d)
            {
                BC::population_->A[BC::population_-> template indexWrite<TS>(x_n, y_n, z_n, !n, d, BC::p_)] =
                BC::population_->A[BC::population_-> template indexRead<!TS>(x_n, y_n, z_n, n, d, BC::p_)];
            }
        }
    }

    return;
}

#endif // LBT_BOUNDARY_BOUNCEBACK
