#ifndef LBT_BOUNDARY_BOUNCEBACK
#define LBT_BOUNDARY_BOUNCEBACK

/**
 * \file     boundary_bounceback.hpp
 * \mainpage Solid walls with simple bounce-back boundaries
*/

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
 * \tparam NX   Simulation domain resolution in x-direction
 * \tparam NY   Simulation domain resolution in y-direction
 * \tparam NZ   Simulation domain resolution in z-direction
 * \tparam LT   Static lattice::DdQq class containing discretisation parameters
 * \tparam T    Floating data type used for simulation
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
class HalfwayBounceBack: public BoundaryCondition<NX,NY,NZ,LT,T,HalfwayBounceBack<NX,NY,NZ,LT,T>>
{
    using BC = BoundaryCondition<NX,NY,NZ,LT,T,HalfwayBounceBack<NX,NY,NZ,LT,T>>;

    public:
        /**\brief     Constructor
         * 
         * \param[in] population         Population object holding microscopic distributions
         * \param[in] boundaryElements   Elements making up the boundary
         * \param[in] p                  Index of relevant population
        */
        HalfwayBounceBack(std::shared_ptr<Population<NX,NY,NZ,LT>> population, std::vector<BoundaryElement<T>> const& boundaryElements, 
                          unsigned int const p = 0):
            BC(population, boundaryElements, p), population_(population), boundaryElements_(boundaryElements), p_(p)
        {
            return;
        }

        /**\fn     implementationBeforeCollisionOperator
         * \brief  Implementation of the boundary condition to be performed after the collision operator
         * 
         * \tparam AA   the timestep in the AA-pattern
        */
        template<timestep AA>
        void implementationBeforeCollisionOperator();

        /**\fn     implementationAfterCollisionOperator
         * \brief  Implementation of the boundary condition to be performed before the collision operator
         * 
         * \tparam AA   the timestep in the AA-pattern
        */
        template<timestep AA>
        void implementationAfterCollisionOperator();
        
    protected:
        std::shared_ptr<Population<NX,NY,NZ,LT>> population_;
        std::vector<BoundaryElement<T>> const boundaryElements_;
        unsigned int const p_;
};

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T> template<timestep AA>
void HalfwayBounceBack<NX,NY,NZ,LT,T>::implementationBeforeCollisionOperator()
{
    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T> template<timestep AA>
void HalfwayBounceBack<NX,NY,NZ,LT,T>::implementationAfterCollisionOperator()
{
    #pragma omp parallel for default(none) shared(boundaryElements_,population_,p_) schedule(static,32)
    for(size_t i = 0; i < BC::boundaryElements_.size(); ++i)
    {
        auto const& boundaryElement = boundaryElements_[i];
        unsigned int const x_n[3] = { (NX + boundaryElement.x - 1) % NX, boundaryElement.x, (boundaryElement.x + 1) % NX };
        unsigned int const y_n[3] = { (NY + boundaryElement.y - 1) % NY, boundaryElement.y, (boundaryElement.y + 1) % NY };
        unsigned int const z_n[3] = { (NZ + boundaryElement.z - 1) % NZ, boundaryElement.z, (boundaryElement.z + 1) % NZ };

        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #pragma GCC unroll (15)
            for(unsigned int d = 1; d < LT::HSPEED; ++d)
            {
                population_->F_[population_-> template AA_IndexWrite<AA>(x_n, y_n, z_n, !n, d, p_)] = 
                population_->F_[population_-> template AA_IndexRead<!AA>(x_n, y_n, z_n, n, d, p_)];
            }
        }
    }

    return;
}

#endif // LBT_BOUNDARY_BOUNCEBACK
