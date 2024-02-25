/**
 * \file     volume_literals.hpp
 * \mainpage Contains definitions for user-defined literals for volume
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_VOLUME_LITERALS
#define LBT_UNIT_VOLUME_LITERALS
#pragma once

#include "lbt/unit/detail/length.hpp"
#include "lbt/unit/detail/length_literals.hpp"
#include "lbt/unit/detail/operators.hpp"
#include "lbt/unit/detail/volume.hpp"


namespace lbt {
  namespace literals {

    /**\fn        operator "" _km3
     * \brief     User-defined literal for a volume given in cubic kilometers
     * 
     * \param[in] k   The volume in cubic kilometers
     * \return    A volume in the base unit cubic meters
    */
    constexpr lbt::unit::Volume operator "" _km3(long double const k) noexcept {
      return k*lbt::unit::Volume{1.0_km*1.0_km*1.0_km};
    }
    /**\fn        operator "" _m3
     * \brief     User-defined literal for a volume given in cubic meters
     * 
     * \param[in] m   The volume in cubic meters
     * \return    A volume in the base unit cubic meters
    */
    constexpr lbt::unit::Volume operator "" _m3(long double const m) noexcept {
      return m*lbt::unit::Volume{1.0_m*1.0_m*1.0_m};
    }
    /**\fn        operator "" _cm3
     * \brief     User-defined literal for a volume given in cubic centimeters
     * 
     * \param[in] c   The volume in cubic centimeters
     * \return    A volume in the base unit cubic meters
    */
    constexpr lbt::unit::Volume operator "" _cm3(long double const c) noexcept {
      return c*lbt::unit::Volume{1.0_cm*1.0_cm*1.0_cm};
    }
    /**\fn        operator "" _mm3
     * \brief     User-defined literal for a volume given in cubic millimeters
     * 
     * \param[in] m   The volume in cubic millimeters
     * \return    A volume in the base unit cubic meters
    */
    constexpr lbt::unit::Volume operator "" _mm3(long double const m) noexcept {
      return m*lbt::unit::Volume{1.0_mm*1.0_mm*1.0_mm};
    }

  }
}

#endif // LBT_UNIT_VOLUME_LITERALS
