#ifndef LBT_BOUNDARY_GUO
#define LBT_BOUNDARY_GUO
#pragma once

/**
 * \file     boundary_guo.hpp
 * \mainpage Interpolation boundary condition for pressure and velocity according to Guo
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <cstdint>

#include "../../general/type_definitions.hpp"
#include "normal.hpp"
#include "orientation.hpp"
#include "type.hpp"


namespace lbt {
  namespace boundary {

    /**\fn        getComponent
     * \brief     Function for determining if the tangential or normal component should be used
     *
     * \tparam    TN                 Orientation: 0 for tangential and +-1 for normal
     * \tparam    A                  Floating data type of the tangential value
     * \tparam    B                  Floating data type of the normal value
     * \param[in] tangential_value   Value that is returned if the velocity is tangential
     * \param[in] normal_value       Value that is returned if the velocity is normal
     * \return    Depending on the orientation either the tangential or normal value
    */
    template <std::int32_t TN, typename A, typename B>
    static constexpr auto getComponent(A const tangential_value, B const normal_value) noexcept {
      if constexpr (TN == 0) {
        return tangential_value;
      } else {
        return normal_value;
      }
    }

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
         * \tparam    T                     Floating data type used for simulation
         * \param[in] boundary_values       Values at the boundary
         * \param[in] interpolated_values   Interpolated values
         * \return    An array that contains a mixture of enforced or interpolated values depending on the boundary type and orientation
        */
        template <typename T>
        static constexpr lbt::StackArray<T,4> get(lbt::StackArray<T,4> const& boundary_values, lbt::StackArray<T,4> const& interpolated_values) noexcept;
    };

    /**\class Velocity: Partial template specialisation
     * \brief Class for velocity boundaries: enforce normal and tangential velocities
     *        Velocity values are given and the density is interpolated
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
     *        The density is given and depending on the component normal to the boundary has to be interpolated
    */
    template <Orientation O>
    class MacroscopicValues<O,Type::Pressure> {
      public:
        template <typename T>
        static constexpr lbt::StackArray<T,4> get(lbt::StackArray<T,4> const& boundary_values, lbt::StackArray<T,4> const& interpolated_values) noexcept {
          return { boundary_values[0],
                   getComponent<Normal<O>::x>(boundary_values[1], interpolated_values[1]),
                   getComponent<Normal<O>::y>(boundary_values[2], interpolated_values[2]),
                   getComponent<Normal<O>::z>(boundary_values[3], interpolated_values[3]) };
        }
    };

  }
}

#endif // LBT_BOUNDARY_GUO
