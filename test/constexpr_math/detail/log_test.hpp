/**
 * \file     log_test.hpp
 * \mainpage Tests for constexpr natural logarithm function
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_LOG_UNITTEST
#define LBT_CEM_LOG_UNITTEST
#pragma once

#include <cmath>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "lbt/constexpr_math/detail/is_inf.hpp"
#include "lbt/constexpr_math/detail/is_nan.hpp"
#include "lbt/constexpr_math/detail/is_almost_equal_eps_abs.hpp"
#include "lbt/constexpr_math/detail/log.hpp"
#include "lbt/constexpr_math/detail/mathematical_constants.hpp"
#include "testing_types.hpp"


namespace lbt {
  namespace test {

    template <typename T>
    struct LogTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(LogTest, FloatingPointDataTypes);

    TYPED_TEST(LogTest, zeroIsNegativeInfinity) {
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      EXPECT_TRUE(lbt::cem::isNegInf(lbt::cem::log(zero)));
    }

    TYPED_TEST(LogTest, unityIsZero) {
      constexpr auto unity {static_cast<TypeParam>(1.0)};
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      EXPECT_TRUE(lbt::cem::isAlmostEqualEpsAbs(lbt::cem::log(unity), zero));
    }

    TYPED_TEST(LogTest, smallerThanZeroIsNan) {
      std::vector<TypeParam> const negative_numbers = { -1.1, -1.5, -1.9 };
      for (auto const& n: negative_numbers) {
        EXPECT_TRUE(lbt::cem::isNan(lbt::cem::log(n)));
      }
    }

    TYPED_TEST(LogTest, negativeInfinityIsNan) {
      constexpr auto inf {-std::numeric_limits<TypeParam>::infinity()};
      EXPECT_TRUE(lbt::cem::isNan(lbt::cem::log(inf)));
    }

    TYPED_TEST(LogTest, positiveInfinityIsInfinity) {
      constexpr auto inf {std::numeric_limits<TypeParam>::infinity()};
      EXPECT_TRUE(lbt::cem::isPosInf(lbt::cem::log(inf)));
    }

    TYPED_TEST(LogTest, nanIsNan) {
      constexpr auto nan {std::numeric_limits<TypeParam>::signaling_NaN()};
      EXPECT_TRUE(lbt::cem::isNan(lbt::cem::log(nan)));
    }

    TYPED_TEST(LogTest, eulersNumberIsUnity) {
      constexpr auto eulers_number {lbt::cem::e<TypeParam>};
      constexpr auto unity {static_cast<TypeParam>(1.0)};
      EXPECT_TRUE(lbt::cem::isAlmostEqualEpsAbs(lbt::cem::log(eulers_number), unity));
    }

    TYPED_TEST(LogTest, positiveNumbersAreCorrect) {
      std::vector<std::pair<TypeParam,TypeParam>> const tests = { {  0.2, static_cast<TypeParam>(-1.609437912434100374600759333226187639525601354268517721912)},
                                                                  {  1.5, static_cast<TypeParam>(0.4054651081081643819780131154643491365719904234624941976140)},
                                                                  { 21.0, static_cast<TypeParam>(3.0445224377234229965005979803657054342845752874046106401940844835)},
                                                                  {325.4, static_cast<TypeParam>(5.7850551947849374239386534894581251752940419349976263065587)}
                                                                };
      for (auto const& [n, solution]: tests) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::log(n), solution);
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::log(n), solution);
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

    TYPED_TEST(LogTest, positiveNumbersEqualToStdLog) {
      std::vector<TypeParam> const tests = { 0.2, 1.5, 21.0, 325.4 };
      for (auto const& n: tests) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::log(n), std::log(n));
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::log(n), std::log(n));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

  }
}

#endif // LBT_CEM_LOG_UNITTEST
