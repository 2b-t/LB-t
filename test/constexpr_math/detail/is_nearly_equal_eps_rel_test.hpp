/**
 * \file     is_nearly_equal_eps_rel_test.hpp
 * \mainpage Tests for nearly equal comparison of floating-point numbers with a relative epsilon
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IS_NEARLY_EQUAL_EPS_REL_UNITTEST
#define LBT_CEM_IS_NEARLY_EQUAL_EPS_REL_UNITTEST
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
    struct NearlyEqualEpsRelTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(NearlyEqualEpsRelTest, FloatingPointDataTypes);

    TYPED_TEST(NearlyEqualEpsRelTest, closeNumberIsNearlyEqual) {
      std::vector<std::pair<TypeParam,TypeParam>> tests {};
      std::vector<TypeParam> const numbers {-1, 0, 1};
      for (auto const& n: numbers) {
        tests.emplace_back(n, std::nextafter(n, static_cast<TypeParam>(-1)));
        tests.emplace_back(n, std::nextafter(n, static_cast<TypeParam>(1)));
      }
      for (auto const& [a, b]: tests) {
        EXPECT_TRUE(lbt::cem::isNearlyEqualEpsRel(a, b));
      }
    }

    TYPED_TEST(NearlyEqualEpsRelTest, distantNumberIsNotNearlyEqual) {
      std::vector<std::pair<TypeParam,TypeParam>> tests = { {-1, 1}, {1, -1}, {3, 3.14} };
      for (auto const& [a, b]: tests) {
        EXPECT_FALSE(lbt::cem::isNearlyEqualEpsRel(a, b));
      }
    }

  }
}

#endif // LBT_CEM_IS_NEARLY_EQUAL_EPS_REL_UNITTEST
