#ifndef LBT_LITERALS_UNITTEST
#define LBT_LITERALS_UNITTEST
#pragma once

/**\file     literals_unittest.hpp
 * \mainpage Contains unit-tests for literals of different physical units
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <type_traits>

#include <gtest/gtest.h>

#include "../../src/general/literals.hpp"


namespace lbt {
  namespace test {

    // km, m, dm, cm, mm, um
    // h, min, s, ms, us
    // t, kg, g
    // km2, m2, cm2, mm2
    // km3, m3, cm3, mm3
    // kmph, mps, cmps, mmps
    // P, cP, St, cSt
    TEST(LengthLiterals, kilometer) {
      using namespace lbt::literals;

      auto const distance {2.0_km};

      //EXPECT_DOUBLE_EQ(false);
      EXPECT_TRUE(true);
    }

  }
}

#endif // LBT_LITERALS_UNITTEST
