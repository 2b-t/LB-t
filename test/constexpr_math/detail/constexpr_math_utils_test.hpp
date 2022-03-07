/**
 * \file     constexpr_math_utils_test.hpp
 * \mainpage Tests for testing utilities for the constexpr math library
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_UTILS_UNITTEST
#define LBT_CEM_UTILS_UNITTEST
#pragma once

#include <limits>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "constexpr_math_utils.hpp"
#include "testing_types.hpp"


namespace lbt {
  namespace test {

    /// Test random number generator for floating point data types
    template <typename T>
    struct RandomFloatingPointNumberTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(RandomFloatingPointNumberTest, FloatingPointDataTypes);

    TYPED_TEST(RandomFloatingPointNumberTest, ResultInBetweenSpecifiedLimits) {
      auto constexpr min {std::numeric_limits<TypeParam>::lowest()};
      auto constexpr max {std::numeric_limits<TypeParam>::max()};
      std::vector<std::pair<TypeParam, TypeParam>> const tests {{min/2, max/2}, {min, 0}, {0, max}, {0, 0}, {0.1, 0.9}, {1, 2}};
      for (auto const& [lower, upper]: tests) {
        auto const random_number {generateRandomNumber(lower, upper)};
        EXPECT_GE(random_number, lower);
        EXPECT_LE(random_number, upper);
      }
    }

    TYPED_TEST(RandomFloatingPointNumberTest, invertLimits) {
      auto constexpr min {std::numeric_limits<TypeParam>::lowest()};
      auto constexpr max {std::numeric_limits<TypeParam>::max()};
      std::vector<std::pair<TypeParam, TypeParam>> const tests {{min/2, max/2}, {min, 0}, {0, max}, {0, 0}, {0.1, 0.9}, {1, 2}};
      for (auto const& [lower, upper]: tests) {
        auto const random_number {generateRandomNumber(upper, lower)};
        EXPECT_GE(random_number, lower);
        EXPECT_LE(random_number, upper);
      }
    }


    /// Test random number generator for integer data types
    template <typename T>
    struct RandomIntegerNumberTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(RandomIntegerNumberTest, IntegerDataTypes);

    TYPED_TEST(RandomIntegerNumberTest, resultInBetweenSpecifiedLimits) {
      auto constexpr min {std::numeric_limits<TypeParam>::min()};
      auto constexpr max {std::numeric_limits<TypeParam>::max()};
      std::vector<std::pair<TypeParam, TypeParam>> const tests {{min, max}, {min/2, max/2}, {min, 0}, {0, max}, {0, 0}, {1, 2}};
      for (auto const& [lower, upper]: tests) {
        EXPECT_NO_THROW({
          auto const random_number {generateRandomNumber(lower, upper)};
          EXPECT_GE(random_number, lower);
          EXPECT_LE(random_number, upper);
        }
        );
      }
    }

    TYPED_TEST(RandomIntegerNumberTest, invertLimits) {
      auto constexpr min {std::numeric_limits<TypeParam>::min()};
      auto constexpr max {std::numeric_limits<TypeParam>::max()};
      std::vector<std::pair<TypeParam, TypeParam>> const tests {{min, max}, {min/2, max/2}, {0, max}, {1, 2}};
      for (auto const& [lower, upper]: tests) {
        EXPECT_THROW(generateRandomNumber(upper, lower), std::invalid_argument);
      }
    }

  }
}

#endif // LBT_CEM_UTILS_UNITTEST
