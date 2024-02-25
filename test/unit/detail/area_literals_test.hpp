/**
 * \file     area_literals_test.hpp
 * \mainpage Contains unit-tests for literals for an area
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_AREA_LITERALS_UNITTEST
#define LBT_UNIT_AREA_LITERALS_UNITTEST
#pragma once

#include <utility>

#include <gtest/gtest.h>

#include "lbt/unit/detail/area.hpp"
#include "lbt/unit/detail/area_literals.hpp"
#include "unit_literals_helper.hpp"


namespace lbt {
  namespace literals {
    namespace test {
      using namespace lbt::literals;

      using AreaLiteralsHelper = UnitLiteralsHelper<lbt::unit::Area>;

      TEST_P(AreaLiteralsHelper, unitConversion) {
        auto const [area, expected_result] = GetParam();
        EXPECT_DOUBLE_EQ(area.get(), expected_result);
      }

      INSTANTIATE_TEST_SUITE_P(AreaLiteralsTest, AreaLiteralsHelper, ::testing::Values(
          std::make_pair(0.1_km2, 1.0e+5L),
          std::make_pair(2.9_m2,  2.9L),
          std::make_pair(7.4_cm2, 7.4e-4L),
          std::make_pair(6.6_mm2, 6.6e-6L)
        )
      );

    }
  }
}

#endif // LBT_UNIT_AREA_LITERALS_UNITTEST
