/**
 * \file     air.hpp
 * \mainpage Contains methods for calculating physical properties of air
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_AIR
#define LBT_MATERIAL_AIR
#pragma once

#include "../../constexpr_math/constexpr_math.hpp"
#include "../../unit/literals.hpp"
#include "../../unit/units.hpp"
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
    
    // Use table like (e.g. https://www.engineeringtoolbox.com/air-temperature-pressure-density-d_771.html)
    // template <>
    // constexpr States<4> StatesTable<Material::Air,4>::states_ = { State{ 1.0, 1.0f, 1.0L},
    //                                                               State{-1.0, 1.0f, 1.0L},
    //                                                               State{-1.0,-1.0f, 2.0L},
    //                                                               State{ 1.0,-1.0f, 2.0L} };
    // using Air = StatesTable<Material::Air,4>;

  }
}

#endif // LBT_MATERIAL_AIR
