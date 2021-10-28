#ifndef LBT_CONTINUUM_UNITTEST
#define LBT_CONTINUUM_UNITTEST
#pragma once

/**
 * \file     continuum_unittest.hpp
 * \mainpage Tests for continuum class
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <string>

#include <gtest/gtest.h>

#include "../../src/continuum/continuum.hpp"
 

/// Tests conversion to string with optional precision
TEST(ToStringWithPrecisionTest, singleDigitToTwoDigitsDouble) {
  constexpr double val {0.1};
  constexpr std::int32_t digits {2};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_TRUE(str == "0.10");
}
TEST(ToStringWithPrecisionTest, threeDigitsToTwoDigitsDouble) {
  constexpr double val {0.123};
  constexpr std::int32_t digits {2};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_TRUE(str == "0.12");
}
TEST(ToStringWithPrecisionTest, singleDigitToThreeDigitsDouble) {
  constexpr double val {0.1};
  constexpr std::int32_t digits {3};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_TRUE(str == "0.100");
}
TEST(ToStringWithPrecisionTest, fourDigitsToThreeDigitsDouble) {
  constexpr double val {0.1234};
  constexpr std::int32_t digits {3};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_TRUE(str == "0.123");
}
TEST(ToStringWithPrecisionTest, singleDigitToTwoDigitsFloat) {
  constexpr float val {0.1f};
  constexpr std::int32_t digits {2};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_TRUE(str == "0.10");
}
TEST(ToStringWithPrecisionTest, noDigitsInteger) {
  constexpr std::int32_t val {1};
  constexpr std::int32_t digits {3};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_TRUE(str == "1");
}

// Test setters and getters of continuum class

// Test export to MHD and VTK

#endif // LBT_CONTINUUM_UNITTEST
