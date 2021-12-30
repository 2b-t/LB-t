#ifndef LBT_INDEXING_UNITTEST
#define LBT_INDEXING_UNITTEST
#pragma once

/**
 * \file     indexing_unittest.hpp
 * \mainpage Tests for indexing of population
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <utility>

#include <gtest/gtest.h>

#include "../../../src/population/indexing/indexing.hpp"
#include "../../lattice/lattice_testing_types.hpp"


namespace lbt {
  namespace test {

    // Test fixture for typed indexing tests
    template <typename L>
    class IndexingTest : public ::testing::Test {
    };

    TYPED_TEST_SUITE(IndexingTest, LatticeTestTypes);

    TYPED_TEST(IndexingTest, zerosResultInZero) {
      // Domain resolution
      constexpr std::int32_t NX {7};
      constexpr std::int32_t NY {11};
      constexpr std::int32_t NZ {21};
      lbt::Indexing<TypeParam,1> const indexing {NX, NY, NZ};
      // First index
      constexpr std::int32_t x {0};
      constexpr std::int32_t y {0};
      constexpr std::int32_t z {0};
      constexpr std::int32_t n {0};
      constexpr std::int32_t d {0};
      constexpr std::int32_t p {0};
      constexpr std::int64_t expected_index {0};
      std::int64_t const index {indexing.spatialToLinear(x,y,z,n,d,p)};
      EXPECT_EQ(index, expected_index);
    }

    // Check that each line is skipped correctly
    // Test for all dimensions separately

    TYPED_TEST(IndexingTest, upcastToInt64) {
      /*constexpr std::int32_t NX;
      constexpr std::int32_t NY;
      constexpr std::int32_t NZ;

      constexpr std::int32_t x;
      constexpr std::int32_t y;
      constexpr std::int32_t z;
      constexpr std::int32_t n;
      constexpr std::int32_t d;
      constexpr std::int32_t p;*/
      // One max and the others reasonable
      EXPECT_TRUE(true);
    }

    // Test tranlating back to linear indices

  }
}

#endif // LBT_INDEXING_UNITTEST
