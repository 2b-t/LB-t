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

    using namespace lbt::literals;

    class Water {
      public:
        // According to Tait's equation: https://en.wikipedia.org/wiki/Tait_equation
        // https://physics.stackexchange.com/questions/464208/incompressible-fluid-definition/510026#510026
        static constexpr lbt::unit::Density density(lbt::unit::Temperature const t = 0.0_deg, 
                                                    lbt::unit::Pressure const p = 1.0_atm) noexcept {
          return lbt::unit::Density{0.0};
        };
        // Also modify this to equation of state

        // Potentially with look-up table and interpolation
        static constexpr lbt::unit::KinematicViscosity kinematicViscosity(lbt::unit::Temperature const t = 0.0_deg, 
                                                                           lbt::unit::Pressure const p = 1.0_atm) noexcept {
          return dynamicViscosity(t)/density(t, p);
        }
        // According to Vogel-Fulcher-Tammann equation: https://en.wikipedia.org/wiki/Viscosity#Water
        static constexpr lbt::unit::DynamicViscosity dynamicViscosity(lbt::unit::Temperature const t = 0.0_deg) noexcept {
          constexpr auto a = 0.02939_mPas;
          constexpr auto b = 507.88_K;
          constexpr auto c = 149.3_K;
        return lbt::cem::exp(b/(t-c))*a;
        }
    };

  }
}

#endif // LBT_MATERIAL_WATER
