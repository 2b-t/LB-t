/**
 * \file     kinematic_viscosity_literals_test.hpp
 * \mainpage Contains unit-tests for literals for a kinematic viscosity
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_KINEMATIC_VISCOSITY_LITERALS_UNITTEST
#define LBT_UNIT_KINEMATIC_VISCOSITY_LITERALS_UNITTEST
#pragma once

#include <utility>

#include <gtest/gtest.h>

#include "unit/detail/kinematic_viscosity.hpp"
#include "unit/detail/kinematic_viscosity_literals.hpp"
#include "unit_literals_helper.hpp"


namespace lbt {
  namespace literals {
    namespace test {
      using namespace lbt::literals;

      using KinematicViscosityLiteralsHelper = UnitLiteralsHelper<lbt::unit::KinematicViscosity>;

      TEST_P(KinematicViscosityLiteralsHelper, unitConversion) {
        auto const [kinematic_viscosity, expected_result] = GetParam();
        EXPECT_DOUBLE_EQ(kinematic_viscosity.get(), expected_result);
      }

      INSTANTIATE_TEST_SUITE_P(KinematicViscosityLiteralsTest, KinematicViscosityLiteralsHelper, ::testing::Values(
          std::make_pair(7.4_m2ps, 7.4L),
          std::make_pair(5.6_St,   5.6e-4L),
          std::make_pair(3.5_cSt,  3.5e-6L)
        )
      );

    }
  }
}

#endif // LBT_UNIT_KINEMATIC_VISCOSITY_LITERALS_UNITTEST
