#ifndef LBT_BOUNDARY_ORIENTATION_UNITTEST
#define LBT_BOUNDARY_ORIENTATION_UNITTEST
#pragma once

/**
 * \file     orientation_test.hpp
 * \mainpage Tests for orientation structs for boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <utility>

#include <gtest/gtest.h>

#include "../../../src/population/boundary/orientation.hpp"


namespace lbt {
  namespace test {

    TEST(BoundaryOrientationTest, negation) {
      EXPECT_EQ(!lbt::boundary::Orientation::Left,   lbt::boundary::Orientation::Right);
      EXPECT_EQ(!lbt::boundary::Orientation::Right,  lbt::boundary::Orientation::Left);
      EXPECT_EQ(!lbt::boundary::Orientation::Front,  lbt::boundary::Orientation::Back);
      EXPECT_EQ(!lbt::boundary::Orientation::Back,   lbt::boundary::Orientation::Front);
      EXPECT_EQ(!lbt::boundary::Orientation::Bottom, lbt::boundary::Orientation::Top);
      EXPECT_EQ(!lbt::boundary::Orientation::Top,    lbt::boundary::Orientation::Bottom);
    }

  }
}

#endif // LBT_BOUNDARY_ORIENTATION_UNITTEST
