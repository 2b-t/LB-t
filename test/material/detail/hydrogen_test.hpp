/**
 * \file     hydrogen_test.hpp
 * \mainpage Contains unit-tests for physical properties of hydrogen
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_HYDROGEN_UNITTEST
#define LBT_MATERIAL_HYDROGEN_UNITTEST
#pragma once

#include <gtest/gtest.h>

#include "material/detail/hydrogen.hpp"


namespace lbt {
  namespace test {
    namespace material {

      using namespace lbt::literals;
      using State = std::tuple<lbt::unit::Pressure, lbt::unit::Temperature, lbt::unit::Density, lbt::unit::DynamicViscosity, lbt::unit::KinematicViscosity>;

      class HydrogenTestHelper : public ::testing::Test, public ::testing::WithParamInterface<State> {
      };

      TEST_P(HydrogenTestHelper, densityFromTemperatureAndPressure) {
        [[maybe_unused]] auto const [pressure, temperature, expected_density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const density {lbt::material::Hydrogen::equationOfState(temperature, pressure)};
        EXPECT_NEAR(density.get(), expected_density.get(), (0.002_kg/1.0_m3).get());
      }

      TEST_P(HydrogenTestHelper, temperatureFromDensityAndPressure) {
        [[maybe_unused]] auto const [pressure, expected_temperature, density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const temperature {lbt::material::Hydrogen::equationOfState(density, pressure)};
        EXPECT_NEAR(temperature.get(), expected_temperature.get(), (6.0_K).get());
      }

      TEST_P(HydrogenTestHelper, pressureFromDensityAndTemperature) {
        [[maybe_unused]] auto const [expected_pressure, temperature, density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const pressure {lbt::material::Hydrogen::equationOfState(density, temperature)};
        EXPECT_NEAR(pressure.get(), expected_pressure.get(), (0.02_bar).get());
      }

      TEST_P(HydrogenTestHelper, dynamicViscosityFromTemperature) {
        [[maybe_unused]] auto const [pressure, temperature, density, expected_dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const dynamic_viscosity {lbt::material::Hydrogen::dynamicViscosity(temperature)};
        EXPECT_NEAR(dynamic_viscosity.get(), expected_dynamic_viscosity.get(), (0.1_uPas).get());
      }

      TEST_P(HydrogenTestHelper, kinematicViscosityFromTemperatureAndPressure) {
        [[maybe_unused]] auto const [pressure, temperature, density, dynamic_viscosity, expected_kinematic_viscosity] = GetParam();
        auto const kinematic_viscosity {lbt::material::Hydrogen::kinematicViscosity(temperature, pressure)};
        EXPECT_NEAR(kinematic_viscosity.get(), expected_kinematic_viscosity.get(), (5.0_cSt).get());
      }

      TEST_P(HydrogenTestHelper, kinematicViscosityFromDensityAndTemperature) {
        [[maybe_unused]] auto const [pressure, temperature, density, dynamic_viscosity, expected_kinematic_viscosity] = GetParam();
        auto const kinematic_viscosity {lbt::material::Hydrogen::kinematicViscosity(density, temperature)};
        EXPECT_NEAR(kinematic_viscosity.get(), expected_kinematic_viscosity.get(), (5.0_cSt).get());
      }

      // Material values taken from https://www.engineeringtoolbox.com/gases-absolute-dynamic-viscosity-d_1888.html
      // and https://www.engineeringtoolbox.com/hydrogen-H2-density-specific-weight-temperature-pressure-d_2044.html
      INSTANTIATE_TEST_SUITE_P(HydrogenTest, HydrogenTestHelper, ::testing::Values(
        std::make_tuple(1.0_atm,   0.0_deg, 0.0888_kg/1.0_m3,  8.4_uPas,  94.5946_cSt),
        std::make_tuple(1.0_atm,  20.0_deg, 0.0827_kg/1.0_m3,  8.8_uPas, 106.4087_cSt),
        std::make_tuple(1.0_atm,  50.0_deg, 0.0749_kg/1.0_m3,  9.4_uPas, 125.50_cSt),
        std::make_tuple(1.0_atm, 100.0_deg, 0.0649_kg/1.0_m3, 10.4_uPas, 160.2465_cSt)
      ));
    }

  }
}

#endif // LBT_MATERIAL_HYDROGEN_UNITTEST
