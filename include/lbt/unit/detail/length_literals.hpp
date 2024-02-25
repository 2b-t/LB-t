/**
 * \file     length_literals.hpp
 * \mainpage Contains definitions for user-defined literals for lengths
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_LENGTH_LITERALS
#define LBT_UNIT_LENGTH_LITERALS
#pragma once

#include "lbt/unit/detail/length.hpp"


namespace lbt {
  namespace literals {

    /**\fn        operator "" _km
     * \brief     User-defined literal for a length given in kilometers
     * 
     * \param[in] k   The distance in kilometers
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _km(long double const k) noexcept {
      return k*lbt::unit::Length{1.0e3};
    }
    /**\fn        operator "" _m
     * \brief     User-defined literal for a length given in meters
     * 
     * \param[in] m   The distance in meters
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _m(long double const m) noexcept {
      return m*lbt::unit::Length{1.0};
    }
    /**\fn        operator "" _dm
     * \brief     User-defined literal for a length given in decimeters
     * 
     * \param[in] d   The distance in decimeters
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _dm(long double const d) noexcept {
      return d*lbt::unit::Length{1.0e-1};
    }
    /**\fn        operator "" _cm
     * \brief     User-defined literal for a length given in centimeters
     * 
     * \param[in] c   The distance in centimeters
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _cm(long double const c) noexcept {
      return c*lbt::unit::Length{1.0e-2};
    }
    /**\fn        operator "" _mm
     * \brief     User-defined literal for a length given in millimeters
     * 
     * \param[in] m   The distance in millimetres
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _mm(long double const m) noexcept {
      return m*lbt::unit::Length{1.0e-3};
    }
    /**\fn        operator "" _um
     * \brief     User-defined literal for a length given in micrometers
     * 
     * \param[in] u   The distance in micrometers
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _um(long double const u) noexcept {
      return u*lbt::unit::Length{1.0e-6};
    }
    /**\fn        operator "" _pm
     * \brief     User-defined literal for a length given in picometers
     * 
     * \param[in] u   The distance in picometers
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _pm(long double const u) noexcept {
      return u*lbt::unit::Length{1.0e-12};
    }

  }
}

#endif // LBT_UNIT_LENGTH_LITERALS
