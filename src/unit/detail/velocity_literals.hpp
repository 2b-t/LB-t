/**
 * \file     velocity_literals.hpp
 * \mainpage Contains definitions for user-defined literals for velocity
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_VELOCITY_LITERALS
#define LBT_UNIT_VELOCITY_LITERALS
#pragma once

#include "length.hpp"
#include "length_literals.hpp"
#include "operators.hpp"
#include "time.hpp"
#include "time_literals.hpp"
#include "velocity.hpp"


namespace lbt {
  namespace literals {

    /**\fn        operator "" _kmph
     * \brief     User-defined literal for a velocity given in kilometers per hour
     * 
     * \param[in] k   The velocity in kilometers per hour
     * \return    A velocity in the base unit meters per second
    */
    constexpr lbt::unit::Velocity operator "" _kmph(long double const k) noexcept {
      return k*lbt::unit::Velocity{1.0_km/1.0_h};
    }
    /**\fn        operator "" _mps
     * \brief     User-defined literal for a velocity given in meters per second
     * 
     * \param[in] m   The velocity in meters per second
     * \return    A velocity in the base unit meters per second
    */
    constexpr lbt::unit::Velocity operator "" _mps(long double const m) noexcept {
      return m*lbt::unit::Velocity{1.0_m/1.0_s};
    }
    /**\fn        operator "" _cmps
     * \brief     User-defined literal for a velocity given in centimeters per second
     * 
     * \param[in] c   The velocity in centimeters per second
     * \return    A velocity in the base unit meters per second
    */
    constexpr lbt::unit::Velocity operator "" _cmps(long double const c) noexcept {
      return c*lbt::unit::Velocity{1.0_cm/1.0_s};
    }
    /**\fn        operator "" _mmps
     * \brief     User-defined literal for a velocity given in millimeters per second
     * 
     * \param[in] m   The velocity in millimeters per second
     * \return    A velocity in the base unit meters per second
    */
    constexpr lbt::unit::Velocity operator "" _mmps(long double const m) noexcept {
      return m*lbt::unit::Velocity{1.0_mm/1.0_s};
    }

  }
}

#endif // LBT_UNIT_VELOCITY_LITERALS
