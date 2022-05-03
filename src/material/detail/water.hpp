/**
 * \file     water.hpp
 * \mainpage Contains methods for calculating physical properties of water
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_WATER
#define LBT_MATERIAL_WATER
#pragma once

#include "../../constexpr_math/constexpr_math.hpp"
#include "../../unit/literals.hpp"
#include "../../unit/units.hpp"


namespace lbt {
  namespace material {

    /**\class  Water
     * \brief  Class for calculating the equation of state according to the Tait equation
    */
    class Water {
      public:
        // According to Tait's equation: https://de.wikipedia.org/wiki/Taitsche_Gleichung
        static constexpr lbt::unit::Density equationOfState(lbt::unit::Pressure const p) noexcept {
          using namespace lbt::literals;
          constexpr auto b {0.3214_GPa};
          constexpr long double m {7};
          constexpr auto rho_0 {997.0_kg/1.0_m3};
          constexpr auto p_0 {1.0_bar};
          return rho_0*cem::pow((p - p_0)/b + 1.0L, 1.0L/m);
        };
        static constexpr lbt::unit::Pressure equationOfState(lbt::unit::Density const rho) noexcept {
          using namespace lbt::literals;
          constexpr auto b {0.3214_GPa};
          constexpr long double m {7};
          constexpr auto rho_0 {997.0_kg/1.0_m3};
          constexpr auto p_0 {1.0_bar};
          return p_0 + (cem::pow(rho/rho_0, m) - 1.0L)*b;
        };

        /**\fn    kinematicViscosity
         * \brief Calculate the kinematic viscosity with Vogel-Fulcher-Tammann equation (https://en.wikipedia.org/wiki/Viscosity#Water)
        */
        static constexpr lbt::unit::KinematicViscosity kinematicViscosity(lbt::unit::Density const rho, 
                                                                          lbt::unit::Temperature const t) noexcept {
          return dynamicViscosity(t)/rho;
        }

        /**\fn    dynamicViscosity
         * \brief Calculate the dynamic viscosity with Vogel-Fulcher-Tammann equation (https://en.wikipedia.org/wiki/Viscosity#Water)
        */
        static constexpr lbt::unit::DynamicViscosity dynamicViscosity(lbt::unit::Temperature const t) noexcept {
          using namespace lbt::literals;
          constexpr auto a {0.02939_mPas};
          constexpr auto b {507.88_K};
          constexpr auto c {149.3_K};
          return lbt::cem::exp(b/(t-c))*a;
        }
    };

  }
}

#endif // LBT_MATERIAL_WATER
