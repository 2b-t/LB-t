/**
 * \file     hydrogen.hpp
 * \mainpage Contains methods for calculating physical properties of hydrogen
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_HYDROGEN
#define LBT_MATERIAL_HYDROGEN
#pragma once

#include "../../unit/literals.hpp"
#include "../../unit/units.hpp"
#include "ideal_gas.hpp"


namespace lbt {
  namespace material {

    namespace physical_constants {
      using namespace lbt::literals;

      class Hydrogen {
        public:
          static constexpr auto molecular_weight = 2.016_gpmol;
          static constexpr auto c = 72.0_K;
          static constexpr auto t_0 = 293.85_K;
          static constexpr auto mu_0 = 8.76_uPas;
      };

    }

    using Hydrogen = IdealGas<physical_constants::Hydrogen>;

  }
}

#endif // LBT_MATERIAL_HYDROGEN
