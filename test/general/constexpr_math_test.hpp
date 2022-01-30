#ifndef LBT_CONSTEXPR_MATH_UNITTEST
#define LBT_CONSTEXPR_MATH_UNITTEST
#pragma once

/**
 * \file     constexpr_math_test.hpp
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

    /// Define testing data types
    using IntegerDataTypes = ::testing::Types<std::int8_t,  std::int16_t,  std::int32_t,  std::int64_t, 
                                              std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>;

    using FloatingPointDataTypes = ::testing::Types<float, double>;

    using ArithmeticDataTypes = ::testing::Types<std::int8_t,  std::int16_t,  std::int32_t,  std::int64_t, 
                                                std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t,
                                                float, double>;

    /// Test pi definition
    template <typename T>
    struct PiConstantTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(PiConstantTest, FloatingPointDataTypes);

    TYPED_TEST(PiConstantTest, equalToStdPi) {
      auto const result {lbt::cem::pi<TypeParam>};
      auto const expected_result {static_cast<TypeParam>(M_PI)};

      if constexpr (std::is_same_v<TypeParam,float>) {
        EXPECT_FLOAT_EQ(result, expected_result);
      } else if constexpr (std::is_same_v<TypeParam,double>) {
        EXPECT_DOUBLE_EQ(result, expected_result);
      } else {
        GTEST_SKIP() << "Test not implemented for given data type!";
      }
    }

    /// Test e defintion
    template <typename T>
    struct EulerNumberConstantTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(EulerNumberConstantTest, FloatingPointDataTypes);

    TYPED_TEST(EulerNumberConstantTest, equalToStdEulerNumber) {
      auto const result {lbt::cem::e<TypeParam>};
      auto const expected_result {static_cast<TypeParam>(std::exp(1.0))};

      if constexpr (std::is_same_v<TypeParam,float>) {
        EXPECT_FLOAT_EQ(result, expected_result);
      } else if constexpr (std::is_same_v<TypeParam,double>) {
        EXPECT_DOUBLE_EQ(result, expected_result);
      } else {
        GTEST_SKIP() << "Test not implemented for given data type!";
      }
    }

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
        auto const random_number {generateRandomNumber(lower, upper)};
        EXPECT_GE(random_number, lower);
        EXPECT_LE(random_number, upper);
      }
    }

    TYPED_TEST(RandomFloatingPointNumberTest, invertLimits) {
      auto constexpr min {std::numeric_limits<TypeParam>::lowest()};
      auto constexpr max {std::numeric_limits<TypeParam>::max()};
      std::vector<std::pair<TypeParam, TypeParam>> const tests {{min/2, max/2}, {min, 0}, {0, max}, {0, 0}, {0.1, 0.9}, {1, 2}};
      for (auto const& [lower, upper]: tests) {
        auto const random_number {generateRandomNumber(upper, lower)};
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
          auto const random_number {generateRandomNumber(lower, upper)};
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
        EXPECT_THROW(generateRandomNumber(upper, lower), std::invalid_argument);
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
        EXPECT_EQ(lbt::cem::isPosInf(n), std::isinf(n));
      }
    }

    TYPED_TEST(IsPosInfTest, negativeNumberIsNotPositiveInfinity) {
      std::vector<TypeParam> const negative_numbers {-0, -1, -100, std::numeric_limits<TypeParam>::min()};
      for (auto const& n: negative_numbers) {
        EXPECT_FALSE(lbt::cem::isPosInf(n));
        EXPECT_EQ(lbt::cem::isPosInf(n), std::isinf(n));
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
      EXPECT_EQ(lbt::cem::isNegInf(neg_inf), std::isinf(neg_inf));
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

    /// Test floating number comparison with nearly equal function
    template <typename T>
    struct NearlyEqualTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(NearlyEqualTest, FloatingPointDataTypes);

    TYPED_TEST(NearlyEqualTest, closeNumberIsNearlyEqual) {
      std::vector<std::pair<TypeParam,TypeParam>> tests {};
      std::vector<TypeParam> const numbers {-1, 0, 1};
      for (auto const& n: numbers) {
        tests.emplace_back(n, std::nextafter(n, static_cast<TypeParam>(-1)));
        tests.emplace_back(n, std::nextafter(n, static_cast<TypeParam>(1)));
      }
      for (auto const& [a, b]: tests) {
        EXPECT_TRUE(lbt::cem::isNearlyEqual(a, b));
      }
    }

    TYPED_TEST(NearlyEqualTest, distantNumberIsNotNearlyEqual) {
      std::vector<std::pair<TypeParam,TypeParam>> tests = { {-1, 1}, {1, -1}, {3, 3.14} };
      for (auto const& [a, b]: tests) {
        EXPECT_FALSE(lbt::cem::isNearlyEqual(a, b));
      }
    }

    /// Test square root function
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
      std::vector<std::pair<TypeParam,TypeParam>> const tests = { {4.0, 2.0}, {9.8596, 3.14}, {100.0, 10.0} };
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
      std::vector<TypeParam> const tests = { {4.0}, {9.8596}, {100.0} };
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
      std::vector<TypeParam> const zeros {static_cast<TypeParam>(-0), static_cast<TypeParam>(+0)};
      for (auto const& z: zeros) {
        auto const smallest_positive_number {std::nextafter(z, static_cast<TypeParam>(1))};
        EXPECT_GT(lbt::cem::sqrt(smallest_positive_number), z);
      }
    }

    TYPED_TEST(SqrtTest, smallestNegativeNumberIsNan) {
      std::vector<TypeParam> const zeros {static_cast<TypeParam>(-0), static_cast<TypeParam>(+0)};
      for (auto const& z: zeros) {
        auto const smallest_negative_number {std::nextafter(z, static_cast<TypeParam>(-1))};
        EXPECT_TRUE(std::isnan(lbt::cem::sqrt(smallest_negative_number)));
      }
    }

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
        EXPECT_DOUBLE_EQ(lbt::cem::sqrt(zero), zero);
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

    /// Tests for exponential function
    template <typename T>
    struct ExpTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(ExpTest, FloatingPointDataTypes);

    TYPED_TEST(ExpTest, zeroIsUnity) {
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      constexpr auto unity {static_cast<TypeParam>(1.0)};
      if constexpr (std::is_same_v<TypeParam,float>) {
        EXPECT_FLOAT_EQ(lbt::cem::exp(zero), unity);
      } else if constexpr (std::is_same_v<TypeParam,double>) {
        EXPECT_DOUBLE_EQ(lbt::cem::exp(zero), unity);
      } else {
        GTEST_SKIP() << "Test not implemented for given data type!";
      }
    }

    TYPED_TEST(ExpTest, negativeInfinityIsZero) {
      constexpr auto inf {-std::numeric_limits<TypeParam>::infinity()};
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      if constexpr (std::is_same_v<TypeParam,float>) {
        EXPECT_FLOAT_EQ(lbt::cem::exp(inf), zero);
      } else if constexpr (std::is_same_v<TypeParam,double>) {
        EXPECT_DOUBLE_EQ(lbt::cem::exp(inf), zero);
      } else {
        GTEST_SKIP() << "Test not implemented for given data type!";
      }
    }

    TYPED_TEST(ExpTest, positiveInfinityIsPositiveInfinity) {
      constexpr auto inf {std::numeric_limits<TypeParam>::infinity()};
      EXPECT_TRUE(cem::isPosInf(lbt::cem::exp(inf)));
    }

    TYPED_TEST(ExpTest, nanIsNan) {
      constexpr auto nan {std::numeric_limits<TypeParam>::signaling_NaN()};
      EXPECT_TRUE(std::isnan(lbt::cem::exp(nan)));
    }

    TYPED_TEST(ExpTest, unityIsEulersNumber) {
      constexpr auto unity {static_cast<TypeParam>(1.0)};
      constexpr auto eulers_number {lbt::cem::e<TypeParam>};
      if constexpr (std::is_same_v<TypeParam,float>) {
        EXPECT_FLOAT_EQ(lbt::cem::exp(unity), eulers_number);
      } else if constexpr (std::is_same_v<TypeParam,double>) {
        EXPECT_DOUBLE_EQ(lbt::cem::exp(unity), eulers_number);
      } else {
        GTEST_SKIP() << "Test not implemented for given data type!";
      }
    }

    TYPED_TEST(ExpTest, positiveNumbersAreCorrect) {
      std::vector<std::pair<TypeParam,TypeParam>> const tests = { { 0.2, static_cast<TypeParam>(1.2214027581601698339210719946396741703075809415205036412734)},
                                                                  { 1.5, static_cast<TypeParam>(4.4816890703380648226020554601192758190057498683696670567726)},
                                                                  {10.0, static_cast<TypeParam>(22026.465794806716516957900645284244366353512618556781074235426355)},
                                                                  {21.0, static_cast<TypeParam>(1.31881573448321469720999888374530278509144443738047574906598e+9)}
                                                                };
      for (auto const& [n, solution]: tests) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::exp(n), solution);
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::exp(n), solution);
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

    TYPED_TEST(ExpTest, negativeNumbersAreCorrect) {
      std::vector<std::pair<TypeParam,TypeParam>> const tests = { { -0.4, static_cast<TypeParam>(0.6703200460356393007444329251478260719369809252108121998889)}, 
                                                                  { -1.0, static_cast<TypeParam>(1.0/lbt::cem::e<TypeParam>)}, 
                                                                  { -2.3, static_cast<TypeParam>(0.1002588437228037337299406937979871569083970498348733336617)}, 
                                                                  {-13.0, static_cast<TypeParam>(2.2603294069810543257852772905386894693531424227032186440353e-6)}
                                                                };
      for (auto const& [n, solution]: tests) {
        EXPECT_NEAR(lbt::cem::exp(n), solution, 0.0005);
      }
    }

    TYPED_TEST(ExpTest, positiveNumbersEqualToStdExp) {
      std::vector<TypeParam> const tests = { 0.2, 1.5, 10.0, 21.0 };
      for (auto const& n: tests) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::exp(n), std::exp(n));
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::exp(n), std::exp(n));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

     TYPED_TEST(ExpTest, negativeNumbersEqualToStdExp) {
      std::vector<TypeParam> const tests = { -0.4, -1.0, -2.3, -13.0 };
      for (auto const& n: tests) {
        EXPECT_NEAR(lbt::cem::exp(n), std::exp(n), 0.0005);
      }
    }

    /// Tests for logarithm function
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
      EXPECT_TRUE(lbt::cem::isNearlyEqual(lbt::cem::log(unity), zero));
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
      EXPECT_TRUE(lbt::cem::isNearlyEqual(lbt::cem::log(eulers_number), unity));
    }

    TYPED_TEST(LogTest, positiveNumbersAreCorrect) {
      std::vector<std::pair<TypeParam,TypeParam>> const tests = { {  0.2, static_cast<TypeParam>(-1.609437912434100374600759333226187639525601354268517721912)},
                                                                  {  1.5, static_cast<TypeParam>(0.4054651081081643819780131154643491365719904234624941976140)},
                                                                  { 21.0, static_cast<TypeParam>(3.0445224377234229965005979803657054342845752874046106401940844835)},
                                                                  {325.4, static_cast<TypeParam>(5.7850551947849374239386534894581251752940419349976263065587)}
                                                                };
      for (auto const& [n, solution]: tests) {
        EXPECT_NEAR(lbt::cem::log(n), solution, 0.0005);
      }
    }

    TYPED_TEST(LogTest, positiveNumbersEqualToStdLog) {
      std::vector<TypeParam> const tests = { 0.2, 1.5, 21.0, 325.4 };
      for (auto const& n: tests) {
        EXPECT_NEAR(lbt::cem::log(n), std::log(n), 0.0005);
      }
    }

    /// Tests for power function
    template <typename T>
    struct PowTest: public ::testing::Test {
    };

    TYPED_TEST_SUITE(PowTest, FloatingPointDataTypes);

    TYPED_TEST(PowTest, zeroToThePowerOfNanIsNan) {
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      constexpr auto nan {std::numeric_limits<TypeParam>::signaling_NaN()};
      EXPECT_TRUE(lbt::cem::isNan(lbt::cem::pow(zero, nan)));
    }

    TYPED_TEST(PowTest, anythingToThePowerOfZeroIsUnity) {
      std::vector<TypeParam> const bases { std::numeric_limits<TypeParam>::signaling_NaN(), 
                                          -std::numeric_limits<TypeParam>::infinity(), 
                                           std::numeric_limits<TypeParam>::infinity(),
                                          -65.23, 0.0, 4.3232 };
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      constexpr auto unity {static_cast<TypeParam>(1.0)};
      for (auto const& b: bases) {
        EXPECT_TRUE(lbt::cem::isNearlyEqual(lbt::cem::pow(b, zero), unity));
      }
    }

    TYPED_TEST(PowTest, zeroToThePowerOfNegativeInfinityIsInfinity) {
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      constexpr auto inf {-std::numeric_limits<TypeParam>::infinity()};
      EXPECT_TRUE(lbt::cem::isPosInf(lbt::cem::pow(zero, inf)));
    }

    TYPED_TEST(PowTest, zeroToThePowerOfPositiveInfinityIsZero) {
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      constexpr auto inf {std::numeric_limits<TypeParam>::infinity()};
      EXPECT_TRUE(lbt::cem::isNearlyEqual(lbt::cem::pow(zero, inf), zero));
    }

    TYPED_TEST(PowTest, anythingElseInvolvingNanIsNan) {
      constexpr auto nan {std::numeric_limits<TypeParam>::signaling_NaN()};
      constexpr auto pos_inf {std::numeric_limits<TypeParam>::infinity()};
      constexpr auto neg_inf {-std::numeric_limits<TypeParam>::infinity()};
      std::vector<std::pair<TypeParam,TypeParam>> const base_exponent = { {  nan,     nan },
                                                                          { -3.6,     nan },
                                                                          {  7.2,     nan },
                                                                          {  nan,    -4.57 },
                                                                          {  nan,     6.1 },
                                                                          {  pos_inf, nan },
                                                                          {  neg_inf, nan },
                                                                          {  nan,     pos_inf },
                                                                          {  nan,     neg_inf }
                                                                        };
      for (auto const& [base, exponent]: base_exponent) {
        EXPECT_TRUE(lbt::cem::isNan(lbt::cem::pow(base, exponent)));
      }
    }

    TYPED_TEST(PowTest, negativeInfinityToThePowerOfFloatingPointNumberIsPositiveInfinity) {
      constexpr auto inf {-std::numeric_limits<TypeParam>::infinity()};
      std::vector<TypeParam> const exponents { 0.9, 1.1, 3.4 };
      for (auto const& e: exponents) {
        EXPECT_TRUE(lbt::cem::isPosInf(lbt::cem::pow(inf, e)));
      }
    }

    TYPED_TEST(PowTest, negativeInfinityToThePowerOfEvenIntegerIsPositiveInfinity) {
      constexpr auto inf {-std::numeric_limits<TypeParam>::infinity()};
      std::vector<TypeParam> const exponents { 2.0, 4.0 };
      for (auto const& e: exponents) {
        EXPECT_TRUE(lbt::cem::isPosInf(lbt::cem::pow(inf, e)));
      }
    }

    TYPED_TEST(PowTest, negativeInfinityToThePowerOfOddIntegerIsNegativeInfinity) {
      constexpr auto inf {-std::numeric_limits<TypeParam>::infinity()};
      std::vector<TypeParam> const exponents { 3.0, 5.0 };
      for (auto const& e: exponents) {
        EXPECT_TRUE(lbt::cem::isNegInf(lbt::cem::pow(inf, e)));
      }
    }

    TYPED_TEST(PowTest, negativeInfinityToThePowerOfNegativeExponentIsZero) {
      constexpr auto inf {-std::numeric_limits<TypeParam>::infinity()};
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      std::vector<TypeParam> const exponents { -3.0, -4.0, -2.3 };
      for (auto const& e: exponents) {
        EXPECT_TRUE(lbt::cem::isNearlyEqual(lbt::cem::pow(inf, e), zero));
      }
    }

    TYPED_TEST(PowTest, positiveInfinityToThePowerOfPositiveExponentIsInfinity) {
      constexpr auto inf {std::numeric_limits<TypeParam>::infinity()};
      std::vector<TypeParam> const exponents { 0.8, 2.3, inf };
      for (auto const& e: exponents) {
        EXPECT_TRUE(lbt::cem::isPosInf(lbt::cem::pow(inf, e)));
      }
    }

    TYPED_TEST(PowTest, negativeInfinityToThePowerOfPositiveInfinityIsInfinity) {
      constexpr auto neg_inf {-std::numeric_limits<TypeParam>::infinity()};
      constexpr auto pos_inf {std::numeric_limits<TypeParam>::infinity()};
      EXPECT_TRUE(lbt::cem::isPosInf(lbt::cem::pow(neg_inf, pos_inf)));
    }

    TYPED_TEST(PowTest, infinityToZero) {
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      constexpr auto pos_inf {std::numeric_limits<TypeParam>::infinity()};
      constexpr auto neg_inf {-std::numeric_limits<TypeParam>::infinity()};
      std::vector<std::pair<TypeParam,TypeParam>> const base_exponent = { { pos_inf, neg_inf },
                                                                          { neg_inf, neg_inf },
                                                                          { 4.0,     neg_inf },
                                                                          { 4.3,     neg_inf },
                                                                          { 0.7,     pos_inf }
                                                                        };
      for (auto const& [base, exponent]: base_exponent) {
        EXPECT_TRUE(lbt::cem::isNearlyEqual(lbt::cem::pow(base, exponent), zero));
      }
    }

    TYPED_TEST(PowTest, infinityToInfinity) {
      constexpr auto pos_inf {std::numeric_limits<TypeParam>::infinity()};
      constexpr auto neg_inf {-std::numeric_limits<TypeParam>::infinity()};
      std::vector<std::pair<TypeParam,TypeParam>> const base_exponent = { { 4.0, pos_inf },
                                                                          { 4.3, pos_inf },
                                                                          { 0.7, neg_inf }
                                                                        };
      for (auto const& [base, exponent]: base_exponent) {
        EXPECT_TRUE(lbt::cem::isPosInf(lbt::cem::pow(base, exponent)));
      }
    }

    TYPED_TEST(PowTest, zeroToThePowerOfPositiveNumberIsZero) {
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      std::vector<TypeParam> const exponents = { 0.9, 4.0, 6.3};
      for (auto const& e: exponents) {
        EXPECT_TRUE(lbt::cem::isNearlyEqual(lbt::cem::pow(zero, e), zero));
      }
    }

    TYPED_TEST(PowTest, zeroToThePowerOfNegativeNumberIsPositiveInfinity) {
      constexpr auto zero {static_cast<TypeParam>(0.0)};
      std::vector<TypeParam> const exponents = { -0.9, -4.0, -6.3};
      for (auto const& e: exponents) {
        EXPECT_TRUE(lbt::cem::isPosInf(lbt::cem::pow(zero, e)));
      }
    }

    TYPED_TEST(PowTest, positiveBasePositiveExponentAreCorrect) {
      std::vector<std::tuple<TypeParam,TypeParam,TypeParam>> const base_exponent_solution = { { 0.7, 0.6, 0.8073443754472971999565038610187399822786628231647471274284 },
                                                                                              { 0.3, 5.0, 0.00243 },
                                                                                              { 0.5, 3.2, 0.1088188204120155173920337521849682623723906780435003810302 },
                                                                                              { 1.3, 0.5, 1.1401754250991379791360490255667544790760053109164103752974 },
                                                                                              { 1.7, 3.5, 6.4057682833521226269492278523299166076805049471570303199646 }, 
                                                                                              { 2.7, 4.0, 53.1441 }
                                                                                            };
      for (auto const& [base, exponent, solution]: base_exponent_solution) {
        EXPECT_NEAR(lbt::cem::pow(base, exponent), solution, 0.0005);
      }
    }
    
    TYPED_TEST(PowTest, positiveBaseNegativeExponentAreCorrect) {
      std::vector<std::tuple<TypeParam,TypeParam,TypeParam>> const base_exponent_solution = { { 0.7, -0.6,   1.2386288062587477656916792410959430949933777803151303412214 },
                                                                                              { 0.3, -5.0, 411.52263374485596707818930041152263374485596707818930041152263374 },
                                                                                              { 0.5, -3.2,   9.1895868399762800543890155742234207155508071127823800441096 },
                                                                                              { 1.3, -0.5,   0.8770580193070292147200377119744265223661579314741618271518 },
                                                                                              { 1.7, -3.5,   0.1561092995822044447404358558309690836203611972775724777948 },
                                                                                              { 2.7, -4.0,   0.0188167642315892074567073296941711309439806112061357704806366087 }
                                                                                            };
      for (auto const& [base, exponent, solution]: base_exponent_solution) {
        EXPECT_NEAR(lbt::cem::pow(base, exponent), solution, 0.0005);
      }
    }

    TYPED_TEST(PowTest, negativeBasePositiveExponentAreCorrect) {
      std::vector<std::tuple<TypeParam,TypeParam,TypeParam>> const base_exponent_solution = { { -0.3, 5.0, -0.00243 },
                                                                                              { -2.7, 4.0, 53.1441 }
                                                                                            };
      for (auto const& [base, exponent, solution]: base_exponent_solution) {
        EXPECT_NEAR(lbt::cem::pow(base, exponent), solution, 0.0005);
      }
    }
    
    TYPED_TEST(PowTest, negativeBaseNegativeExponentAreCorrect) {
      std::vector<std::tuple<TypeParam,TypeParam,TypeParam>> const base_exponent_solution = { { -0.3, -5.0, -411.5226337448559670781893004115226337448559670781893004115 },
                                                                                              { -2.7, -4.0, 0.0188167642315892074567073296941711309439806112061357704806366087 }
                                                                                            };
      for (auto const& [base, exponent, solution]: base_exponent_solution) {
        EXPECT_NEAR(lbt::cem::pow(base, exponent), solution, 0.0005);
      }
    }

  }
}

#endif // LBT_CONSTEXPR_MATH_UNITTEST
