#ifndef LBT_UNITS_UNITTEST
#define LBT_UNITS_UNITTEST
#pragma once

/**\file     units_unittest.hpp
 * \mainpage Contains unit-tests for different physical units
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <gtest/gtest.h>

#include "../../src/general/units.hpp"


namespace lbt {
  namespace test {
    using UnitTestingTypes = ::testing::Types<lbt::unit::Length, lbt::unit::Distance,
                                              lbt::unit::Time, lbt::unit::Duration,
                                              lbt::unit::Mass, lbt::unit::Area, lbt::unit::Volume,
                                              lbt::unit::Velocity, lbt::unit::Density, lbt::unit::KinematicViscosity>;

    // Test basic operators for all data types

    // Test operators for composed units

    TEST(SomeTest, someName) {
      using namespace lbt::literals;
      constexpr auto t = 1.0_s;
      constexpr auto d = 3.0_mm;
      constexpr auto v = 1.0_kg / (3.0_mm*1.0_m2);
      constexpr lbt::unit::Density c {v};
      lbt::unit::Density const dd = 2.0;
      long double const ddd = dd.get();

      auto a = 2.0*1.0_s;

      EXPECT_TRUE(false) << "Velocity: " << a;
    }

  }
}

#endif // LBT_UNITS_UNITTEST
