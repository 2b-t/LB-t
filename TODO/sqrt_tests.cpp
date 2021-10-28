/**
 * \file   sqrt_tests.cpp
 * \brief  Different templated tests of the square root function from the standard library with GTest.
 *         The tests were designed taking into consideration the documentation at https://en.cppreference.com/w/cpp/numeric/math/sqrt
 * \author Tobit Flatscher <tobit.flatscher@fraunhofer.it>
 *         Copyright (C) 2021 Fraunhofer Italia Research
*/

#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <random>
#include <tuple>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>


namespace sqrtTests {

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>,std::decay_t<T>>
generateRandomNumber(T const lower_bound, T const upper_bound) noexcept {
  std::uniform_real_distribution<T> uniform_distribution {lower_bound, upper_bound};
  std::random_device random_device {};
  std::default_random_engine random_engine {random_device()};
  T const random_number {uniform_distribution(random_engine)};
  return random_number;
}

template <typename T>
std::enable_if_t<std::is_integral_v<T>,std::decay_t<T>>
generateRandomNumber(T const lower_bound, T const upper_bound) noexcept {
  std::uniform_int_distribution<T> uniform_distribution {lower_bound, upper_bound};
  std::random_device random_device {};
  std::default_random_engine random_engine {random_device()};
  T const random_number {uniform_distribution(random_engine)};
  return random_number;
}

}

/// Templated fixture class
template <typename T>
struct SqrtTest: public ::testing::Test {
};

/// Templated testing types
using TestingDataTypes = ::testing::Types<float, double>;

/// Connect ficture class with templated testing types
TYPED_TEST_SUITE(SqrtTest, TestingDataTypes);

/// Templated tests
TYPED_TEST(SqrtTest, positiveInfinityStaysPositiveInfinity) {
  constexpr auto inf {std::numeric_limits<TypeParam>::infinity()};
  EXPECT_TRUE(std::isinf(std::sqrt(inf)));
}

TYPED_TEST(SqrtTest, positiveZeroStaysPositiveZero) {
  constexpr TypeParam positive_zero {+0.0};
  if constexpr (std::is_same_v<TypeParam,float>) {
    EXPECT_FLOAT_EQ(std::sqrt(positive_zero), positive_zero);
  } else if constexpr (std::is_same_v<TypeParam,double>) {
    EXPECT_DOUBLE_EQ(std::sqrt(positive_zero), positive_zero);
  } else {
    EXPECT_TRUE(false) << "Only float and double data types supported!";
  }
}

TYPED_TEST(SqrtTest, negativeZeroStaysNegativeZero) {
  constexpr TypeParam negative_zero {-0.0};
  if constexpr (std::is_same_v<TypeParam,float>) {
    EXPECT_FLOAT_EQ(std::sqrt(negative_zero), negative_zero);
  } else if constexpr (std::is_same_v<TypeParam,double>) {
    EXPECT_DOUBLE_EQ(std::sqrt(negative_zero), negative_zero);
  } else {
    EXPECT_TRUE(false) << "Only float and double data types supported!";
  }
}

TYPED_TEST(SqrtTest, signalingNanIsNan) {
  constexpr auto nan {std::numeric_limits<TypeParam>::signaling_NaN()};
  EXPECT_TRUE(std::isnan(std::sqrt(nan)));
}

TYPED_TEST(SqrtTest, quietNanIsNan) {
  constexpr auto nan {std::numeric_limits<TypeParam>::quiet_NaN()};
  EXPECT_TRUE(std::isnan(std::sqrt(nan)));
}

TYPED_TEST(SqrtTest, negativeNumberIsNan) {
  constexpr TypeParam negative_number {-1.0};
  EXPECT_TRUE(std::isnan(std::sqrt(negative_number)));
}

TYPED_TEST(SqrtTest, maxIsPositiveNumber) {
  constexpr auto max {std::numeric_limits<TypeParam>::max()};
  EXPECT_TRUE(std::sqrt(max) > 0.0);
}

TYPED_TEST(SqrtTest, unityIsUnity) {
  constexpr auto unity {static_cast<TypeParam>(1.0)};
  if constexpr (std::is_same_v<TypeParam,float>) {
    EXPECT_FLOAT_EQ(std::sqrt(unity), unity);
  } else if constexpr (std::is_same_v<TypeParam,double>) {
    EXPECT_DOUBLE_EQ(std::sqrt(unity), unity);
  } else {
    EXPECT_TRUE(false) << "Only float and double data types supported!";
  }
}

TYPED_TEST(SqrtTest, positiveNumbersCorrectSqrt) {
  std::vector<std::pair<TypeParam,TypeParam>> const positive_numbers = { {4.0, 2.0}, {9.8596, 3.14}, {100.0, 10.0} };
  for (auto const& p: positive_numbers) {
    if constexpr (std::is_same_v<TypeParam,float>) {
      EXPECT_FLOAT_EQ(std::sqrt(p.first), p.second);
    } else if constexpr (std::is_same_v<TypeParam,double>) {
      EXPECT_DOUBLE_EQ(std::sqrt(p.first), p.second);
    } else {
      EXPECT_TRUE(false) << "Only float and double data types supported!";
    }
  }
}

TYPED_TEST(SqrtTest, randomNumberSquaredCorrectSqrt) {
  constexpr TypeParam largest_number {std::numeric_limits<TypeParam>::max()};
  int exponent_max {0};
  TypeParam const mantissa_max {std::frexp(largest_number, &exponent_max)};
  // The exponent is only allowed to the sqrt(2^n)=2^(n/2)
  exponent_max /= 2;
  
  // Generate a random number by constructing random number for exponent and mantissa seperately so numbers are not too large
  int const exponent {sqrtTests::generateRandomNumber(0, exponent_max)};
  TypeParam const mantissa {sqrtTests::generateRandomNumber(static_cast<TypeParam>(0.0), mantissa_max)};
  TypeParam const random_number {mantissa*static_cast<TypeParam>(std::pow(static_cast<TypeParam>(2.0), exponent))};
  
  // Square it and test if it is the same number again
  TypeParam const random_number_squared {static_cast<TypeParam>(std::pow(random_number, 2))};
  if constexpr (std::is_same_v<TypeParam,float>) {
    EXPECT_FLOAT_EQ(std::sqrt(random_number_squared), random_number);
  } else if constexpr (std::is_same_v<TypeParam,double>) {
    EXPECT_DOUBLE_EQ(std::sqrt(random_number_squared), random_number);
  } else {
    EXPECT_TRUE(false) << "Only float and double data types supported!";
  }
}

TYPED_TEST(SqrtTest, smallestPositiveNumberLargerSqrt) {
  auto const smallest_positive_number {std::nextafter(static_cast<TypeParam>(0.0), static_cast<TypeParam>(1.0))};
  EXPECT_TRUE(std::sqrt(smallest_positive_number) > smallest_positive_number);
}


/**\fn        main
 * \brief     The main function which performs all tests and returns a Boolean value signaling success or failure.
 *
 * \param[in] argc   The number of input arguments supplied to the tests
 * \param[in] argv   The different input arguments supplied to the tests
 * \return    The return value signaling success or failure of the tests
*/
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
