/**
 * \file     temperature_literals.hpp
 * \mainpage Contains definitions for user-defined literals for temperature
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_TEMPERATURE_LITERALS
#define LBT_UNIT_TEMPERATURE_LITERALS
#pragma once

#include "temperature.hpp"


namespace lbt {
  namespace literals {

    /**\fn        operator "" _K
     * \brief     User-defined literal for a temperature given in Kelvin
     * 
     * \param[in] t   The temperature in Kelvin
     * \return    A temperature in the base unit Kelvin
    */
    constexpr lbt::unit::Temperature operator "" _K(long double const t) noexcept {
      return lbt::unit::Temperature{t};
    }
    /**\fn        operator "" _deg
     * \brief     User-defined literal for a temperature given in degrees
     * 
     * \param[in] t   The temperature in degrees
     * \return    A temperature in the base unit Kelvin
    */
    constexpr lbt::unit::Temperature operator "" _deg(long double const t) noexcept {
      return lbt::unit::Temperature{t + 273.15};
    }

  }
}

#endif // LBT_UNIT_TEMPERATURE_LITERALS
