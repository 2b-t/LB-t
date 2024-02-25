/**
 * \file     kinematic_viscosity_literals.hpp
 * \mainpage Contains definitions for user-defined literals for kinematic viscosity
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_KINEMATIC_VISCOSITY_LITERALS
#define LBT_UNIT_KINEMATIC_VISCOSITY_LITERALS
#pragma once

#include "lbt/unit/detail/kinematic_viscosity.hpp"
#include "lbt/unit/detail/length.hpp"
#include "lbt/unit/detail/length_literals.hpp"
#include "lbt/unit/detail/operators.hpp"
#include "lbt/unit/detail/time.hpp"
#include "lbt/unit/detail/time_literals.hpp"


namespace lbt {
  namespace literals {

    /**\fn        operator "" _m2ps
     * \brief     User-defined literal for a kinematic viscosity given in square meters per second
     * 
     * \param[in] p   The kinematic viscosity in square meters per second
     * \return    A kinematic viscosity in the base unit meters squared per second
    */
    constexpr lbt::unit::KinematicViscosity operator "" _m2ps(long double const m) noexcept {
      return m*lbt::unit::KinematicViscosity{1.0_m*1.0_m/1.0_s};
    }
    /**\fn        operator "" _St
     * \brief     User-defined literal for a kinematic viscosity given in Stokes
     * 
     * \param[in] p   The kinematic viscosity in Stokes
     * \return    A kinematic viscosity in the base unit meters squared per second
    */
    constexpr lbt::unit::KinematicViscosity operator "" _St(long double const s) noexcept {
      return s*lbt::unit::KinematicViscosity{1.0e-4};
    }
    /**\fn        operator "" _cSt
     * \brief     User-defined literal for a kinematic viscosity given in Centistokes
     * 
     * \param[in] p   The kinematic viscosity in Centistokes
     * \return    A kinematic viscosity in the base unit meters squared per second
    */
    constexpr lbt::unit::KinematicViscosity operator "" _cSt(long double const c) noexcept {
      return c*lbt::unit::KinematicViscosity{1.0e-6};
    }

  }
}

#endif // LBT_UNIT_KINEMATIC_VISCOSITY_LITERALS
