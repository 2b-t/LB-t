#ifndef LBT_MATERIAL_AIR
#define LBT_MATERIAL_AIR
#pragma once

/**\file     air.hpp
 * \mainpage Contains methods for calulcating physical properties of air
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include "../literals.hpp"
#include "../units.hpp"


namespace lbt {
  namespace material {

    using namespace lbt::literals;

    class Air {
      public:
        // https://en.wikipedia.org/wiki/Density_of_air
        static constexpr lbt::unit::Density density(lbt::unit::Temperature const t = 0.0_deg, 
                                                    lbt::unit::Pressure const p = 1.0_atm) noexcept {
          return lbt::unit::Density{0.0};
        };
        // Potentially with look-up table and interpolation
        static constexpr lbt::unit::KinematicViscosity kinematic_viscosity(lbt::unit::Temperature const t = 0.0_deg, 
                                                                           lbt::unit::Pressure const p = 1.0_atm) noexcept {
          return lbt::unit::KinematicViscosity{0.0};
        }
        // TODO: Add dynamic_viscosity formula
    };

  }
}

#endif // LBT_MATERIAL_AIR
