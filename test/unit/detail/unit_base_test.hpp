/**
 * \file     unit_base_test.hpp
 * \mainpage Contains different tests for base unit class
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_UNIT_BASE_UNITTEST
#define LBT_UNIT_UNIT_BASE_UNITTEST
#pragma once

#include <type_traits>

#include <gtest/gtest.h>

#include "../../../src/unit/detail/unit_base.hpp"
#include "../../../src/unit/units.hpp"


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
                                              lbt::unit::Velocity, lbt::unit::Density, 
                                              lbt::unit::Temperature, lbt::unit::Pressure, 
                                              lbt::unit::KinematicViscosity, lbt::unit::DynamicViscosity,
                                              lbt::unit::MolarMass, lbt::unit::AmountOfSubstance>;

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

    TYPED_TEST(PhysicalUnitTest, division) {
      TypeParam const a {1.0};
      TypeParam const b {2.0};
      long double const expected_result {a.get() / b.get()};
      long double const res {a / b};
      EXPECT_DOUBLE_EQ(res, expected_result);
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

  }
}

#endif // LBT_UNIT_UNIT_BASE_UNITTEST
