#ifndef LBT_CONSTEXPR_MATH_UNITTEST
#define LBT_CONSTEXPR_MATH_UNITTEST
#pragma once

/**
 * \file     constexpr_math_unittest.hpp
 * \mainpage Tests for constexpr math functions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cmath>
#include <limits>

#include <gtest/gtest.h>

#include "../../src/general/constexpr_math.hpp"
 

/// Tests for absolute value
TEST(AbsTest, DoubleZero) {
  EXPECT_DOUBLE_EQ(lbt::cem::abs(0.0), 0.0);
  EXPECT_DOUBLE_EQ(lbt::cem::abs(0.0), std::abs(0.0));
}
TEST(AbsTest, DoublePositive) {
  EXPECT_DOUBLE_EQ(lbt::cem::abs(1.0), 1.0);
  EXPECT_DOUBLE_EQ(lbt::cem::abs(1.0), std::abs(1.0));
  EXPECT_DOUBLE_EQ(lbt::cem::abs(3.33), 3.33);
  EXPECT_DOUBLE_EQ(lbt::cem::abs(3.33), std::abs(3.33));
  EXPECT_DOUBLE_EQ(lbt::cem::abs(4.0), 4.0);
  EXPECT_DOUBLE_EQ(lbt::cem::abs(4.0), std::abs(4.0));
}
TEST(AbsTest, DoubleNegative) {
  EXPECT_DOUBLE_EQ(lbt::cem::abs(-1.0), 1.0);
  EXPECT_DOUBLE_EQ(lbt::cem::abs(-1.0), std::abs(1.0));
  EXPECT_DOUBLE_EQ(lbt::cem::abs(-3.33), 3.33);
  EXPECT_DOUBLE_EQ(lbt::cem::abs(-3.33), std::abs(3.33));
  EXPECT_DOUBLE_EQ(lbt::cem::abs(-4.0), 4.0);
  EXPECT_DOUBLE_EQ(lbt::cem::abs(-4.0), std::abs(4.0));
}

/// Test floating number comparison
TEST(NearlyEqualTest, NearlyEqual) {
  //constexpr double some_very_large_negative_number {std::numeric_limits<double>::min()/2.0};
  //EXPECT_TRUE(lbt::cem::nearlyEqual(some_very_large_negative_number, std::nextafter(some_very_large_negative_number, -1.0)));
  //constexpr double some_large_negative_number {-9999.99};
  //EXPECT_TRUE(lbt::cem::nearlyEqual(some_large_negative_number, std::nextafter(some_large_negative_number, -1.0)));
  constexpr double some_negative_number {-1.0};
  EXPECT_TRUE(lbt::cem::nearlyEqual(some_negative_number, std::nextafter(some_negative_number, -1.0)));
  constexpr double zero {0.0};
  EXPECT_TRUE(lbt::cem::nearlyEqual(zero, std::nextafter(zero, 1.0)));
  constexpr double some_positive_number {1.0};
  EXPECT_TRUE(lbt::cem::nearlyEqual(some_positive_number, std::nextafter(some_positive_number, 1.0)));
  //constexpr double some_large_positive_number {9999.99};
  //EXPECT_TRUE(lbt::cem::nearlyEqual(some_large_positive_number, std::nextafter(some_large_positive_number, 1.0)));
  //constexpr double some_very_large_positive_number {std::numeric_limits<double>::max()/2.0};
  //EXPECT_TRUE(lbt::cem::nearlyEqual(some_very_large_positive_number, std::nextafter(some_very_large_positive_number, 1.0)));
}

/// Tests for square root
TEST(SqrtTest, DoubleZero) {
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(0.0), 0.0);
}
TEST(SqrtTest, DoublePositive) {
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(1.0), 1.0);
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(1.0), std::sqrt(1.0));
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(3.33), std::sqrt(3.33));
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(4.0), 2.0);
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(4.0), std::sqrt(4.0));
}
TEST(SqrtTest, DoubleNegative) {
  EXPECT_TRUE(std::isnan(lbt::cem::sqrt(-1.0)));
}
TEST(SqrtTest, DoubleNan) {
  EXPECT_TRUE(std::isnan(lbt::cem::sqrt(std::numeric_limits<double>::infinity())));
}

/// Tests for ceiling function
TEST(CeilTest, DoubleZero) {
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(0.0), 0);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(0.0), std::ceil(0.0));
}
TEST(CeilTest, DoublePositive) {
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.1), 2);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.1), std::ceil(1.1));
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.5), 2);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.5), std::ceil(1.5));
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.9), 2);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.9), std::ceil(1.9));
}
TEST(CeilTest, DoubleNegative) {
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.1), -1);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.1), std::ceil(-1.1));
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.5), -1);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.5), std::ceil(-1.5));
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.9), -1);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.9), std::ceil(-1.9));
}

#endif // LBT_CONSTEXPR_MATH_UNITTEST
