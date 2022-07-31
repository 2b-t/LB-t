/**
 * \file     ideal_gas.hpp
 * \mainpage Contains methods for calculating physical properties of ideal gases
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_IDEAL_GAS
#define LBT_MATERIAL_IDEAL_GAS
#pragma once

#include "../../constexpr_math/constexpr_math.hpp"
#include "../../unit/literals.hpp"
#include "../../unit/units.hpp"


namespace lbt {
  namespace material {

    /// Traits with physical constants
    namespace physical_constants {
      using namespace lbt::literals;

      class Air {
        public:
          static constexpr auto molecular_weight = 28.966_gpmol;
          static constexpr auto c = 120.0_K; // Constants for Sutherland's model: https://de.wikipedia.org/wiki/Sutherland-Modell
          static constexpr auto t_0 = 291.15_K;
          static constexpr auto mu_0 = 18.27_Pas;
      };

      class CarbonDioxide {
        public:
          static constexpr auto molecular_weight = 44.01_gpmol;
          static constexpr auto c = 240.0_K;
          static constexpr auto t_0 = 293.15_K;
          static constexpr auto mu_0 = 14.8_Pas;
      };

      class Hydrogen {
        public:
          static constexpr auto molecular_weight = 2.016_gpmol;
          static constexpr auto c = 72.0_K;
          static constexpr auto t_0 = 293.85_K;
          static constexpr auto mu_0 = 8.76_Pas;
      };

      class Oxygen {
        public:
          static constexpr auto molecular_weight = 31.9988_gpmol;
          static constexpr auto c = 127.0_K;
          static constexpr auto t_0 = 292.25_K;
          static constexpr auto mu_0 = 20.18_Pas;
      };

    }

    /**\class  IdealGas
     * \brief  Class for calculating the equation of state according to the ideal gas law
     *
     * \tparam T   The struct containing the material parameters that the equation of state should be calculated for
    */
    template <typename T>
    class IdealGas {
      public:
        /**\fn    equationOfState
         * \brief Calculate the missing state variable from the other two using the ideal gas law
        */
        static constexpr lbt::unit::Density equationOfState(lbt::unit::Temperature const t, 
                                                            lbt::unit::Pressure const p) noexcept {
          return lbt::unit::Density{p.get()/(specific_gas_constant*t.get())};
        };
        static constexpr lbt::unit::Temperature equationOfState(lbt::unit::Density const rho, 
                                                                lbt::unit::Pressure const p) noexcept {
          return lbt::unit::Temperature{p.get()/(specific_gas_constant*rho.get())};
        };
        static constexpr lbt::unit::Pressure equationOfState(lbt::unit::Density const rho, 
                                                             lbt::unit::Temperature const t) noexcept {
          return lbt::unit::Pressure{specific_gas_constant*rho.get()*t.get()};
        };

        /**\fn    kinematicViscosity
         * \brief Calculate the kinematic viscosity with Sutherland's model (https://de.wikipedia.org/wiki/Sutherland-Modell) and the ideal gas law
        */
        static constexpr lbt::unit::KinematicViscosity kinematicViscosity(lbt::unit::Temperature const t, 
                                                                          lbt::unit::Pressure const p) noexcept {
          return dynamicViscosity(t)/equationOfState(t, p);
        }
        static constexpr lbt::unit::KinematicViscosity kinematicViscosity(lbt::unit::Density const rho, 
                                                                          lbt::unit::Temperature const t) noexcept {
          return dynamicViscosity(t)/rho;
        }

        /**\fn    dynamicViscosity
         * \brief Calculate the dynamic viscosity with Sutherland's model (https://de.wikipedia.org/wiki/Sutherland-Modell)
        */
        static constexpr lbt::unit::DynamicViscosity dynamicViscosity(lbt::unit::Temperature const t) noexcept {
          return T::mu_0*(T::t_0 + T::c)/(t + T::c)*cem::pow(t/T::t_0, 3/2);
        }

      protected:
        // TODO(tobit): Introduce remaining SI-units
        // See https://en.wikipedia.org/wiki/Gas_constant
        static constexpr long double universal_gas_constant {8.31446261815324L}; // In SI-units [J/(K mol)]
        static constexpr long double avogadro_constant {6.02214076e+23L}; // In SI-units [1/mol]
        static constexpr long double specific_gas_constant {universal_gas_constant/T::molecular_weight.get()}; // In SI-units [J/(kg K)]
    };

    using Air = IdealGas<physical_constants::Air>;
    using CarbonDioxide = IdealGas<physical_constants::CarbonDioxide>;
    using Hydrogen = IdealGas<physical_constants::Hydrogen>;
    using Oxygen = IdealGas<physical_constants::Oxygen>;

  }
}

#endif // LBT_MATERIAL_IDEAL_GAS
