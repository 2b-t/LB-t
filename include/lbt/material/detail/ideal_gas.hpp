/**
 * \file     ideal_gas.hpp
 * \mainpage Contains methods for calculating physical properties of an ideal gases
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_IDEAL_GAS
#define LBT_MATERIAL_IDEAL_GAS
#pragma once

#include "lbt/constexpr_math/constexpr_math.hpp"
#include "lbt/unit/literals.hpp"
#include "lbt/unit/units.hpp"


namespace lbt {
  namespace material {

    /// Traits with physical constants
    /*
    namespace physical_constants {
      using namespace lbt::literals;

      class SomeGas {
        public:
          static constexpr auto molecular_weight = 0.0_gpmol;
          static constexpr auto c = 0.0_K; // Constants for Sutherland's model: https://de.wikipedia.org/wiki/Sutherland-Modell
          static constexpr auto t_0 = 0.0_K;
          static constexpr auto mu_0 = 0.0_Pas;
      };
    }
    */

    /**\class  IdealGas
     * \brief  Class for calculating the equation of state according to the ideal gas law
     *
     * \tparam T   The struct containing the material parameters that the equation of state should be calculated for, see the struct above
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
          return T::mu_0*((T::t_0 + T::c)/(t + T::c)*cem::pow(t/T::t_0, 3.0L/2.0L));
        }

      protected:
        IdealGas() = default;
        IdealGas(IdealGas const&) = default;
        IdealGas& operator= (IdealGas const&) = default;
        IdealGas(IdealGas&&) = default;
        IdealGas& operator= (IdealGas&&) = default;

        // TODO(tobit): Introduce remaining SI-units
        // See https://en.wikipedia.org/wiki/Gas_constant
        static constexpr long double universal_gas_constant {8.31446261815324L}; // In SI-units [J/(K mol)]
        static constexpr long double avogadro_constant {6.02214076e+23L}; // In SI-units [1/mol]
        static constexpr long double specific_gas_constant {universal_gas_constant/T::molecular_weight.get()}; // In SI-units [J/(kg K)]
    };

  }
}

#endif // LBT_MATERIAL_IDEAL_GAS
