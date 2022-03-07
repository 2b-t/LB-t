/**
 * \file     is_nearly_equal_eps_abs_test.hpp
 * \mainpage Tests for nearly equal comparison of floating-point numbers with an absolute epsilon
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IS_NEARLY_EQUAL_EPS_ABS_UNITTEST
#define LBT_CEM_IS_NEARLY_EQUAL_EPS_ABS_UNITTEST
#pragma once

#include <cmath>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "../../../src/constexpr_math/detail/abs.hpp"
#include "testing_types.hpp"


namespace lbt {
  namespace test {

    template <typename T>
    struct NearlyEqualEpsAbsTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(NearlyEqualEpsAbsTest, FloatingPointDataTypes);

    TYPED_TEST(NearlyEqualEpsAbsTest, closeNumberIsNearlyEqual) {
      std::vector<std::pair<TypeParam,TypeParam>> tests {};
      std::vector<TypeParam> const numbers {-1, 0, 1};
      for (auto const& n: numbers) {
        tests.emplace_back(n, std::nextafter(n, static_cast<TypeParam>(-1)));
        tests.emplace_back(n, std::nextafter(n, static_cast<TypeParam>(1)));
      }
      for (auto const& [a, b]: tests) {
        EXPECT_TRUE(lbt::cem::isNearlyEqualEpsAbs(a, b));
      }
    }

    TYPED_TEST(NearlyEqualEpsAbsTest, distantNumberIsNotNearlyEqual) {
      std::vector<std::pair<TypeParam,TypeParam>> tests = { {-1, 1}, {1, -1}, {3, 3.14} };
      for (auto const& [a, b]: tests) {
        EXPECT_FALSE(lbt::cem::isNearlyEqualEpsAbs(a, b));
      }
    }

  }
}

#endif // LBT_CEM_IS_NEARLY_EQUAL_EPS_ABS_UNITTEST
