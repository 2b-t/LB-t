/**
 * \file     air_test.hpp
 * \mainpage Contains unit-tests for physical properties of air
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_AIR_UNITTEST
#define LBT_MATERIAL_AIR_UNITTEST
#pragma once

#include <gtest/gtest.h>

#include "material/detail/air.hpp"


namespace lbt {
  namespace test {
    namespace material {

      using namespace lbt::literals;
      using State = std::tuple<lbt::unit::Pressure, lbt::unit::Temperature, lbt::unit::Density, lbt::unit::DynamicViscosity, lbt::unit::KinematicViscosity>;

      class AirTestHelper : public ::testing::Test, public ::testing::WithParamInterface<State> {
      };

      TEST_P(AirTestHelper, densityFromTemperatureAndPressure) {
        [[maybe_unused]] auto const [pressure, temperature, expected_density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const density {lbt::material::Air::equationOfState(temperature, pressure)};
        EXPECT_NEAR(density.get(), expected_density.get(), (0.01_kg/1.0_m3).get());
      }

      TEST_P(AirTestHelper, temperatureFromDensityAndPressure) {
        [[maybe_unused]] auto const [pressure, expected_temperature, density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const temperature {lbt::material::Air::equationOfState(density, pressure)};
        EXPECT_NEAR(temperature.get(), expected_temperature.get(), (1.0_K).get());
      }

      TEST_P(AirTestHelper, pressureFromDensityAndTemperature) {
        [[maybe_unused]] auto const [expected_pressure, temperature, density, dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const pressure {lbt::material::Air::equationOfState(density, temperature)};
        EXPECT_NEAR(pressure.get(), expected_pressure.get(), (0.01_bar).get());
      }

      TEST_P(AirTestHelper, dynamicViscosityFromTemperature) {
        [[maybe_unused]] auto const [pressure, temperature, density, expected_dynamic_viscosity, kinematic_viscosity] = GetParam();
        auto const dynamic_viscosity {lbt::material::Air::dynamicViscosity(temperature)};
        EXPECT_NEAR(dynamic_viscosity.get(), expected_dynamic_viscosity.get(), (0.001_mPas).get());
      }

      TEST_P(AirTestHelper, kinematicViscosityFromTemperatureAndPressure) {
        [[maybe_unused]] auto const [pressure, temperature, density, dynamic_viscosity, expected_kinematic_viscosity] = GetParam();
        auto const kinematic_viscosity {lbt::material::Air::kinematicViscosity(temperature, pressure)};
        EXPECT_NEAR(kinematic_viscosity.get(), expected_kinematic_viscosity.get(), (0.5_cSt).get());
      }

      TEST_P(AirTestHelper, kinematicViscosityFromDensityAndTemperature) {
        [[maybe_unused]] auto const [pressure, temperature, density, dynamic_viscosity, expected_kinematic_viscosity] = GetParam();
        auto const kinematic_viscosity {lbt::material::Air::kinematicViscosity(density, temperature)};
        EXPECT_NEAR(kinematic_viscosity.get(), expected_kinematic_viscosity.get(), (0.5_cSt).get());
      }

      // Material values taken from https://www.engineersedge.com/calculators/air-density.htm
      // and https://www.engineeringtoolbox.com/air-absolute-kinematic-viscosity-d_601.html
      INSTANTIATE_TEST_SUITE_P(AirTest, AirTestHelper, ::testing::Values(
        std::make_tuple(1.0_atm,   0.0_deg, 1.293_kg/1.0_m3,  0.01715_mPas, 13.28_cSt),
        std::make_tuple(1.0_atm,  20.0_deg, 1.204_kg/1.0_m3,  0.01813_mPas, 15.06_cSt),
        std::make_tuple(1.0_atm,  40.0_deg, 1.127_kg/1.0_m3,  0.01907_mPas, 16.92_cSt),
        std::make_tuple(1.0_atm,  60.0_deg, 1.060_kg/1.0_m3,  0.01999_mPas, 18.86_cSt),
        std::make_tuple(1.0_atm,  80.0_deg, 0.9996_kg/1.0_m3, 0.02088_mPas, 20.88_cSt),
        std::make_tuple(1.0_atm, 100.0_deg, 0.9461_kg/1.0_m3, 0.02174_mPas, 22.97_cSt)
      ));
    }

  }
}

#endif // LBT_MATERIAL_AIR_UNITTEST
