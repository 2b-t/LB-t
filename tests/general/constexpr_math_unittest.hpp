#ifndef LBT_CONSTEXPR_MATH_UNITTEST
#define LBT_CONSTEXPR_MATH_UNITTEST
#pragma once

/**
 * \file     constexpr_math_unittest.hpp
 * \mainpage Tests for constexpr math functions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cmath>
#include <cstdint>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "../../src/general/constexpr_math.hpp"


namespace lbt {
  namespace test {
    /**\fn        generateRandomNumber
     * \brief     Generate a random number of floating type \p T between \p lower_bound and \p upper_bound
     *
     * \tparam    T             The corresponding floating point data type
     * \param[in] lower_bound   The smallest possible random floating point number
     * \param[in] upper_bound   The largest possible random floating point number
    * \return    A random floating point number between \p lower_bound and \p upper_bound
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    auto generateRandomNumber(T const lower_bound, T const upper_bound) noexcept {
      std::uniform_real_distribution<T> uniform_distribution {lower_bound, upper_bound};
      std::random_device random_device {};
      std::default_random_engine random_engine {random_device()};
      T const random_number {uniform_distribution(random_engine)};
      return random_number;
    }

    /**\fn        generateRandomNumber
     * \brief     Generate a random number of integer type \p T between \p lower_bound and \p upper_bound
     *
     * \tparam    T             The corresponding integral data type
     * \param[in] lower_bound   The smallest possible random integer number
     * \param[in] upper_bound   The largest possible random integer number
     * \return    A integer random number between \p lower_bound and \p upper_bound
    */
    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
    auto generateRandomNumber(T const lower_bound, T const upper_bound) {
      if (lower_bound > upper_bound) {
        throw std::invalid_argument("Range invalid: lower_bound > upper_bound (" + std::to_string(lower_bound) + " > " + std::to_string(upper_bound) + ")!");
      }
      std::uniform_int_distribution<T> uniform_distribution {lower_bound, upper_bound};
      std::random_device random_device {};
      std::default_random_engine random_engine {random_device()};
      T const random_number {uniform_distribution(random_engine)};
      return random_number;
    }

  }
}


/// Define testing data types
using IntegerDataTypes = ::testing::Types<std::int8_t,  std::int16_t,  std::int32_t,  std::int64_t, 
                                          std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>;

using FloatingPointDataTypes = ::testing::Types<float, double, long double>;

using ArithmeticDataTypes = ::testing::Types<std::int8_t,  std::int16_t,  std::int32_t,  std::int64_t, 
                                             std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t,
                                             float, double, long double>;


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
    auto const random_number {lbt::test::generateRandomNumber(lower, upper)};
    EXPECT_GE(random_number, lower);
    EXPECT_LE(random_number, upper);
  }
}

TYPED_TEST(RandomFloatingPointNumberTest, invertLimits) {
  auto constexpr min {std::numeric_limits<TypeParam>::lowest()};
  auto constexpr max {std::numeric_limits<TypeParam>::max()};
  std::vector<std::pair<TypeParam, TypeParam>> const tests {{min/2, max/2}, {min, 0}, {0, max}, {0, 0}, {0.1, 0.9}, {1, 2}};
  for (auto const& [lower, upper]: tests) {
    auto const random_number {lbt::test::generateRandomNumber(upper, lower)};
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
      auto const random_number {lbt::test::generateRandomNumber(lower, upper)};
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
    EXPECT_THROW(lbt::test::generateRandomNumber(upper, lower), std::invalid_argument);
  }
}

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

/// Test function for detecting positive infinity
template <typename T>
struct IsPosInfTest: public ::testing::Test {
};

TYPED_TEST_SUITE(IsPosInfTest, FloatingPointDataTypes);

TYPED_TEST(IsPosInfTest, positiveInfinityIsPositiveInfinity) {
  constexpr auto pos_inf {std::numeric_limits<TypeParam>::infinity()};
  EXPECT_TRUE(lbt::cem::isPosInf(pos_inf));
  EXPECT_TRUE(lbt::cem::isPosInf(pos_inf) == std::isinf(pos_inf));
}

TYPED_TEST(IsPosInfTest, negativeInfinityIsNotPositiveInfinity) {
  constexpr auto neg_inf {-std::numeric_limits<TypeParam>::infinity()};
  EXPECT_FALSE(lbt::cem::isPosInf(neg_inf));
}

TYPED_TEST(IsPosInfTest, positiveNumberIsNotPositiveInfinity) {
  std::vector<TypeParam> const positive_numbers {+0, 1, 100, std::numeric_limits<TypeParam>::max()};
  for (auto const& n: positive_numbers) {
    EXPECT_FALSE(lbt::cem::isPosInf(n));
    EXPECT_TRUE(lbt::cem::isPosInf(n) == std::isinf(n));
  }
}

TYPED_TEST(IsPosInfTest, negativeNumberIsNotPositiveInfinity) {
  std::vector<TypeParam> const negative_numbers {-0, -1, -100, std::numeric_limits<TypeParam>::min()};
  for (auto const& n: negative_numbers) {
    EXPECT_FALSE(lbt::cem::isPosInf(n));
    EXPECT_TRUE(lbt::cem::isPosInf(n) == std::isinf(n));
  }
}

/// Test function for detecting negative infinity
template <typename T>
struct IsNegInfTest: public ::testing::Test {
};

TYPED_TEST_SUITE(IsNegInfTest, FloatingPointDataTypes);

TYPED_TEST(IsNegInfTest, negativeInfinityIsNegativeInfinity) {
  constexpr auto neg_inf {-std::numeric_limits<TypeParam>::infinity()};
  EXPECT_TRUE(lbt::cem::isNegInf(neg_inf));
  EXPECT_TRUE(lbt::cem::isPosInf(neg_inf) == std::isinf(neg_inf));
}

TYPED_TEST(IsNegInfTest, positiveInfinityIsNotNegativeInfinity) {
  constexpr auto pos_inf {std::numeric_limits<TypeParam>::infinity()};
  EXPECT_FALSE(lbt::cem::isNegInf(pos_inf));
}

TYPED_TEST(IsNegInfTest, positiveNumberIsNotNegativeInfinity) {
  std::vector<TypeParam> const positive_numbers {+0, 1, 100, std::numeric_limits<TypeParam>::max()};
  for (auto const& n: positive_numbers) {
    EXPECT_FALSE(lbt::cem::isNegInf(n));
    EXPECT_TRUE(lbt::cem::isPosInf(n) == std::isinf(n));
  }
}

TYPED_TEST(IsNegInfTest, negativeNumberIsNotNegativeInfinity) {
  std::vector<TypeParam> const negative_numbers {-0, -1, -100, std::numeric_limits<TypeParam>::min()};
  for (auto const& n: negative_numbers) {
    EXPECT_FALSE(lbt::cem::isNegInf(n));
    EXPECT_TRUE(lbt::cem::isPosInf(n) == std::isinf(n));
  }
}

/// Test function for calculating absolute value
template <typename T>
struct AbsTest: public ::testing::Test {
};

TYPED_TEST_SUITE(AbsTest, ArithmeticDataTypes);

TYPED_TEST(AbsTest, negativeZeroIsPositiveZero) {
  constexpr TypeParam negative_zero {-0};
  constexpr TypeParam positive_zero {+0};
  if constexpr (std::is_same_v<TypeParam,float>) {
    EXPECT_FLOAT_EQ(lbt::cem::abs(negative_zero), positive_zero);
    EXPECT_FLOAT_EQ(lbt::cem::abs(negative_zero), std::abs(negative_zero));
  } else if constexpr (std::is_same_v<TypeParam,double>) {
    EXPECT_DOUBLE_EQ(lbt::cem::abs(negative_zero), positive_zero);
    EXPECT_DOUBLE_EQ(lbt::cem::abs(negative_zero), std::abs(negative_zero));
  } else {
    GTEST_SKIP() << "Test not implemented for given data type!";
  }
}

TYPED_TEST(AbsTest, negativeNumberIsPositiveNumber) {
  std::vector<std::pair<TypeParam, TypeParam>> const tests {{-1, 1}, {-100, 100}};
  for (auto const& [n, p]: tests) {
    if constexpr (std::is_same_v<TypeParam,float>) {
      EXPECT_FLOAT_EQ(lbt::cem::abs(n), p);
      EXPECT_FLOAT_EQ(lbt::cem::abs(n), std::abs(n));
    } else if constexpr (std::is_same_v<TypeParam,double>) {
      EXPECT_DOUBLE_EQ(lbt::cem::abs(n), p);
      EXPECT_DOUBLE_EQ(lbt::cem::abs(n), std::abs(n));
    } else {
      GTEST_SKIP() << "Test not implemented for given data type!";
    }
  }
}

TYPED_TEST(AbsTest, positiveNumberStaysPositiveNumber) {
  std::vector<TypeParam> const tests {1, 100};
  for (auto const& n: tests) {
    if constexpr (std::is_same_v<TypeParam,float>) {
      EXPECT_FLOAT_EQ(lbt::cem::abs(n), n);
      EXPECT_FLOAT_EQ(lbt::cem::abs(n), std::abs(n));
    } else if constexpr (std::is_same_v<TypeParam,double>) {
      EXPECT_DOUBLE_EQ(lbt::cem::abs(n), n);
      EXPECT_DOUBLE_EQ(lbt::cem::abs(n), std::abs(n));
    } else {
      GTEST_SKIP() << "Test not implemented for given data type!";
    }
  }
}

/// Test floating number comparison
TEST(NearlyEqualTest, nearlyEqual) {
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

/// Templated fixture class
//template <typename T>
//struct SqrtTest: public ::testing::Test {
//};

//TYPED_TEST_SUITE(SqrtTest, FloatingPointDataTypes);

//GTEST_SKIP() << "Test not implemented for given data type!";

/// Tests for square root
TEST(SqrtTest, doubleZero) {
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(0.0), 0.0);
}
TEST(SqrtTest, doublePositive) {
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(1.0), 1.0);
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(1.0), std::sqrt(1.0));
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(3.33), std::sqrt(3.33));
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(4.0), 2.0);
  EXPECT_DOUBLE_EQ(lbt::cem::sqrt(4.0), std::sqrt(4.0));
}
TEST(SqrtTest, doubleNegative) {
  EXPECT_TRUE(std::isnan(lbt::cem::sqrt(-1.0)));
}
TEST(SqrtTest, doubleNan) {
  EXPECT_TRUE(std::isinf(lbt::cem::sqrt(std::numeric_limits<double>::infinity())));
}

/// Tests for ceiling function
TEST(CeilTest, doubleZero) {
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(0.0), 0);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(0.0), std::ceil(0.0));
}
TEST(CeilTest, doublePositive) {
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.1), 2);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.1), std::ceil(1.1));
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.5), 2);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.5), std::ceil(1.5));
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.9), 2);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(1.9), std::ceil(1.9));
}
TEST(CeilTest, doubleNegative) {
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.1), -1);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.1), std::ceil(-1.1));
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.5), -1);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.5), std::ceil(-1.5));
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.9), -1);
  EXPECT_DOUBLE_EQ(lbt::cem::ceil(-1.9), std::ceil(-1.9));
}

#endif // LBT_CONSTEXPR_MATH_UNITTEST
