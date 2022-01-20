#ifndef LBT_BOUNDARY_TYPE
#define LBT_BOUNDARY_TYPE
#pragma once

/**
 * \file     boundary_type.hpp
 * \mainpage Holds classes for different types of boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>

#include "../../general/type_definitions.hpp"
#include "boundary_orientation.hpp"


namespace lbt {
  namespace boundary {

    /**\fn        velocityComponent
     * \brief     Function for determining if the velocity is tangential of normal
     *
     * \tparam    T                       Floating data type used for the simulation
     * \param[in] tangential_or_normal    Orientation: 0 for tangential and +-1 for normal
     * \param[in] tangential_value        Value that is returned if the velocity is tangental
     * \param[in] normal_value            Value that is returned if the velocity is normal
    */
    template <typename T>
    static constexpr T velocityComponent(std::int32_t const tangential_or_normal, T const tangential_value, T const normal_value) noexcept {
      return (tangential_or_normal == 0) ? tangential_value : normal_value;
    }

    /**\enum  boundary::Type
     * \brief Strongly typed for different boundary condition types
    */
    enum class Type { Velocity, Pressure };

    /**\class  MacroscopicValues
     * \brief  Class for the macroscopic values for each individual type and orientation of the boundary condition
     *
     * \tparam O    Orientation of the normal
     * \tparam TP   Type of the boundary condition
    */
    template <Orientation O, Type TP>
    class MacroscopicValues {
      public:
        /**\fn        get
         * \brief     Determine if the values of the boundary or the interpolated values from the simulation should be used
         *
         * \tparam    T    Floating data type used for simulation
         * \param[in] boundary_values       Values at the boundary
         * \param[in] interpolated_values   Interpolated values
         * \return    An array that contains a mixture of enforced or interpolated values depending on the precise boundary
        */
        template <typename T>
        static constexpr lbt::StackArray<T,4> get(lbt::StackArray<T,4> const& boundary_values, lbt::StackArray<T,4> const& interpolated_values) noexcept;
    };

    /**\class Velocity: Partial template specialisation
     * \brief Class for velocity boundaries: enforce normal and tangential velocities
    */
    template <Orientation O>
    class MacroscopicValues<O,Type::Velocity> {
      public:
        template <typename T>
        static constexpr lbt::StackArray<T,4> get(lbt::StackArray<T,4> const& boundary_values, lbt::StackArray<T,4> const& interpolated_values) noexcept {
          return { interpolated_values[0], boundary_values[1], boundary_values[2], boundary_values[3] };
        }
    };

    /**\class Pressure: Partial template specialisation
     * \brief Class for pressure boundaries: enforce rho and tangential velocities
    */
    template <Orientation O>
    class MacroscopicValues<O,Type::Pressure> {
      public:
        template <typename T>
        static constexpr lbt::StackArray<T,4> get(lbt::StackArray<T,4> const& boundary_values, lbt::StackArray<T,4> const& interpolated_values) noexcept {
          return { boundaryValues[0],
                   velocityComponent(Normal<O>::x, boundary_values[1], interpolated_values[1]),
                   velocityComponent(Normal<O>::y, boundary_values[2], interpolated_values[2]),
                   velocityComponent(Normal<O>::z, boundary_values[3], interpolated_values[3]) };
        }
    };

  }
}

#endif // LBT_BOUNDARY_TYPE
