#ifndef LBT_UNIT_LITERALS_UNITTEST
#define LBT_UNIT_LITERALS_UNITTEST
#pragma once

/**\file     unit_literals_unittest.hpp
 * \mainpage Contains unit-tests for different physical units and their literals
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <gtest/gtest.h>

#include "../../src/general/unit_literals.hpp"


namespace lbt {
  namespace test {
    TEST(SomeTest, someName) {
      using namespace lbt::literals;
      constexpr auto t = 1.0_s;
      constexpr auto d = 3.0_mm;
      constexpr auto v = 1.0_kg / (3.0_mm*1.0_m2);

      auto a = 2.0*1.0_s;

      EXPECT_TRUE(false) << "Velocity: " << a;
    }
  }
}

#endif // LBT_UNIT_LITERALS_UNITTEST
