#ifndef LBT_AA_POPULATION_UNITTEST
#define LBT_AA_POPULATION_UNITTEST
#pragma once

/**
 * \file     aa_population_unittest.hpp
 * \mainpage Tests for a population with A-A access pattern
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <limits>
#include <utility>

#include <gtest/gtest.h>

#include "../../src/population/aa_population.hpp"
#include "../lattice/lattice_testing_types.hpp"


namespace lbt {
  namespace test {
    // TODO: Check for even and odd time steps

    // TODO: Check if index read is index write in the next step: Write values

    // TODO: Check if indexing works also over borders: Write values
  }
}

#endif // LBT_AA_POPULATION_UNITTEST
