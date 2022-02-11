/**
 * \file     air.hpp
 * \mainpage Contains methods for calculating physical properties of air
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_AIR
#define LBT_MATERIAL_AIR
#pragma once

#include <cmath>

#include "../../constexpr_math/constexpr_math.hpp"
#include "../literals.hpp"
#include "../units.hpp"
#include "ideal_gas.hpp"
#include "physical_constants.hpp"


namespace lbt {
  namespace material {

    using namespace lbt::literals;

    class Air: public IdealGas<physical_constant::Air> {
      public:
        // https://en.wikipedia.org/wiki/Density_of_air

        // https://en.wikipedia.org/wiki/Viscosity#Air
        static constexpr lbt::unit::KinematicViscosity kinematicViscosity(lbt::unit::Temperature const t) noexcept {
          return lbt::unit::KinematicViscosity{lbt::cem::pow(t.get(), 0.7355L)*2.791e-7L};
        }
        static constexpr lbt::unit::DynamicViscosity dynamicViscosity(lbt::unit::Temperature const t, 
                                                                       lbt::unit::Pressure const p) noexcept {
          return kinematicViscosity(t)*IdealGas<physical_constant::Air>::equationOfState(t, p);
        }

        // Inherit constructor
        using IdealGas<physical_constant::Air>::IdealGas;
    };

  }
}

#endif // LBT_MATERIAL_AIR
