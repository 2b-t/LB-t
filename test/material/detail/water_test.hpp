/**
 * \file     water_test.hpp
 * \mainpage Contains unit-tests for physical properties of water
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_WATER_UNITTEST
#define LBT_MATERIAL_WATER_UNITTEST
#pragma once

#include <tuple>

#include <gtest/gtest.h>

#include "../../../src/material/detail/water.hpp"
#include "../../../src/unit/literals.hpp"
#include "../../../src/unit/units.hpp"


namespace lbt {
  namespace test {
    namespace material {
        using namespace lbt::literals;
        using State = std::tuple<lbt::unit::Pressure, lbt::unit::Temperature, lbt::unit::Density, lbt::unit::DynamicViscosity, lbt::unit::KinematicViscosity>;

        class WaterTestHelper : public ::testing::Test, public ::testing::WithParamInterface<State> {
        };

        TEST_P(WaterTestHelper, densityFromPressure) {
          [[maybe_unused]] auto const [pressure, temperature, expected_density, dynamic_viscosity, kinematic_viscosity] = GetParam();
          auto const density {lbt::material::Water::equationOfState(temperature, pressure)};
          EXPECT_NEAR(density.get(), expected_density.get(), (3.0_kg/1.0_m3).get());
        }

        // TODO: Inverse seems to suffer from severe rounding errors!
        /*
        TEST_P(WaterTestHelper, pressureFromDensity) {
          [[maybe_unused]] auto const [expected_pressure, temperature, density, dynamic_viscosity, kinematic_viscosity] = GetParam();
          auto const pressure {lbt::material::Water::equationOfState(density, temperature)};
          EXPECT_NEAR(pressure.get(), expected_pressure.get(), (0.03_atm).get());
        }
        */

        TEST_P(WaterTestHelper, dynamicViscosityFromTemperature) {
          [[maybe_unused]] auto const [pressure, temperature, density, expected_dynamic_viscosity, kinematic_viscosity] = GetParam();
          auto const dynamic_viscosity {lbt::material::Water::dynamicViscosity(temperature)};
          EXPECT_NEAR(dynamic_viscosity.get(), expected_dynamic_viscosity.get(), (0.03_mPas).get());
        }

        TEST_P(WaterTestHelper, kinematicViscosityFromTemperatureAndDensity) {
          [[maybe_unused]] auto const [pressure, temperature, density, dynamic_viscosity, expected_kinematic_viscosity] = GetParam();
          auto const kinematic_viscosity {lbt::material::Water::kinematicViscosity(density, temperature)};
          EXPECT_NEAR(kinematic_viscosity.get(), expected_kinematic_viscosity.get(), (0.03_cSt).get());
        }

        INSTANTIATE_TEST_SUITE_P(WaterTest, WaterTestHelper, ::testing::Values(
          std::make_tuple(1.0_atm,   0.0_deg, 999.84_kg/1.0_m3, 1.793_mPas,  1.787_cSt),
          std::make_tuple(1.0_atm,  20.0_deg, 998.21_kg/1.0_m3, 1.002_mPas,  1.004_cSt),
          std::make_tuple(1.0_atm,  40.0_deg, 992.22_kg/1.0_m3, 0.6532_mPas, 0.658_cSt),
          std::make_tuple(1.0_atm,  60.0_deg, 983.20_kg/1.0_m3, 0.4665_mPas, 0.475_cSt),
          std::make_tuple(1.0_atm,  80.0_deg, 971.82_kg/1.0_m3, 0.3544_mPas, 0.365_cSt),
          std::make_tuple(1.0_atm, 100.0_deg, 958.40_kg/1.0_m3, 0.2818_mPas, 0.294_cSt)
        ));
    }
  }
}

#endif // LBT_MATERIAL_WATER_UNITTEST
