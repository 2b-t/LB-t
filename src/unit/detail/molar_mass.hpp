/**
 * \file     molar_mass.hpp
 * \mainpage Contains unit definition for a molar mass
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_MOLAR_MASS
#define LBT_UNIT_MOLAR_MASS
#pragma once

#include "unit_base.hpp"


namespace lbt {
  namespace unit {

    /**\class MolarMass
     * \brief Unit class for molar mass
    */
    class MolarMass : public lbt::unit::detail::UnitBase<MolarMass> {
      public:
        /**\fn    Mass
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit kilogram per mole
        */
        explicit constexpr MolarMass(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        MolarMass(MolarMass const&) = default;
        MolarMass& operator= (MolarMass const&) = default;
        MolarMass(MolarMass&&) = default;
        MolarMass& operator= (MolarMass&&) = default;
    };

  }
}

#endif // LBT_UNIT_MOLAR_MASS
