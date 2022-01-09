#ifndef LBT_POPULATION
#define LBT_POPULATION
#pragma once

/**
 * \file     population.hpp
 * \mainpage Class for microscopic populations
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>

#include "aa_population.hpp"


namespace lbt {
  // Convenient alias for population based on A-A access pattern
  template <class LT, std::int32_t NPOP>
  using Population = AaPopulation<LT, NPOP>;
}

#endif // LBT_POPULATION
