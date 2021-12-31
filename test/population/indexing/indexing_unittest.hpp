#ifndef LBT_INDEXING_UNITTEST
#define LBT_INDEXING_UNITTEST
#pragma once

/**
 * \file     indexing_unittest.hpp
 * \mainpage Tests for indexing of population
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <limits>
#include <utility>

#include <gtest/gtest.h>

#include "../../../src/population/indexing/indexing.hpp"
#include "../../lattice/lattice_testing_types.hpp"


namespace lbt {
  namespace test {

    // Test fixture for typed indexing tests
    template <typename L>
    class IndexingLimitsTest : public ::testing::Test {
    };

    TYPED_TEST_SUITE(IndexingLimitsTest, LatticeTestTypes);

    TYPED_TEST(IndexingLimitsTest, upcastToInt64) {
      // Make sure that final index succeeds the numeric limit of the int32_t
      constexpr std::int32_t NX {1};
      constexpr std::int32_t NY {std::numeric_limits<std::int32_t>::max()};
      constexpr std::int32_t NZ {0};
      constexpr std::int32_t NP {1};
      lbt::Indexing<TypeParam,1> const indexing {NX, NY, NZ};
      constexpr std::int32_t x {0};
      constexpr std::int32_t y {0};
      constexpr std::int32_t z {1};
      constexpr std::int32_t n {0};
      constexpr std::int32_t d {0};
      constexpr std::int32_t p {0};
      // Calculate expected index analytically
      constexpr std::int64_t expected_index {static_cast<std::int64_t>(NY)*TypeParam::ND};
      std::int64_t const index {indexing.spatialToLinear(x,y,z,n,d,p)};
      EXPECT_EQ(index, expected_index);
    }

    template <typename L>
    class IndexingTest : public ::testing::Test {
      public:
        // Initialise with arbitrary resolution
        constexpr IndexingTest(std::int32_t const NX = 9, std::int32_t const NY = 11, std::int32_t const NZ = 21) noexcept
          : NX{NX}, NY{NY}, NZ{NZ}, indexing{NX,NY,NZ} {
          return;
        }
      protected:
        std::int32_t NX;
        std::int32_t NY;
        std::int32_t NZ;
        static constexpr std::int32_t NP {2};
        lbt::Indexing<L,NP> indexing;
    };

    TYPED_TEST_SUITE(IndexingTest, LatticeTestTypes);

    TYPED_TEST(IndexingTest, zerosResultInZero) {
      // Calculate first linear index
      constexpr std::int32_t x {0};
      constexpr std::int32_t y {0};
      constexpr std::int32_t z {0};
      constexpr std::int32_t n {0};
      constexpr std::int32_t d {0};
      constexpr std::int32_t p {0};
      constexpr std::int64_t expected_index {0};
      std::int64_t const index {this->indexing.spatialToLinear(x,y,z,n,d,p)};
      EXPECT_EQ(index, expected_index);
    }

    TYPED_TEST(IndexingTest, dJumpsByD) {
      constexpr std::int32_t step_d {TypeParam::HSPEED-1};
      constexpr std::int32_t x {5};
      constexpr std::int32_t y {2};
      constexpr std::int32_t z {3};
      constexpr std::int32_t n {1};
      std::int32_t d {0};
      constexpr std::int32_t p {0};
      std::int64_t const index {this->indexing.spatialToLinear(x,y,z,n,d,p)};
      d += step_d;
      std::int64_t const difference = this->indexing.spatialToLinear(x,y,z,n,d,p) - index;
      constexpr std::int64_t expected_difference {static_cast<std::int64_t>(step_d)};
      EXPECT_EQ(difference, expected_difference);
    }

    TYPED_TEST(IndexingTest, nJumpsByOff) {
      constexpr std::int32_t step_n {1};
      constexpr std::int32_t x {5};
      constexpr std::int32_t y {2};
      constexpr std::int32_t z {3};
      std::int32_t n {0};
      constexpr std::int32_t d {TypeParam::HSPEED/2};
      constexpr std::int32_t p {0};
      std::int64_t const index {this->indexing.spatialToLinear(x,y,z,n,d,p)};
      n += step_n;
      std::int64_t const difference = this->indexing.spatialToLinear(x,y,z,n,d,p) - index;
      constexpr std::int64_t expected_difference {static_cast<std::int64_t>(TypeParam::OFF)};
      EXPECT_EQ(difference, expected_difference);
    }

    TYPED_TEST(IndexingTest, pJumpsByNd) {
      constexpr std::int32_t step_p {1};
      constexpr std::int32_t x {5};
      constexpr std::int32_t y {2};
      constexpr std::int32_t z {3};
      constexpr std::int32_t n {1};
      constexpr std::int32_t d {TypeParam::HSPEED/2};
      std::int32_t p {0};
      std::int64_t const index {this->indexing.spatialToLinear(x,y,z,n,d,p)};
      p += step_p;
      std::int64_t const difference = this->indexing.spatialToLinear(x,y,z,n,d,p) - index;
      constexpr std::int64_t expected_difference {static_cast<std::int64_t>(TypeParam::ND)};
      EXPECT_EQ(difference, expected_difference);
    }

    TYPED_TEST(IndexingTest, xJumpsByNpNd) {
      constexpr std::int32_t step_x {1};
      std::int32_t x {5};
      constexpr std::int32_t y {2};
      constexpr std::int32_t z {3};
      constexpr std::int32_t n {1};
      constexpr std::int32_t d {TypeParam::HSPEED/2};
      constexpr std::int32_t p {0};
      std::int64_t const index {this->indexing.spatialToLinear(x,y,z,n,d,p)};
      x += step_x;
      std::int64_t const difference = this->indexing.spatialToLinear(x,y,z,n,d,p) - index;
      std::int64_t const expected_difference {static_cast<std::int64_t>(this->NP)*TypeParam::ND};
      EXPECT_EQ(difference, expected_difference);
    }

    TYPED_TEST(IndexingTest, yJumpsByNxNpNd) {
      constexpr std::int32_t step_y {1};
      constexpr std::int32_t x {5};
      std::int32_t y {2};
      constexpr std::int32_t z {3};
      constexpr std::int32_t n {1};
      constexpr std::int32_t d {TypeParam::HSPEED/2};
      constexpr std::int32_t p {0};
      std::int64_t const index {this->indexing.spatialToLinear(x,y,z,n,d,p)};
      y += step_y;
      std::int64_t const difference = this->indexing.spatialToLinear(x,y,z,n,d,p) - index;
      std::int64_t const expected_difference {static_cast<std::int64_t>(this->NX)*this->NP*TypeParam::ND};
      EXPECT_EQ(difference, expected_difference);
    }

    TYPED_TEST(IndexingTest, zJumpsByNyNxNpNd) {
      constexpr std::int32_t step_z {1};
      constexpr std::int32_t x {5};
      constexpr std::int32_t y {2};
      std::int32_t z {3};
      constexpr std::int32_t n {1};
      constexpr std::int32_t d {TypeParam::HSPEED/2};
      constexpr std::int32_t p {0};
      std::int64_t const index {this->indexing.spatialToLinear(x,y,z,n,d,p)};
      z += step_z;
      std::int64_t const difference = this->indexing.spatialToLinear(x,y,z,n,d,p) - index;
      std::int64_t const expected_difference {static_cast<std::int64_t>(this->NY)*this->NX*this->NP*TypeParam::ND};
      EXPECT_EQ(difference, expected_difference);
    }

    TYPED_TEST(IndexingTest, linearToSpatial) {
      // Max sure that the conversion from linear to spatial and back works
      constexpr std::int32_t expected_x {5};
      constexpr std::int32_t expected_y {2};
      constexpr std::int32_t expected_z {3};
      constexpr std::int32_t expected_n {1};
      constexpr std::int32_t expected_d {TypeParam::HSPEED/2};
      constexpr std::int32_t expected_p {0};
      std::int64_t const index {this->indexing.spatialToLinear(expected_x,expected_y,expected_z,expected_n,expected_d,expected_p)};
      auto const [x,y,z,n,d,p] = this->indexing.linearToSpatial(index);
      EXPECT_EQ(x, expected_x);
      EXPECT_EQ(y, expected_y);
      EXPECT_EQ(z, expected_z);
      EXPECT_EQ(n, expected_n);
      EXPECT_EQ(d, expected_d);
      EXPECT_EQ(p, expected_p);
    }

  }
}

#endif // LBT_INDEXING_UNITTEST
