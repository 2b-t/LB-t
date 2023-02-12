/**
 * \file     lattice_test.hpp
 * \mainpage Tests for different discretisation lattices
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <type_traits>

#include <gtest/gtest.h>

#include "constexpr_math/constexpr_math.hpp"
#include "../testing_utilities/array_utilities.hpp"
#include "lattice_testing_types.hpp"


namespace lbt {
  namespace test {

    /// Templated tests for each lattice
    template <typename L>
    struct DdQqTest: public ::testing::Test {
    };
    TYPED_TEST_SUITE(DdQqTest, LatticeTestTypes);

    /// Test that class contains all relevant class members
    TYPED_TEST(DdQqTest, containsAllVariables) {
      using T = typename TypeParam::type;
      EXPECT_TRUE(std::is_floating_point_v<T>);

      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::DIM)>);
      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::SPEEDS)>);
      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::HSPEED)>);
      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::PAD)>);
      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::ND)>);
      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::OFF)>);

      using A = typename lbt::StackArray<T, TypeParam::ND>;
      constexpr bool has_dx {std::is_same_v<std::decay_t<decltype(TypeParam::DX)>, A>};
      EXPECT_TRUE(has_dx);
      constexpr bool has_dy {std::is_same_v<std::decay_t<decltype(TypeParam::DY)>, A>};
      EXPECT_TRUE(has_dy);
      constexpr bool has_dz {std::is_same_v<std::decay_t<decltype(TypeParam::DZ)>, A>};
      EXPECT_TRUE(has_dz);
      constexpr bool has_w {std::is_same_v<std::decay_t<decltype(TypeParam::W)>, A>};
      EXPECT_TRUE(has_w);
      constexpr bool has_mask {std::is_same_v<std::decay_t<decltype(TypeParam::MASK)>, A>};
      EXPECT_TRUE(has_mask);

      constexpr bool is_same_cs {std::is_same_v<std::decay_t<decltype(TypeParam::CS)>, T>};
      EXPECT_TRUE(is_same_cs);
    }

    /// Tests for discretisation dimensions
    TYPED_TEST(DdQqTest, discretisationDimensions) {
      EXPECT_GT(TypeParam::DIM, 0);
      EXPECT_LE(TypeParam::DIM, 3);
    }

    /// Tests correlation between array size parameters
    TYPED_TEST(DdQqTest, speedsCorrelations) {
      EXPECT_GE(TypeParam::SPEEDS, 2*TypeParam::DIM);
      EXPECT_GT(TypeParam::HSPEED, TypeParam::DIM);
      EXPECT_GE(TypeParam::PAD, 0);
      EXPECT_GT(TypeParam::ND, TypeParam::SPEEDS);

      EXPECT_EQ(TypeParam::HSPEED, (TypeParam::SPEEDS + 1)/2);
      EXPECT_GT(TypeParam::ND, TypeParam::SPEEDS);
      EXPECT_EQ(TypeParam::ND, TypeParam::SPEEDS + TypeParam::PAD);
      EXPECT_EQ(TypeParam::OFF, TypeParam::ND/2);
    }

    /// Test that first entry is the same and other are mirrored
    TYPED_TEST(DdQqTest, speedsSymmetries) {
      bool is_correct {true};
      is_correct &= isAntimetric(TypeParam::DX);
      is_correct &= isAntimetric(TypeParam::DY);
      is_correct &= isAntimetric(TypeParam::DZ);
      is_correct &= isSymmetric(TypeParam::W);
      EXPECT_TRUE(is_correct);
    }

    /// Array sum
    TYPED_TEST(DdQqTest, arraySum) {
      bool is_correct {true};
      is_correct &= sumsTo(TypeParam::DX, static_cast<typename TypeParam::type>(0));
      is_correct &= sumsTo(TypeParam::DY, static_cast<typename TypeParam::type>(0));
      is_correct &= sumsTo(TypeParam::DZ, static_cast<typename TypeParam::type>(0));
      is_correct &= sumsTo(TypeParam::W,  static_cast<typename TypeParam::type>(1) + TypeParam::W.at(0));

      EXPECT_TRUE(is_correct);
    }

    /// Test that mask is correct
    TYPED_TEST(DdQqTest, logicalMask) {
      bool is_success {true};
      constexpr std::int32_t hspeed {TypeParam::ND/2};
      for (std::int32_t i = 0; i <= 1; ++i) {
        for (std::int32_t j = 0; j < TypeParam::HSPEED; ++j) {
          std::int32_t const index {i*TypeParam::OFF + j};
          if (index != hspeed) {
            is_success &= lbt::cem::isAlmostEqualEpsAbs(TypeParam::MASK.at(index), static_cast<typename TypeParam::type>(1));
          } else {
            is_success &= lbt::cem::isAlmostEqualEpsAbs(TypeParam::MASK.at(index), static_cast<typename TypeParam::type>(0));
          }
        }
      }

      EXPECT_TRUE(is_success);
    }

    /// Test that speed of sound positive
    TYPED_TEST(DdQqTest, speedOfSound) {
      EXPECT_GT(TypeParam::CS, 0);
      EXPECT_LE(TypeParam::CS, 1);
    }

    /// Test alignment of the different arrays
    TYPED_TEST(DdQqTest, cacheLineAlignment) {
      EXPECT_TRUE(isAligned(&TypeParam::DX.at(0), lbt::alignment));
      EXPECT_TRUE(isAligned(&TypeParam::DY.at(0), lbt::alignment));
      EXPECT_TRUE(isAligned(&TypeParam::DZ.at(0), lbt::alignment));
      EXPECT_TRUE(isAligned(&TypeParam::W.at(0), lbt::alignment));
      EXPECT_TRUE(isAligned(&TypeParam::MASK.at(0), lbt::alignment));
    }

  }
}
