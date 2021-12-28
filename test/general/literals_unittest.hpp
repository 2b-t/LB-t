#ifndef LBT_LITERALS_UNITTEST
#define LBT_LITERALS_UNITTEST
#pragma once

/**\file     literals_unittest.hpp
 * \mainpage Contains unit-tests for literals of different physical units
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <gtest/gtest.h>

#include "../../src/general/literals.hpp"


namespace lbt {
  namespace test {

    // Test user-defined literals (test if correct and test if same)
    TEST(LengthLiterals, kilometer) {
      using namespace lbt::literals;

      EXPECT_TRUE(false);
    }

  }
}

#endif // LBT_LITERALS_UNITTEST
