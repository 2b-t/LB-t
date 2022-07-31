/**
 * \file     operators_test.hpp
 * \mainpage Contains unit-tests for operators between different physical units
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_OPERATORS_UNITTEST
#define LBT_UNIT_OPERATORS_UNITTEST
#pragma once

#include <gtest/gtest.h>

#include "../../../src/unit/detail/operators.hpp"


namespace lbt {
  namespace test {

    TEST(AreaTest, lengthMultipliedByLengthIsArea) {
      lbt::unit::Length const a_length {2.0};
      lbt::unit::Length const another_length {3.5};
      long double const expected_result {a_length.get()*another_length.get()};
      lbt::unit::Area const area {a_length*another_length};
      EXPECT_DOUBLE_EQ(area.get(), expected_result);
    }

    TEST(VolumeTest, lengthMultipliedByAreaIsVolume) {
      lbt::unit::Length const length {2.0};
      lbt::unit::Area const area {3.5};
      long double const expected_result {length.get()*area.get()};
      lbt::unit::Volume const volume {length*area};
      EXPECT_DOUBLE_EQ(volume.get(), expected_result);
    }

    TEST(VolumeTest, areaMultipliedByLengthIsVolume) {
      lbt::unit::Area const area {3.5};
      lbt::unit::Length const length {2.0};
      long double const expected_result {area.get()*length.get()};
      lbt::unit::Volume const volume {area*length};
      EXPECT_DOUBLE_EQ(volume.get(), expected_result);
    }

    TEST(VelocityTest, lengthDividedByTimeIsVelocity) {
      lbt::unit::Length const length {2.0};
      lbt::unit::Time const time {3.5};
      long double const expected_result {length.get()/time.get()};
      lbt::unit::Velocity const velocity {length/time};
      EXPECT_DOUBLE_EQ(velocity.get(), expected_result);
    }

    TEST(DensityTest, massDividedByVolumeIsDensity) {
      lbt::unit::Mass const mass {2.0};
      lbt::unit::Volume const volume {3.5};
      long double const expected_result {mass.get()/volume.get()};
      lbt::unit::Density const density {mass/volume};
      EXPECT_DOUBLE_EQ(density.get(), expected_result);
    }

    TEST(KinematicViscosityTest, areaDividedByTimeIsKinematicViscosity) {
      lbt::unit::Area const area {2.0};
      lbt::unit::Time const time {3.5};
      long double const expected_result {area.get()/time.get()};
      lbt::unit::KinematicViscosity const kinematic_viscosity {area/time};
      EXPECT_DOUBLE_EQ(kinematic_viscosity.get(), expected_result);
    }

    TEST(KinematicViscosityTest, velocityMultipliedByLengthIsKinematicViscosity) {
      lbt::unit::Velocity const velocity {2.0};
      lbt::unit::Length const length {3.5};
      long double const expected_result {velocity.get()*length.get()};
      lbt::unit::KinematicViscosity const kinematic_viscosity {velocity*length};
      EXPECT_DOUBLE_EQ(kinematic_viscosity.get(), expected_result);
    }

    TEST(KinematicViscosityTest, dynamicViscosityDividedByDensityIsKinematicViscosity) {
      lbt::unit::DynamicViscosity const dynamic_viscosity {2.0};
      lbt::unit::Density const density {3.5};
      long double const expected_result {dynamic_viscosity.get()/density.get()};
      lbt::unit::KinematicViscosity const kinematic_viscosity {dynamic_viscosity/density};
      EXPECT_DOUBLE_EQ(kinematic_viscosity.get(), expected_result);
    }

    TEST(DynamicViscosityTest, pressureMultipliedByTimeIsDynamicViscosity) {
      lbt::unit::Pressure const pressure {2.0};
      lbt::unit::Time const time {3.5};
      long double const expected_result {pressure.get()*time.get()};
      lbt::unit::DynamicViscosity const dynamic_viscosity {pressure*time};
      EXPECT_DOUBLE_EQ(dynamic_viscosity.get(), expected_result);
    }

    TEST(DynamicViscosityTest, kinematicViscosityMultipliedByDensityIsDynamicViscosity) {
      lbt::unit::KinematicViscosity const kinematic_viscosity {2.0};
      lbt::unit::Density const density {3.5};
      long double const expected_result {kinematic_viscosity.get()*density.get()};
      lbt::unit::DynamicViscosity const dynamic_viscosity {kinematic_viscosity*density};
      EXPECT_DOUBLE_EQ(dynamic_viscosity.get(), expected_result);
    }

    TEST(MolarMassTest, massDividedByAmountOfSubstanceIsMolarMass) {
      lbt::unit::Mass const mass {2.0};
      lbt::unit::AmountOfSubstance const amount_of_substance {3.5};
      long double const expected_result {mass.get()/amount_of_substance.get()};
      lbt::unit::MolarMass const molar_mass {mass/amount_of_substance};
      EXPECT_DOUBLE_EQ(molar_mass.get(), expected_result);
    }

  }
}

#endif // LBT_UNIT_OPERATORS_UNITTEST
