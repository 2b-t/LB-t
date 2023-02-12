/**
 * \file     ceil_test.hpp
 * \mainpage Tests for constexpr ceil function
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_CEIL_UNITTEST
#define LBT_CEM_CEIL_UNITTEST
#pragma once

#include <cmath>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "constexpr_math/detail/ceil.hpp"
#include "testing_types.hpp"


namespace lbt {
  namespace test {

    /// Tests for ceiling function
    template <typename T>
    struct CeilTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(CeilTest, FloatingPointDataTypes);

    TYPED_TEST(CeilTest, zeroIsZero) {
      auto const zero {static_cast<TypeParam>(0)};
      if constexpr (std::is_same_v<TypeParam,float>) {
        EXPECT_FLOAT_EQ(lbt::cem::ceil(zero), zero);
      } else if constexpr (std::is_same_v<TypeParam,double>) {
        EXPECT_DOUBLE_EQ(lbt::cem::ceil(zero), zero);
      } else {
        GTEST_SKIP() << "Test not implemented for given data type!";
      }
    }

    TYPED_TEST(CeilTest, positiveNumbersAreCorrect) {
      std::vector<std::pair<TypeParam,TypeParam>> const tests = { {1.1, 2}, {1.5, 2}, {1.9, 2} };
      for (auto const& [n, solution]: tests) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::ceil(n), solution);
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::ceil(n), solution);
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

    TYPED_TEST(CeilTest, negativeNumbersAreCorrect) {
      std::vector<std::pair<TypeParam,TypeParam>> const tests = { {-1.1, -1}, {-1.5, -1}, {-1.9, -1} };
      for (auto const& [n, solution]: tests) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::ceil(n), solution);
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::ceil(n), solution);
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

    TYPED_TEST(CeilTest, positiveNumbersAreEqualToStdSqrt) {
      std::vector<TypeParam> const positive_numbers = { 1.1, 1.5, 1.9 };
      for (auto const& n: positive_numbers) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::ceil(n), std::ceil(n));
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::ceil(n), std::ceil(n));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

    TYPED_TEST(CeilTest, negativeNumbersAreEqualToStdSqrt) {
      std::vector<TypeParam> const negative_numbers = { -1.1, -1.5, -1.9 };
      for (auto const& n: negative_numbers) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::ceil(n), std::ceil(n));
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::ceil(n), std::ceil(n));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

  }
}

#endif // LBT_CEM_CEIL_UNITTEST
