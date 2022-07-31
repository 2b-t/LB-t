/**
 * \file     population.hpp
 * \mainpage Class for microscopic populations
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_POPULATION
#define LBT_POPULATION
#pragma once

#include <cstdint>

#include "aa_population.hpp"


namespace lbt {
  // Convenient alias for population based on A-A access pattern
  template <class LT, std::int32_t NP>
  using Population = AaPopulation<LT, NP>;
}

#endif // LBT_POPULATION
