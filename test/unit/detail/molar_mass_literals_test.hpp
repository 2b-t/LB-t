/**
 * \file     molar_mass_literals_test.hpp
 * \mainpage Contains unit-tests for literals for a molar mass
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_MOLAR_MASS_LITERALS_UNITTEST
#define LBT_UNIT_MOLAR_MASS_LITERALS_UNITTEST
#pragma once

#include <utility>

#include <gtest/gtest.h>

#include "lbt/unit/detail/molar_mass.hpp"
#include "lbt/unit/detail/molar_mass_literals.hpp"
#include "unit_literals_helper.hpp"


namespace lbt {
  namespace literals {
    namespace test {
      using namespace lbt::literals;

      using MolarMassLiteralsHelper = UnitLiteralsHelper<lbt::unit::MolarMass>;

      TEST_P(MolarMassLiteralsHelper, unitConversion) {
        auto const [molar_mass, expected_result] = GetParam();
        EXPECT_DOUBLE_EQ(molar_mass.get(), expected_result);
      }

      INSTANTIATE_TEST_SUITE_P(MolarMassLiteralsTest, MolarMassLiteralsHelper, ::testing::Values(
          std::make_pair(9.9_gpmol,  9.9e-3L),
          std::make_pair(0.7_kgpmol, 7.0e-1L)
        )
      );

    }
  }
}

#endif // LBT_UNIT_MOLAR_MASS_LITERALS_UNITTEST
