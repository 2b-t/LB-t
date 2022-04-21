/**
 * \file     area_literals.hpp
 * \mainpage Contains definitions for user-defined literals for areas
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_AREA_LITERALS
#define LBT_UNIT_AREA_LITERALS
#pragma once

#include "area.hpp"
#include "length.hpp"
#include "length_literals.hpp"
#include "operators.hpp"


namespace lbt {
  namespace literals {

    /**\fn        operator "" _km2
     * \brief     User-defined literal for an area given in square kilometers
     * 
     * \param[in] k   The area in square kilometers
     * \return    An area in the base unit square meters
    */
    constexpr lbt::unit::Area operator "" _km2(long double const k) noexcept {
      return k*lbt::unit::Area{1.0_km*1.0_km};
    }
    /**\fn        operator "" _m2
     * \brief     User-defined literal for an area given in square meters
     * 
     * \param[in] m   The area in square meters
     * \return    An area in the base unit square meters
    */
    constexpr lbt::unit::Area operator "" _m2(long double const m) noexcept {
      return m*lbt::unit::Area{1.0_m*1.0_m};
    }
    /**\fn        operator "" _cm2
     * \brief     User-defined literal for an area given in square centimeters
     * 
     * \param[in] c   The area in square centimeters
     * \return    An area in the base unit square meters
    */
    constexpr lbt::unit::Area operator "" _cm2(long double const c) noexcept {
      return c*lbt::unit::Area{1.0_cm*1.0_cm};
    }
    /**\fn        operator "" _mm2
     * \brief     User-defined literal for an area given in square millimeters
     * 
     * \param[in] m   The area in square millimeters
     * \return    An area in the base unit square meters
    */
    constexpr lbt::unit::Area operator "" _mm2(long double const m) noexcept {
      return m*lbt::unit::Area{1.0_mm*1.0_mm};
    }

  }
}

#endif // LBT_UNIT_AREA_LITERALS
