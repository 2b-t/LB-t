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

      class Air {
        public:
          static constexpr auto molecular_weight = 28.966_gpmol; // In g/mol
      };

      class CarbonDioxide {
        public:
          static constexpr auto molecular_weight = 44.01_gpmol;
          static constexpr auto kinetic_diameter = 330.0_pm; // In picometers
      };

      class Hydrogen {
        public:
          static constexpr auto molecular_weight = 2.016_gpmol;
          static constexpr auto kinetic_diameter = 289.0_pm;
      };

      class Oxygen {
        public:
          static constexpr auto molecular_weight = 31.9988_gpmol;
          static constexpr auto kinetic_diameter = 346.0_pm;
      };

    }
  }
}

#endif // LBT_MATERIAL_PHYSICAL_CONSTANTS
