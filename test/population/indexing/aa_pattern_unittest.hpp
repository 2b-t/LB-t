#ifndef LBT_AA_PATTERN_UNITTEST
#define LBT_AA_PATTERN_UNITTEST
#pragma once

/**
 * \file     aa_pattern_unittest.hpp
 * \mainpage Tests for indexing of population via the one-population AA-access pattern
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <limits>
#include <utility>

#include <gtest/gtest.h>

#include "../../../src/population/indexing/aa_pattern.hpp"
#include "../../lattice/lattice_testing_types.hpp"


namespace lbt {
  namespace test {
    // TODO: Check the following for all different lattices

    // TODO: Check if even and odd function is working

    // TODO: Check if index read is index write in the next step

    // TODO: Check if indexing works also over borders
  }
}

#endif // LBT_AA_PATTERN_UNITTEST
