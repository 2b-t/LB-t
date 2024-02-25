/**
 * \file     carbon_dioxide_test.hpp
 * \mainpage Contains unit-tests for physical properties of carbon dioxide
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_CARBON_DIOXIDE_UNITTEST
#define LBT_MATERIAL_CARBON_DIOXIDE_UNITTEST
#pragma once

#include <gtest/gtest.h>

#include "lbt/material/detail/carbon_dioxide.hpp"


namespace lbt {
  namespace test {
    namespace material {

      using namespace lbt::literals;
      using State = std::tuple<lbt::unit::Pressure, lbt::unit::Temperature, lbt::unit::Density, lbt::unit::DynamicViscosity, lbt::unit::KinematicViscosity>;

      class CarbonDioxideTestHelper : public ::testing::Test, public ::testing::WithParamInterface<State> {
      };

      TEST_P(CarbonDioxideTestHelper, densityFromTemperatureAndPressure) {
        [[maybe_unused]] auto const [pressure, temperature, expected_density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const density {lbt::material::CarbonDioxide::equationOfState(temperature, pressure)};
        EXPECT_NEAR(density.get(), expected_density.get(), (0.015_kg/1.0_m3).get());
      }

      TEST_P(CarbonDioxideTestHelper, temperatureFromDensityAndPressure) {
        [[maybe_unused]] auto const [pressure, expected_temperature, density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const temperature {lbt::material::CarbonDioxide::equationOfState(density, pressure)};
        EXPECT_NEAR(temperature.get(), expected_temperature.get(), (2.0_K).get());
      }

      TEST_P(CarbonDioxideTestHelper, pressureFromDensityAndTemperature) {
        [[maybe_unused]] auto const [expected_pressure, temperature, density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const pressure {lbt::material::CarbonDioxide::equationOfState(density, temperature)};
        EXPECT_NEAR(pressure.get(), expected_pressure.get(), (0.01_bar).get());
      }

      TEST_P(CarbonDioxideTestHelper, dynamicViscosityFromTemperature) {
        [[maybe_unused]] auto const [pressure, temperature, density, expected_dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const dynamic_viscosity {lbt::material::CarbonDioxide::dynamicViscosity(temperature)};
        EXPECT_NEAR(dynamic_viscosity.get(), expected_dynamic_viscosity.get(), (0.5_uPas).get());
      }

      TEST_P(CarbonDioxideTestHelper, kinematicViscosityFromTemperatureAndPressure) {
        [[maybe_unused]] auto const [pressure, temperature, density, dynamic_viscosity, expected_kinematic_viscosity] = GetParam();
        auto const kinematic_viscosity {lbt::material::CarbonDioxide::kinematicViscosity(temperature, pressure)};
        EXPECT_NEAR(kinematic_viscosity.get(), expected_kinematic_viscosity.get(), (0.5_cSt).get());
      }

      TEST_P(CarbonDioxideTestHelper, kinematicViscosityFromDensityAndTemperature) {
        [[maybe_unused]] auto const [pressure, temperature, density, dynamic_viscosity, expected_kinematic_viscosity] = GetParam();
        auto const kinematic_viscosity {lbt::material::CarbonDioxide::kinematicViscosity(density, temperature)};
        EXPECT_NEAR(kinematic_viscosity.get(), expected_kinematic_viscosity.get(), (0.5_cSt).get());
      }

      // Material values taken from https://www.engineeringtoolbox.com/carbon-dioxide-dynamic-kinematic-viscosity-temperature-pressure-d_2074.html
      // and https://www.engineeringtoolbox.com/carbon-dioxide-density-specific-weight-temperature-pressure-d_2018.html
      INSTANTIATE_TEST_SUITE_P(CarbonDioxideTest, CarbonDioxideTestHelper, ::testing::Values(
        std::make_tuple(1.0_bar,   6.9_deg, 1.902_kg/1.0_m3, 14.05_uPas,  7.387_cSt),
        std::make_tuple(1.0_bar,  26.9_deg, 1.773_kg/1.0_m3, 15.02_uPas,  8.472_cSt),
        std::make_tuple(1.0_bar,  46.9_deg, 1.661_kg/1.0_m3, 15.98_uPas,  9.621_cSt),
        std::make_tuple(1.0_bar,  66.9_deg, 1.562_kg/1.0_m3, 16.93_uPas, 10.84_cSt),
        std::make_tuple(1.0_bar,  86.9_deg, 1.474_kg/1.0_m3, 17.87_uPas, 12.12_cSt)
      ));
    }

  }
}

#endif // LBT_MATERIAL_CARBON_DIOXIDE_UNITTEST
