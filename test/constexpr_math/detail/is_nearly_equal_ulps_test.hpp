/**
 * \file     is_nearly_equal_ulps_test.hpp
 * \mainpage Tests for constexpr nearly equal comparison functions with ULPS criterion.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IS_NEARLY_EQUAL_ULPS_UNITTEST
#define LBT_CEM_IS_NEARLY_EQUAL_ULPS_UNITTEST
#pragma once

#include <cmath>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "../../../src/constexpr_math/detail/is_nearly_equal_ulps.hpp"
#include "testing_types.hpp"


namespace lbt {
  namespace test {

    template <typename T>
    struct NearlyEqualUlpsTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(NearlyEqualUlpsTest, FloatingPointDataTypes);

    TYPED_TEST(NearlyEqualUlpsTest, closeNumberIsNearlyEqual) {
      std::vector<std::pair<TypeParam,TypeParam>> tests {};
      std::vector<TypeParam> const numbers {-1, 0, 1};
      for (auto const& n: numbers) {
        tests.emplace_back(n, std::nextafter(n, static_cast<TypeParam>(-1)));
        tests.emplace_back(n, std::nextafter(n, static_cast<TypeParam>(1)));
      }
      for (auto const& [a, b]: tests) {
        EXPECT_TRUE(lbt::cem::isNearlyEqualUlps(a, b));
      }
    }

    TYPED_TEST(NearlyEqualUlpsTest, distantNumberIsNotNearlyEqual) {
      std::vector<std::pair<TypeParam,TypeParam>> tests = { {-1, 1}, {1, -1}, {3, 3.14} };
      for (auto const& [a, b]: tests) {
        EXPECT_FALSE(lbt::cem::isNearlyEqualUlps(a, b));
      }
    }

  }
}

#endif // LBT_CEM_IS_NEARLY_EQUAL_ULPS_UNITTEST
