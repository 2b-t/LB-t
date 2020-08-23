#ifndef BOUNDARY_HPP_INCLUDED
#define BOUNDARY_HPP_INCLUDED

/**
 * \file     boundary.hpp
 * \mainpage Holds struct for boundary element information
*/

#include <memory>

#include "../../continuum/continuum.hpp"
#include "../population.hpp"


/**\struct BoundaryElement
 * \brief  Struct holding properties of boundary element
 *
 * \tparam T   Floating data type used for simulation
*/
template <typename T>
struct BoundaryElement
{
    /// spatial position of element
    unsigned int const x;
    unsigned int const y;
    unsigned int const z;

    /// macroscopic properties
    T const rho; ///< density
    T const u;   ///< velocity in x-direction
    T const v;   ///< velocity in y-direction
    T const w;   ///< velocity in z-direction
};


/**\class BoundaryCondition
 * \brief Base class for all boundary conditions
 * 
 * \tparam NX             Simulation domain resolution in x-direction
 * \tparam NY             Simulation domain resolution in y-direction
 * \tparam NZ             Simulation domain resolution in z-direction
 * \tparam LT             Static lattice::DdQq class containing discretisation parameters
 * \tparam T              Floating data type used for simulation
 * \tparam DerivedClass   The derived boundary condition that inherits from this base class
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T, typename DerivedClass>
class BoundaryCondition
{
    public:
        /**\fn     beforeCollisionOperator
         * \brief  Curiously Recurring Template Pattern (CRTP) for static polymorphism of the part of the boundary
         *         condition to be performed before collision
         * 
         * \tparam AA   The timestep in the AA-pattern
        */
        template <timestep AA>
        void beforeCollisionOperator()
        {
            static_cast<DerivedClass*>(this)->template implementationBeforeCollisionOperator<AA>();

            return;
        }

        /**\fn     afterCollisionOperator
         * \brief  Curiously Recurring Template Pattern (CRTP) for static polymorphism of the part of the boundary
         *         condition to be performed after collision
         * 
         * \tparam AA   The timestep in the AA-pattern
        */
        template <timestep AA>
        void afterCollisionOperator()
        {
            static_cast<DerivedClass*>(this)->template implementationAfterCollisionOperator<AA>();

            return;
        }

        std::vector<BoundaryElement<T>> const boundaryElements_; ///< public in order to avoid circular dependency with continuum
        
    protected:
        /**\brief     Constructor
         * 
         * \param[in] population         Population object holding microscopic distributions
         * \param[in] boundaryElements   Elements making up the boundary
         * \param[in] p                  Index of relevant population
        */
        BoundaryCondition(std::shared_ptr<Population<NX,NY,NZ,LT>> population, std::vector<BoundaryElement<T>> boundaryElements, 
                          unsigned int const p = 0):
            population_(population), boundaryElements_(boundaryElements), p_(p)
        {
            return;
        }

        std::shared_ptr<Population<NX,NY,NZ,LT>> population_;
        unsigned int const p_;
};

#endif // BOUNDARY_HPP_INCLUDED
