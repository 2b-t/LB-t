/**
 * \file     amount_of_substance_literals.hpp
 * \mainpage Contains definitions for user-defined literals for amount of substance
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_AMOUNT_OF_SUBSTANCES_LITERALS
#define LBT_UNIT_AMOUNT_OF_SUBSTANCES_LITERALS
#pragma once

#include "lbt/unit/detail/amount_of_substance.hpp"


namespace lbt {
  namespace literals {

    /**\fn        operator "" _t
     * \brief     User-defined literal for an amount of substance given in mole
     * 
     * \param[in] m   The amount of substance in mole
     * \return    An amount of substance in the base unit mole
    */
    constexpr lbt::unit::AmountOfSubstance operator "" _mol(long double const m) noexcept {
      return m*lbt::unit::AmountOfSubstance{1.0};
    }
    /**\fn        operator "" _kg
     * \brief     User-defined literal for an amount of substance given in kilomole
     * 
     * \param[in] k   The amount of substance in kilomole
     * \return    An amount of substance in the base unit mole
    */
    constexpr lbt::unit::AmountOfSubstance operator "" _kmol(long double const k) noexcept {
      return k*lbt::unit::AmountOfSubstance{1000.0};
    }

  }
}

#endif // LBT_UNIT_AMOUNT_OF_SUBSTANCES_LITERALS
