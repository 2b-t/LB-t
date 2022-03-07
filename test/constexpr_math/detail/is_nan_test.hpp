/**
 * \file     is_inf_test.hpp
 * \mainpage Tests for functions checking whether a certain value corresponds to NaN or not
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IS_NAN_UNITTEST
#define LBT_CEM_IS_NAN_UNITTEST
#pragma once

#include <cmath>
#include <limits>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include "../../../src/constexpr_math/detail/is_nan.hpp"
#include "testing_types.hpp"


namespace lbt {
  namespace test {

    /// Test function for detecting Nan values
    template <typename T>
    struct IsNanTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(IsNanTest, FloatingPointDataTypes);

    TYPED_TEST(IsNanTest, signalingNanIsNan) {
      constexpr auto nan {std::numeric_limits<TypeParam>::signaling_NaN()};
      EXPECT_TRUE(lbt::cem::isNan(nan));
      EXPECT_TRUE(lbt::cem::isNan(nan) == std::isnan(nan));
    }

    TYPED_TEST(IsNanTest, quietNanIsNan) {
      constexpr auto nan {std::numeric_limits<TypeParam>::quiet_NaN()};
      EXPECT_TRUE(lbt::cem::isNan(nan));
      EXPECT_TRUE(lbt::cem::isNan(nan) == std::isnan(nan));
    }

    TYPED_TEST(IsNanTest, positiveNumberIsNotNan) {
      std::vector<TypeParam> const positive_numbers {+0, 1, 100, std::numeric_limits<TypeParam>::max()};
      for (auto const& n: positive_numbers) {
        EXPECT_FALSE(lbt::cem::isNan(n));
        EXPECT_TRUE(lbt::cem::isNan(n) == std::isnan(n));
      }
    }

    TYPED_TEST(IsNanTest, negativeNumberIsNotNan) {
      std::vector<TypeParam> const negative_numbers {-0, -1, -100, std::numeric_limits<TypeParam>::min()};
      for (auto const& n: negative_numbers) {
        EXPECT_FALSE(lbt::cem::isNan(n));
        EXPECT_TRUE(lbt::cem::isNan(n) == std::isnan(n));
      }
    }

  }
}

#endif // LBT_CEM_IS_NAN_UNITTEST
