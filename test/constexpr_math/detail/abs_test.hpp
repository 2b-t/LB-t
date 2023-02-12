/**
 * \file     abs_test.hpp
 * \mainpage Tests for constexpr absolute value function
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_ABS_UNITTEST
#define LBT_CEM_ABS_UNITTEST
#pragma once

#include <cmath>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "constexpr_math/detail/abs.hpp"
#include "testing_types.hpp"


namespace lbt {
  namespace test {

    /// Test function for calculating absolute value
    template <typename T>
    struct AbsTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(AbsTest, ArithmeticDataTypes);

    TYPED_TEST(AbsTest, negativeZeroIsPositiveZero) {
      if constexpr (std::is_floating_point_v<TypeParam>) {
        constexpr TypeParam negative_zero {-0};
        constexpr TypeParam positive_zero {+0};
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::abs(negative_zero), positive_zero);
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::abs(negative_zero), positive_zero);
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      } else {
        GTEST_SKIP() << "Test only possible for signed number types!";
      }
    }

    TYPED_TEST(AbsTest, negativeNumberIsPositiveNumber) {
      if constexpr (std::is_signed_v<TypeParam>) {
        std::vector<std::pair<TypeParam, TypeParam>> const tests {{-1, 1}, {-100, 100}};
        for (auto const& [n, p]: tests) {
          if constexpr (std::is_integral_v<TypeParam>) {
            EXPECT_EQ(lbt::cem::abs(n), p);
          } else if constexpr (std::is_same_v<TypeParam,float>) {
            EXPECT_FLOAT_EQ(lbt::cem::abs(n), p);
            EXPECT_FLOAT_EQ(lbt::cem::abs(n), std::abs(n));
          } else if constexpr (std::is_same_v<TypeParam,double>) {
            EXPECT_DOUBLE_EQ(lbt::cem::abs(n), p);
            EXPECT_DOUBLE_EQ(lbt::cem::abs(n), std::abs(n));
          } else {
            GTEST_SKIP() << "Test not implemented for given data type!";
          }
        }
      } else {
        GTEST_SKIP() << "Test only possible for signed number types!";
      }
    }

    TYPED_TEST(AbsTest, positiveNumberStaysPositiveNumber) {
      std::vector<TypeParam> const positive_numbers {1, 100};
      for (auto const& n: positive_numbers) {
        if constexpr (std::is_integral_v<TypeParam>) {
          EXPECT_EQ(lbt::cem::abs(n), n);
        } else if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::abs(n), n);
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::abs(n), n);
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

    TYPED_TEST(AbsTest, negativeNumberIsEqualToStdAbs) {
      if constexpr (std::is_signed_v<TypeParam>) {
        std::vector<TypeParam> const negative_numbers {-1, -100};
        for (auto const& n: negative_numbers) {
          if constexpr (std::is_integral_v<TypeParam>) {
            EXPECT_EQ(lbt::cem::abs(n), std::abs(n));
          } else if constexpr (std::is_same_v<TypeParam,float>) {
            EXPECT_FLOAT_EQ(lbt::cem::abs(n), std::abs(n));
          } else if constexpr (std::is_same_v<TypeParam,double>) {
            EXPECT_DOUBLE_EQ(lbt::cem::abs(n), std::abs(n));
          } else {
            GTEST_SKIP() << "Test not implemented for given data type!";
          }
        }
      } else {
        GTEST_SKIP() << "Test only possible for signed number types!";
      }
    }

    TYPED_TEST(AbsTest, positiveNumberIsEqualToStdAbs) {
      std::vector<TypeParam> const positive_numbers {1, 100};
      for (auto const& n: positive_numbers) {
        if constexpr (std::is_integral_v<TypeParam>) {
          EXPECT_EQ(lbt::cem::abs(n), n);
        } else if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::abs(n), std::abs(n));
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::abs(n), std::abs(n));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

  }
}

#endif // LBT_CEM_ABS_UNITTEST
