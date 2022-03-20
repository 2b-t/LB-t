/**
 * \file     pow_test.hpp
 * \mainpage Tests for constexpr power function
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_POW_UNITTEST
#define LBT_CEM_POW_UNITTEST
#pragma once

#include <cmath>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "../../../src/constexpr_math/detail/is_inf.hpp"
#include "../../../src/constexpr_math/detail/is_nan.hpp"
#include "../../../src/constexpr_math/detail/is_nearly_equal_eps_abs.hpp"
#include "../../../src/constexpr_math/detail/pow.hpp"
#include "testing_types.hpp"


namespace lbt {
  namespace test {

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
        EXPECT_TRUE(lbt::cem::isNearlyEqualEpsAbs(lbt::cem::pow(b, zero), unity));
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
      EXPECT_TRUE(lbt::cem::isNearlyEqualEpsAbs(lbt::cem::pow(zero, inf), zero));
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
        EXPECT_TRUE(lbt::cem::isNearlyEqualEpsAbs(lbt::cem::pow(inf, e), zero));
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
        EXPECT_TRUE(lbt::cem::isNearlyEqualEpsAbs(lbt::cem::pow(base, exponent), zero));
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
        EXPECT_TRUE(lbt::cem::isNearlyEqualEpsAbs(lbt::cem::pow(zero, e), zero));
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
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::pow(base, exponent), solution);
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::pow(base, exponent), solution);
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
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
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::pow(base, exponent), solution);
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::pow(base, exponent), solution);
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

    TYPED_TEST(PowTest, negativeBasePositiveExponentAreCorrect) {
      std::vector<std::tuple<TypeParam,TypeParam,TypeParam>> const base_exponent_solution = { { -0.3, 5.0, -0.00243 },
                                                                                              { -2.7, 4.0, 53.1441 }
                                                                                            };
      for (auto const& [base, exponent, solution]: base_exponent_solution) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::pow(base, exponent), solution);
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::pow(base, exponent), solution);
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }
    
    TYPED_TEST(PowTest, negativeBaseNegativeExponentAreCorrect) {
      std::vector<std::tuple<TypeParam,TypeParam,TypeParam>> const base_exponent_solution = { { -0.3, -5.0, -411.5226337448559670781893004115226337448559670781893004115 },
                                                                                              { -2.7, -4.0, 0.0188167642315892074567073296941711309439806112061357704806366087 }
                                                                                            };
      for (auto const& [base, exponent, solution]: base_exponent_solution) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::pow(base, exponent), solution);
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::pow(base, exponent), solution);
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

    TYPED_TEST(PowTest, positiveBasePositiveExponentEqualToStdPow) {
      std::vector<std::tuple<TypeParam,TypeParam>> const base_exponent = { { 0.7, 0.6 },
                                                                           { 0.3, 5.0 },
                                                                           { 0.5, 3.2 },
                                                                           { 1.3, 0.5 },
                                                                           { 1.7, 3.5 }, 
                                                                           { 2.7, 4.0 }
                                                                         };
      for (auto const& [base, exponent]: base_exponent) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::pow(base, exponent), std::pow(base, exponent));
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::pow(base, exponent), std::pow(base, exponent));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }
    
    TYPED_TEST(PowTest, positiveBaseNegativeExponentEqualToStdPow) {
      std::vector<std::tuple<TypeParam,TypeParam>> const base_exponent = { { 0.7, -0.6 },
                                                                           { 0.3, -5.0 },
                                                                           { 0.5, -3.2 },
                                                                           { 1.3, -0.5 },
                                                                           { 1.7, -3.5 },
                                                                           { 2.7, -4.0 }
                                                                         };
      for (auto const& [base, exponent]: base_exponent) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::pow(base, exponent), std::pow(base, exponent));
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::pow(base, exponent), std::pow(base, exponent));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

    TYPED_TEST(PowTest, negativeBasePositiveExponentEqualToStdPow) {
      std::vector<std::tuple<TypeParam,TypeParam>> const base_exponent = { { -0.3, 5.0 },
                                                                           { -2.7, 4.0 }
                                                                         };
      for (auto const& [base, exponent]: base_exponent) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::pow(base, exponent), std::pow(base, exponent));
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::pow(base, exponent), std::pow(base, exponent));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }
    
    TYPED_TEST(PowTest, negativeBaseNegativeExponentEqualToStdPow) {
      std::vector<std::tuple<TypeParam,TypeParam>> const base_exponent = { { -0.3, -5.0 },
                                                                           { -2.7, -4.0 }
                                                                         };
      for (auto const& [base, exponent]: base_exponent) {
        if constexpr (std::is_same_v<TypeParam,float>) {
          EXPECT_FLOAT_EQ(lbt::cem::pow(base, exponent), std::pow(base, exponent));
        } else if constexpr (std::is_same_v<TypeParam,double>) {
          EXPECT_DOUBLE_EQ(lbt::cem::pow(base, exponent), std::pow(base, exponent));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
      }
    }

  }
}

#endif // LBT_CEM_POW_UNITTEST
