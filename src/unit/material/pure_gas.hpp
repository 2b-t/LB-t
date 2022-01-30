#ifndef LBT_MATERIAL_PURE_GAS
#define LBT_MATERIAL_PURE_GAS
#pragma once

/**\file     pure_gas.hpp
 * \mainpage Contains methods for calculating physical properties of ideal gases
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <ostream>

#include "../../general/constexpr_math.hpp"
#include "../literals.hpp"
#include "../units.hpp"
#include "ideal_gas.hpp"
#include "physical_constants.hpp"


namespace lbt {
  namespace material {

    template <template <typename> typename T>
    class PureGas: public IdealGas<T> {
      public:
        // Calculated according to Maxwell-Boltzmann distribution https://physics.stackexchange.com/a/510860/245414
        static lbt::unit::KinematicViscosity kinematicViscosity(lbt::unit::Temperature const t, 
                                                                           lbt::unit::Pressure const p) noexcept {
          return dynamicViscosity(t)/IdealGas<T>::equationOfState(t, p);
        }
        static lbt::unit::DynamicViscosity dynamicViscosity(lbt::unit::Temperature const t) noexcept {
          // TODO: Fix implementation of isNearlyEqual for very large and very small numbers
          return lbt::unit::DynamicViscosity{5.0/16.0*std::sqrt(IdealGas<T>::specific_gas_constant*mass_single_molecule*mass_single_molecule*t.get()/lbt::cem::pi<long double>)/(kinetic_diameter*kinetic_diameter)*1.0_cP};
        }

        using IdealGas<T>::IdealGas;

        friend std::ostream& operator<< (std::ostream& os, PureGas const& pure_gas) noexcept {
          os << "Molecular mass: " << molecular_mass << std::endl;
          os << "Kinetic diameter: " << kinetic_diameter << std::endl;
          os << "Mass of a single molecule: " << mass_single_molecule << std::endl;
          os << "Specific gas constant: " << IdealGas<T>::specific_gas_constant << std::endl;
          return os;
        }

      protected:
        static constexpr long double molecular_mass = T<long double>::molecular_mass;
        static constexpr long double kinetic_diameter = T<long double>::kinetic_diameter.get();
        static constexpr long double mass_single_molecule = molecular_mass/avogadro_constant<long double>;
    };

    /// Define convenient aliases for different ideal gases
    using CarbonDioxide = PureGas<physical_constant::CarbonDioxide>;
    using Hydrogen = PureGas<physical_constant::Hydrogen>;
    using Oxygen = PureGas<physical_constant::Oxygen>;

  }
}

#endif // LBT_MATERIAL_PURE_GAS
