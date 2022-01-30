#ifndef LBT_MATERIAL_AIR
#define LBT_MATERIAL_AIR
#pragma once

/**\file     air.hpp
 * \mainpage Contains methods for calculating physical properties of air
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <cmath>

#include "../../general/constexpr_math.hpp"
#include "../literals.hpp"
#include "../units.hpp"
#include "ideal_gas.hpp"


namespace lbt {
  namespace material {

    using namespace lbt::literals;

    class Air {
      public:
        // https://en.wikipedia.org/wiki/Density_of_air
        static constexpr lbt::unit::Density density(lbt::unit::Temperature const t = 0.0_deg, 
                                                    lbt::unit::Pressure const p = 1.0_atm) noexcept {
          return ideal_gas::Air::density(t, p);
        };
        static constexpr lbt::unit::KinematicViscosity kinematic_viscosity(lbt::unit::Temperature const t = 0.0_deg, 
                                                                           lbt::unit::Pressure const p = 1.0_atm) noexcept {
          return lbt::unit::KinematicViscosity{lbt::cem::pow(t.get(), 0.7355L)*2.791e-7L};
        }
        // https://en.wikipedia.org/wiki/Viscosity#Air
        static constexpr lbt::unit::DynamicViscosity dynamic_viscosity(lbt::unit::Temperature const t = 0.0_deg, 
                                                                       lbt::unit::Pressure const p = 1.0_atm) noexcept {
          return kinematic_viscosity(t, p)*density(t, p);
        }
    };

  }
}

#endif // LBT_MATERIAL_AIR
