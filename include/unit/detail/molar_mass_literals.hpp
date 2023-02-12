/**
 * \file     molar_mass_literals.hpp
 * \mainpage Contains definitions for user-defined literals for molar mass
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_MOLAR_MASS_LITERALS
#define LBT_UNIT_MOLAR_MASS_LITERALS
#pragma once

#include "amount_of_substance.hpp"
#include "amount_of_substance_literals.hpp"
#include "mass.hpp"
#include "mass_literals.hpp"
#include "molar_mass.hpp"
#include "operators.hpp"


namespace lbt {
  namespace literals {

    /**\fn        operator "" _gpmol
     * \brief     User-defined literal for a molar mass given in gram per mole
     * 
     * \param[in] g   The molar mass in gram per mole
     * \return    A molar mass in the base unit kilogram per mole
    */
    constexpr lbt::unit::MolarMass operator "" _gpmol(long double const g) noexcept {
      return g*lbt::unit::MolarMass{1.0_g/1.0_mol};
    }
    /**\fn        operator "" _kgpmol
     * \brief     User-defined literal for a molar mass given in kilogram per mole
     * 
     * \param[in] k   The molar mass in kilogram per mole
     * \return    A molar mass in the base unit kilogram per mole
    */
    constexpr lbt::unit::MolarMass operator "" _kgpmol(long double const k) noexcept {
      return k*lbt::unit::MolarMass{1.0};
    }

  }
}

#endif // LBT_UNIT_MOLAR_MASS_LITERALS
