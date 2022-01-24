#ifndef LBT_MATERIAL_IDEAL_GAS
#define LBT_MATERIAL_IDEAL_GAS
#pragma once

/**\file     ideal_gas.hpp
 * \mainpage Contains methods for calulcating physical properties of ideal gases
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include "../../general/constexpr_math.hpp"
#include "../literals.hpp"
#include "../units.hpp"


namespace lbt {
  namespace material {
    namespace ideal_gas {

      /*using namespace lbt::literals;

      // Viscosity: Sutherland's law: https://de.wikipedia.org/wiki/Sutherland-Modell

      // TODO: Add unit and user-defined literal for individual gas constant
      namespace physical_constant {
        // See https://en.wikipedia.org/wiki/Gas_constant
        template <typename T>
        constexpr T universal_gas_constant = static_cast<T>(8.31446261815324L); // IN SI-units [J/(K mol)]

        template <typename T = long double>
        class Air {
          public:
            static constexpr T molar_mass = 28.966; // In SI-unit [kg/mol]
            static constexpr T specific_gas_constant = universal_gas_constant<T>/molar_mass; // 287.05; // In SI-units [J/kg K]
        };

        template <typename T>
        class CarbonDioxide {
          public:
            static constexpr T molar_mass = 44.01;
            static constexpr T specific_gas_constant = universal_gas_constant<T>/molar_mass; // 188.92;
        };

        template <typename T>
        class Hydrogen {
          public:
            static constexpr T molar_mass = 2.016;
            static constexpr T specific_gas_constant = universal_gas_constant<T>/molar_mass; // 4124.2;
        };

        template <typename T>
        class Oxygen {
          public:
            static constexpr T molar_mass = 31.9988;
            static constexpr T specific_gas_constant = universal_gas_constant<T>/molar_mass; // 259.84;
        };
      }

      template <template <typename> typename T>
      class IdealGas {
        public:
          // Calculated according to ideal gas law 
          static constexpr lbt::unit::Density density(lbt::unit::Temperature const t = 0.0_deg, 
                                                      lbt::unit::Pressure const p = 1.0_atm) noexcept {
            return lbt::unit::Density{p.get/(T<long double>::specific_gas_constant*t.get())};
          };
          // Calculated according to Maxwell-Boltzmann distribution https://physics.stackexchange.com/a/510860/245414
          static constexpr lbt::unit::KinematicViscosity kinematic_viscosity(lbt::unit::Temperature const t = 0.0_deg, 
                                                                             lbt::unit::Pressure const p = 1.0_atm) noexcept {
            return lbt::unit::KinematicViscosity{0.0};
          }
          static constexpr lbt::unit::DynamicViscosity dynamic_viscosity(lbt::unit::Temperature const t = 0.0_deg, 
                                                                         lbt::unit::Pressure const p = 1.0_atm) noexcept {
            return lbt::unit::DynamicViscosity{0.0};
          }
      };

      /// Define convenient aliases for different ideal gases
      using Air = IdealGas<physical_constant::Air>;
      using CarbonDioxide = IdealGas<physical_constant::CarbonDioxide>;
      using Hydrogen = IdealGas<physical_constant::Hydrogen>;
      using Oxygen = IdealGas<physical_constant::Oxygen>;*/

    }
  }
}

#endif // LBT_MATERIAL_IDEAL_GAS
