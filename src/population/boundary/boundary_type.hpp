#ifndef LBT_BOUNDARY_TYPE
#define LBT_BOUNDARY_TYPE

/**
 * \file     boundary_type.hpp
 * \mainpage Holds classes for different types boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <array>

#include "boundary_orientation.hpp"


namespace boundary
{
    /**\fn        velocityComponent
     * \brief     Function for determining if the velocity is tangential of normal
     *
     * \tparam    T                     Floating data type used for the simulation
     * \param[in] tangentialOrNormal    Orientation: 0 for tangential and +-1 for normal
     * \param[in] tangentialComponent   Value that is returned if the velocity is tangental
     * \param[in] normalComponent       Value that is returned if the velocity is normal
    */
    template <typename T>
    static inline T velocityComponent(int const tangentialOrNormal, T const tangentialComponent, T const normalComponent)
    {
        return (tangentialOrNormal == 0) ? tangentialComponent : normalComponent;
    }

    /**\enum  boundary::Type
     * \brief Strongly typed for different boundary condition types
    */
    enum class Type { Velocity, Pressure };

    /**\class  MacroscopicValues
     * \brief  Class for the macroscopic values for each individual type and orientation of the boundary condition
     *
     * \tparam T    Floating data type used for simulation
     * \tparam O    Orientation of the normal
     * \tparam TP   Type of the boundary condition
    */
    template <typename T, Orientation O, Type TP>
    class MacroscopicValues final
    {
        public:
            /**\fn        get
             * \brief     Determine if the values of the boundary or the interpolated values from the simulation should be used
             *
             * \param[in] boundaryValues       Values at the boundary
             * \param[in] interpolatedValues   Interpolated values
             * \return    An array that contains a mixture of enforced or interpolated values depending on the precise boundary
            */
            static inline std::array<T,4> get(std::array<T,4> const& boundaryValues, std::array<T,4> const& interpolatedValues);
    };

    /**\class Velocity: Partial template specialisation
     * \brief Class for velocity boundaries: enforce normal and tangential velocities
    */
    template <typename T, Orientation O>
    class MacroscopicValues<T,O,Type::Velocity> final
    {
        public:
            static inline std::array<T,4> get(std::array<T,4> const& boundaryValues, std::array<T,4> const& interpolatedValues)
            {
                return { interpolatedValues[0], boundaryValues[1], boundaryValues[2], boundaryValues[3] };
            }
    };

    /**\class Pressure: Partial template specialisation
     * \brief Class for pressure boundaries: enforce rho and tangential velocities
    */
    template <typename T, Orientation O>
    class MacroscopicValues<T,O,Type::Pressure> final
    {
        public:
            static inline std::array<T,4> get(std::array<T,4> const& boundaryValues, std::array<T,4> const& interpolatedValues)
            {
                return { boundaryValues[0],
                         velocityComponent(Normal<O>::x, boundaryValues[1], interpolatedValues[1]),
                         velocityComponent(Normal<O>::y, boundaryValues[2], interpolatedValues[2]),
                         velocityComponent(Normal<O>::z, boundaryValues[3], interpolatedValues[3]) };
            }
    };
}

#endif // LBT_BOUNDARY_TYPE
