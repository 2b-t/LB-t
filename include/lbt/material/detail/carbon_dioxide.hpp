/**
 * \file     carbon_dioxide.hpp
 * \mainpage Contains methods for calculating physical properties of carbon dioxide
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_CARBON_DIOXIDE
#define LBT_MATERIAL_CARBON_DIOXIDE
#pragma once

#include "lbt/material/detail/ideal_gas.hpp"
#include "lbt/unit/literals.hpp"
#include "lbt/unit/units.hpp"


namespace lbt {
  namespace material {

    namespace physical_constants {
      using namespace lbt::literals;

      class CarbonDioxide {
        public:
          static constexpr auto molecular_weight = 44.01_gpmol;
          static constexpr auto c = 240.0_K;
          static constexpr auto t_0 = 293.15_K;
          static constexpr auto mu_0 = 14.8_uPas;

        protected:
          CarbonDioxide() = default;
          CarbonDioxide(CarbonDioxide const&) = default;
          CarbonDioxide& operator= (CarbonDioxide const&) = default;
          CarbonDioxide(CarbonDioxide&&) = default;
          CarbonDioxide& operator= (CarbonDioxide&&) = default;
      };

    }

    using CarbonDioxide = IdealGas<physical_constants::CarbonDioxide>;

  }
}

#endif // LBT_MATERIAL_CARBON_DIOXIDE
