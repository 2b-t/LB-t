#ifndef LBT_DISCLAIMER_UNITTEST
#define LBT_DISCLAIMER_UNITTEST
#pragma once

/**
 * \file     disclaimer_test.hpp
 * \mainpage Tests for printing the program's disclaimer
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <string>

#include <gtest/gtest.h>

#include "../../src/general/disclaimer.hpp"
 

namespace lbt {
  namespace test {

    /// Tests that is non-empty
    TEST(DisclaimerTest, nonEmpty) {
      EXPECT_FALSE(lbt::disclaimer().empty());
    }

  }
}

#endif // LBT_DISCLAIMER_UNITTEST
