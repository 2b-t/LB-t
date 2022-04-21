/**
 * \file     physical_constants.hpp
 * \mainpage Contains classes for calculating physical properties of ideal gases
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_PHYSICAL_CONSTANTS
#define LBT_MATERIAL_PHYSICAL_CONSTANTS
#pragma once

#include "../../unit/literals.hpp"
#include "../../unit/units.hpp"


namespace lbt {
  namespace material {

    // TODO: Add unit and user-defined literals for individual gas constant
    namespace physical_constant {
      using namespace lbt::literals;

      template <typename T>
      class Air {
        public:
          static constexpr T molecular_mass = 28.966; // In SI-unit [g/mol]
      };

      template <typename T>
      class CarbonDioxide {
        public:
          static constexpr T molecular_mass = 44.01;
          static constexpr auto kinetic_diameter = 330.0_pm; // In picometers
      };

      template <typename T>
      class Hydrogen {
        public:
          static constexpr T molecular_mass = 2.016;
          static constexpr auto kinetic_diameter = 289.0_pm;
      };

      template <typename T>
      class Oxygen {
        public:
          static constexpr T molecular_mass = 31.9988;
          static constexpr auto kinetic_diameter = 346.0_pm;
      };

    }
  }
}

#endif // LBT_MATERIAL_PHYSICAL_CONSTANTS
