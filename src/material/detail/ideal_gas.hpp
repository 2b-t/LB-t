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

    using namespace lbt::literals;

    // See https://en.wikipedia.org/wiki/Gas_constant and https://en.wikipedia.org/wiki/Kinetic_diameter
    template <typename T>
    constexpr T universal_gas_constant = static_cast<T>(8.31446261815324L); // In SI-units [J/(K mol)]
    template <typename T>
    constexpr T avogadro_constant = static_cast<T>(6.02214076e+23L); // In SI-units [1/mol]

    // Viscosity: Sutherland's law: https://de.wikipedia.org/wiki/Sutherland-Modell
    template <typename T>
    class IdealGas {
      public:
        // Calculated according to ideal gas law 
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

      protected:
        static constexpr long double specific_gas_constant = universal_gas_constant<long double>/T::molecular_weight.get(); // In SI-units [J/kg K]
    };

  }
}

#endif // LBT_MATERIAL_IDEAL_GAS
