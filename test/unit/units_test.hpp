#ifndef LBT_UNITS_UNITTEST
#define LBT_UNITS_UNITTEST
#pragma once

/**\file     units_test.hpp
 * \mainpage Contains unit-tests for different physical units
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <type_traits>

#include <gtest/gtest.h>

#include "../../src/unit/units.hpp"


namespace lbt {
  namespace test {
    /// Test custom is_unit type trait
    TEST(IsUnitTraitTest, lengthIsUnit) {
      constexpr bool is_unit = lbt::unit::is_unit_v<lbt::unit::Length>;
      EXPECT_TRUE(is_unit);
    }

    TEST(IsUnitTraitTest, doubleIsNotUnit) {
      constexpr bool is_unit = lbt::unit::is_unit_v<double>;
      EXPECT_FALSE(is_unit);
    }

    using UnitTestingTypes = ::testing::Types<lbt::unit::Length, lbt::unit::Distance,
                                              lbt::unit::Time, lbt::unit::Duration,
                                              lbt::unit::Mass, lbt::unit::Area, lbt::unit::Volume,
                                              lbt::unit::Velocity, lbt::unit::Density, lbt::unit::KinematicViscosity, 
                                              lbt::unit::Temperature, lbt::unit::Pressure>;

    /**\class PhysicalUnitTest
     * \brief Helper class for the tests of the lbt::unit::detail::UnitBase class
     * 
     * \tparam T   The type of physical unit (e.g. lbt::unit::Length)
     * \tparam Dummy parameter used for SFINAE
    */
    template <typename T, typename std::enable_if_t<lbt::unit::is_unit_v<T>>* = nullptr>
    struct PhysicalUnitTest: public ::testing::Test {
    };

    /// Tests for standard operations involving operations between physical units of the same type
    TYPED_TEST_SUITE(PhysicalUnitTest, UnitTestingTypes);

    TYPED_TEST(PhysicalUnitTest, constructorAndGet) {
      constexpr long double value {7.0};
      TypeParam const unit {value};
      EXPECT_DOUBLE_EQ(unit.get(), value);
    }

    TYPED_TEST(PhysicalUnitTest, setAndGet) {
      constexpr long double value {7.0};
      TypeParam unit {};
      unit.set(value);
      EXPECT_DOUBLE_EQ(unit.get(), value);
    }

    TYPED_TEST(PhysicalUnitTest, addition) {
      TypeParam const a {1.0};
      TypeParam const b {2.0};
      long double const expected_result {a.get() + b.get()};
      TypeParam const res = a + b;
      EXPECT_DOUBLE_EQ(res.get(), expected_result);
    }

    TYPED_TEST(PhysicalUnitTest, subtraction) {
      TypeParam const a {1.0};
      TypeParam const b {2.0};
      long double const expected_result {a.get() - b.get()};
      TypeParam const res = a - b;
      EXPECT_DOUBLE_EQ(res.get(), expected_result);
    }

    TYPED_TEST(PhysicalUnitTest, multiplyConstant) {
      TypeParam const a {1.0};
      long double const c {2.0};
      long double const expected_result {c*a.get()};
      TypeParam const res = c*a;
      EXPECT_DOUBLE_EQ(res.get(), expected_result);
    }

    TYPED_TEST(PhysicalUnitTest, divideConstant) {
      TypeParam const a {1.0};
      long double const c {2.0};
      long double const expected_result {a.get()/c};
      TypeParam const res = a/c;
      EXPECT_DOUBLE_EQ(res.get(), expected_result);
    }

    /// Tests involving operations between different physical units resulting in composed units
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

  }
}

#endif // LBT_UNITS_UNITTEST
