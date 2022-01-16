#ifndef LBT_POPULATION_BASE_UNITTEST
#define LBT_POPULATION_BASE_UNITTEST
#pragma once

/**
 * \file     population_base_unittest.hpp
 * \mainpage Tests for a population with arbitrary access patterns and underlying memory layout
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <limits>
#include <utility>

#include <gtest/gtest.h>

#include "../lattice/lattice_testing_types.hpp"


namespace lbt {
  namespace test {
    // TODO: Check if index read is index write in the next step for entire domain: Write values
    // Check for even and odd time steps
    // Two distinct loops so can see that correct and not overwritten by other values

    // TODO: If encapsulated array has size() function: Check if size has been initialised correctly
  }
}

#endif // LBT_POPULATION_BASE_UNITTEST
