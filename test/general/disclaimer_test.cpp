/**
 * \file     disclaimer_test.cpp
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
