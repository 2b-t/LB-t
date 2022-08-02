/**
 * \file     oxygen_test.hpp
 * \mainpage Contains unit-tests for physical properties of oxygen
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_OXYGEN_UNITTEST
#define LBT_MATERIAL_OXYGEN_UNITTEST
#pragma once

#include <gtest/gtest.h>

#include "../../../src/material/detail/oxygen.hpp"


namespace lbt {
  namespace test {
    namespace material {

      using namespace lbt::literals;
      using State = std::tuple<lbt::unit::Pressure, lbt::unit::Temperature, lbt::unit::Density, lbt::unit::DynamicViscosity, lbt::unit::KinematicViscosity>;

      class OxygenTestHelper : public ::testing::Test, public ::testing::WithParamInterface<State> {
      };

      TEST_P(OxygenTestHelper, densityFromTemperatureAndPressure) {
        [[maybe_unused]] auto const [pressure, temperature, expected_density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const density {lbt::material::Oxygen::equationOfState(temperature, pressure)};
        EXPECT_NEAR(density.get(), expected_density.get(), (0.05_kg/1.0_m3).get());
      }

      TEST_P(OxygenTestHelper, temperatureFromDensityAndPressure) {
        [[maybe_unused]] auto const [pressure, expected_temperature, density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const temperature {lbt::material::Oxygen::equationOfState(density, pressure)};
        EXPECT_NEAR(temperature.get(), expected_temperature.get(), (2.0_K).get());
      }

      TEST_P(OxygenTestHelper, pressureFromDensityAndTemperature) {
        [[maybe_unused]] auto const [expected_pressure, temperature, density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const pressure {lbt::material::Oxygen::equationOfState(density, temperature)};
        EXPECT_NEAR(pressure.get(), expected_pressure.get(), (0.01_bar).get());
      }

      TEST_P(OxygenTestHelper, dynamicViscosityFromTemperature) {
        [[maybe_unused]] auto const [pressure, temperature, density, expected_dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const dynamic_viscosity {lbt::material::Oxygen::dynamicViscosity(temperature)};
        EXPECT_NEAR(dynamic_viscosity.get(), expected_dynamic_viscosity.get(), (0.5_uPas).get());
      }

      TEST_P(OxygenTestHelper, kinematicViscosityFromTemperatureAndPressure) {
        [[maybe_unused]] auto const [pressure, temperature, density, dynamic_viscosity, expected_kinematic_viscosity] = GetParam();
        auto const kinematic_viscosity {lbt::material::Oxygen::kinematicViscosity(temperature, pressure)};
        EXPECT_NEAR(kinematic_viscosity.get(), expected_kinematic_viscosity.get(), (0.5_cSt).get());
      }

      TEST_P(OxygenTestHelper, kinematicViscosityFromDensityAndTemperature) {
        [[maybe_unused]] auto const [pressure, temperature, density, dynamic_viscosity, expected_kinematic_viscosity] = GetParam();
        auto const kinematic_viscosity {lbt::material::Oxygen::kinematicViscosity(density, temperature)};
        EXPECT_NEAR(kinematic_viscosity.get(), expected_kinematic_viscosity.get(), (0.5_cSt).get());
      }

      // Material values taken from https://www.engineeringtoolbox.com/gases-absolute-dynamic-viscosity-d_1888.html
      // and https://www.engineeringtoolbox.com/oxygen-O2-density-specific-weight-temperature-pressure-d_2082.html
      INSTANTIATE_TEST_SUITE_P(OxygenTest, OxygenTestHelper, ::testing::Values(
        std::make_tuple(1.0_atm,   0.0_deg, 1.411_kg/1.0_m3, 19.5_uPas, 13.82_cSt),
        std::make_tuple(1.0_atm,  20.0_deg, 1.314_kg/1.0_m3, 20.4_uPas, 15.5251_cSt),
        std::make_tuple(1.0_atm,  50.0_deg, 1.19_kg/1.0_m3,  21.8_uPas, 18.3193_cSt),
        std::make_tuple(1.0_atm, 100.0_deg, 1.031_kg/1.0_m3, 24.4_uPas, 23.6663_cSt)
      ));
    }

  }
}

#endif // LBT_MATERIAL_OXYGEN_UNITTEST
