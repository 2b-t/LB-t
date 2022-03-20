/**
 * \file     sqrt_test.hpp
 * \mainpage Tests for constexpr square root function
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_SQRT_UNITTEST
#define LBT_CEM_SQRT_UNITTEST
#pragma once

#include <cmath>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "../../../src/constexpr_math/detail/is_inf.hpp"
#include "../../../src/constexpr_math/detail/is_nan.hpp"
#include "../../../src/constexpr_math/detail/is_nearly_equal_eps_abs.hpp"
#include "../../../src/constexpr_math/detail/sqrt.hpp"
#include "constexpr_math_utilities.hpp"
#include "testing_types.hpp"


namespace lbt {
  namespace test {

    template <typename T>
    struct SqrtTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(SqrtTest, FloatingPointDataTypes);

    TYPED_TEST(SqrtTest, positiveInfinityStaysPositiveInfinity) {
      constexpr auto inf {std::numeric_limits<TypeParam>::infinity()};
      EXPECT_TRUE(std::isinf(lbt::cem::sqrt(inf)));
    }

    TYPED_TEST(SqrtTest, positiveZeroStaysPositiveZero) {
      constexpr TypeParam positive_zero {+0.0};
      if constexpr (std::is_same_v<TypeParam,float>) {
        EXPECT_FLOAT_EQ(lbt::cem::sqrt(positive_zero), positive_zero);
      } else if constexpr (std::is_same_v<TypeParam,double>) {
        EXPECT_DOUBLE_EQ(lbt::cem::sqrt(positive_zero), positive_zero);
      } else {
        GTEST_SKIP() << "Test not implemented for given data type!";
      }
    }

    TYPED_TEST(SqrtTest, negativeZeroStaysNegativeZero) {
      constexpr TypeParam negative_zero {-0.0};
      if constexpr (std::is_same_v<TypeParam,float>) {
        EXPECT_FLOAT_EQ(lbt::cem::sqrt(negative_zero), negative_zero);
      } else if constexpr (std::is_same_v<TypeParam,double>) {
        EXPECT_DOUBLE_EQ(lbt::cem::sqrt(negative_zero), negative_zero);
      } else {
        GTEST_SKIP() << "Test not implemented for given data type!";
      }
    }

    TYPED_TEST(SqrtTest, signalingNanIsNan) {
      constexpr auto nan {std::numeric_limits<TypeParam>::signaling_NaN()};
      EXPECT_TRUE(std::isnan(lbt::cem::sqrt(nan)));
    }

    TYPED_TEST(SqrtTest, quietNanIsNan) {
      constexpr auto nan {std::numeric_limits<TypeParam>::quiet_NaN()};
      EXPECT_TRUE(std::isnan(lbt::cem::sqrt(nan)));
    }

    TYPED_TEST(SqrtTest, negativeNumberIsNan) {
      std::vector<TypeParam> const negative_numbers {-1, -10, -100};
      for (auto const& n: negative_numbers) {
        EXPECT_TRUE(std::isnan(lbt::cem::sqrt(n)));
      }
    }

    TYPED_TEST(SqrtTest, minIsNan) {
      constexpr auto min {-std::numeric_limits<TypeParam>::max()};
      EXPECT_TRUE(std::isnan(lbt::cem::sqrt(min)));
    }

    TYPED_TEST(SqrtTest, maxIsPositiveNumber) {
      constexpr auto max {std::numeric_limits<TypeParam>::max()};
      EXPECT_GT(lbt::cem::sqrt(max), 0);
    }

    TYPED_TEST(SqrtTest, unityIsUnity) {
      constexpr auto unity {static_cast<TypeParam>(1.0)};
      if constexpr (std::is_same_v<TypeParam,float>) {
        EXPECT_FLOAT_EQ(lbt::cem::sqrt(unity), unity);
      } else if constexpr (std::is_same_v<TypeParam,double>) {
        EXPECT_DOUBLE_EQ(lbt::cem::sqrt(unity), unity);
      } else {
        GTEST_SKIP() << "Test not implemented for given data type!";
      }
    }

    TYPED_TEST(SqrtTest, positiveNumbersCorrectSqrt) {
      std::vector<std::pair<TypeParam,TypeParam>> const tests = { {5.0e-13,7.0710678118654752440084436210484903928483593768847403e-7}, 
                                                                  {4.0, 2.0}, 
                                                                  {9.8596, 3.14}, 
                                                                  {100.0, 10.0}, 
                                                                  {5.0e13, 7.07106781186547524400844362104849039284835937688474036e6}
                                                                };
      for (auto const& [n, solution]: tests) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::sqrt(n), solution);
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::sqrt(n), solution);
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

    TYPED_TEST(SqrtTest, positiveNumbersEqualToStdSqrt) {
      std::vector<TypeParam> const tests = { 5.0e-13, 4.0, 9.8596, 100.0, 5.0e13 };
      for (auto const& n: tests) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::sqrt(n), std::sqrt(n));
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::sqrt(n), std::sqrt(n));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

    TYPED_TEST(SqrtTest, randomNumberSquaredCorrectSqrt) {
      constexpr TypeParam largest_number {std::numeric_limits<TypeParam>::max()};
      int exponent_max {0};
      TypeParam const mantissa_max {std::frexp(largest_number, &exponent_max)};
      exponent_max /= 2;
      
      // Generate a random number by constructing random number for exponent and mantissa seperately so numbers are not too large
      int const exponent {generateRandomNumber(0, exponent_max)};
      TypeParam const mantissa {generateRandomNumber(static_cast<TypeParam>(0), mantissa_max)};
      TypeParam const random_number {mantissa*static_cast<TypeParam>(std::pow(static_cast<TypeParam>(2), exponent))};
      
      // Square it and test if it is the same number again
      TypeParam const random_number_squared {static_cast<TypeParam>(std::pow(random_number, 2))};
      if constexpr (std::is_same_v<TypeParam,float>) {
        EXPECT_FLOAT_EQ(lbt::cem::sqrt(random_number_squared), random_number);
      } else if constexpr (std::is_same_v<TypeParam,double>) {
        EXPECT_DOUBLE_EQ(lbt::cem::sqrt(random_number_squared), random_number);
      } else {
        GTEST_SKIP() << "Test not implemented for given data type!";
      }
    }

    TYPED_TEST(SqrtTest, smallestPositiveNumberLargerSqrt) {
      auto const smallest_positive_number {std::numeric_limits<TypeParam>::min()};
      EXPECT_GT(lbt::cem::sqrt(smallest_positive_number), smallest_positive_number);
    }

    TYPED_TEST(SqrtTest, smallestNegativeNumberIsNan) {
      auto const smallest_negative_number {-std::numeric_limits<TypeParam>::min()};
      EXPECT_TRUE(std::isnan(lbt::cem::sqrt(smallest_negative_number)));
    }

  }
}

#endif // LBT_CEM_SQRT_UNITTEST
