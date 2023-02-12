/**
 * \file     converter_test.hpp
 * \mainpage Tests for the unit converter between lattice-Boltzmann simulation units and physical units
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <gtest/gtest.h>

#include "converter.hpp"
#include "unit/literals.hpp"
#include "unit/units.hpp"


namespace lbt {
  namespace test {
    using namespace lbt::literals;

    /**\class ConverterTest
     * \brief Helper class for setting up a test for unit conversions between lattice-Boltzmann simulation units and physical units
    */
    class ConverterTest: public ::testing::Test {
      public:
        /**\fn        ConverterTest
         * \brief     (Default) constructor
         *
         * \param[in] a_length                  An arbitrary length in SI-units
         * \param[in] equivalent_lbm_length     The equivalent of the physical length in LBM units
         * \param[in] a_velocity                An arbitrary velocity in SI-units
         * \param[in] equivalent_lbm_velocity   The equivalent of the physical velocity in LBM units
         * \param[in] a_density                 An arbitrary density in SI-units
         * \param[in] equivalent_lbm_density    The equivalent of the physical density in LBM units
        */
        ConverterTest(lbt::unit::Length const a_length = 1.23_m, long double const equivalent_lbm_length = 456,
                      lbt::unit::Velocity const a_velocity = 3.14_mps, long double const equivalent_lbm_velocity = 0.01,
                      lbt::unit::Density const a_density = 2.9_kg/1.0_m3, long double const equivalent_lbm_density = 1.0) noexcept 
          : converter_{a_length, equivalent_lbm_length, a_velocity, equivalent_lbm_velocity, a_density, equivalent_lbm_density},
            a_length_{a_length}, equivalent_lbm_length_{equivalent_lbm_length}, a_velocity_{a_velocity}, equivalent_lbm_velocity_{equivalent_lbm_velocity},
            a_density_{a_density}, equivalent_lbm_density_{equivalent_lbm_density} {
          return;
        }
        ConverterTest(ConverterTest const&) = delete;
        ConverterTest(ConverterTest&&) = delete;
        ConverterTest& operator= (ConverterTest const&) = delete;
        ConverterTest& operator= (ConverterTest&&) = delete;
      protected:
        lbt::Converter converter_;
        lbt::unit::Length a_length_;
        long double equivalent_lbm_length_;
        lbt::unit::Velocity a_velocity_;
        long double equivalent_lbm_velocity_;
        lbt::unit::Density a_density_;
        long double equivalent_lbm_density_;
    };

    TEST_F(ConverterTest, lengthToLbm) {
      auto const converted_length {converter_.toLbm(a_length_)};
      EXPECT_DOUBLE_EQ(converted_length, equivalent_lbm_length_);
    }

    TEST_F(ConverterTest, lengthToPhysical) {
      auto const converted_length {converter_.toPhysical<lbt::unit::Length>(equivalent_lbm_length_)};
      EXPECT_DOUBLE_EQ(converted_length.get(), a_length_.get());
    }

    TEST_F(ConverterTest, velocityToLbm) {
      auto const converted_velocity {converter_.toLbm(a_velocity_)};
      EXPECT_DOUBLE_EQ(converted_velocity, equivalent_lbm_velocity_);
    }

    TEST_F(ConverterTest, velocityToPhysical) {
      auto const converted_velocity {converter_.toPhysical<lbt::unit::Velocity>(equivalent_lbm_velocity_)};
      EXPECT_DOUBLE_EQ(converted_velocity.get(), a_velocity_.get());
    }

    TEST_F(ConverterTest, densityToLbm) {
      auto const converted_density {converter_.toLbm(a_density_)};
      EXPECT_DOUBLE_EQ(converted_density, equivalent_lbm_density_);
    }

    TEST_F(ConverterTest, densityToPhysical) {
      auto const converted_density {converter_.toPhysical<lbt::unit::Density>(equivalent_lbm_density_)};
      EXPECT_DOUBLE_EQ(converted_density.get(), a_density_.get());
    }

    TEST_F(ConverterTest, lengthRoundtrip) {
      auto const length {3.1_m};
      auto const a {converter_.toLbm(length)};
      auto const b {converter_.toPhysical<lbt::unit::Length>(a)};
      EXPECT_DOUBLE_EQ(b.get(), length.get());
    }

    TEST_F(ConverterTest, velocityRoundtrip) {
      auto const velocity {9.3_mps};
      auto const a {converter_.toLbm(velocity)};
      auto const b {converter_.toPhysical<lbt::unit::Velocity>(a)};
      EXPECT_DOUBLE_EQ(b.get(), velocity.get());
    }

    TEST_F(ConverterTest, densityRoundtrip) {
      auto const density {1.293_kg/1.0_m3};
      auto const a {converter_.toLbm(density)};
      auto const b {converter_.toPhysical<lbt::unit::Density>(a)};
      EXPECT_DOUBLE_EQ(b.get(), density.get());
    }

    TEST_F(ConverterTest, timeRoundtrip) {
      auto const time {2.4_min};
      auto const a {converter_.toLbm(time)};
      auto const b {converter_.toPhysical<lbt::unit::Time>(a)};
      EXPECT_DOUBLE_EQ(b.get(), time.get());
    }

    TEST_F(ConverterTest, kinematicViscosityRoundtrip) {
      auto const kinematic_viscosity {13.28_cSt};
      auto const a {converter_.toLbm(kinematic_viscosity)};
      auto const b {converter_.toPhysical<lbt::unit::KinematicViscosity>(a)};
      EXPECT_DOUBLE_EQ(b.get(), kinematic_viscosity.get());
    }

    TEST_F(ConverterTest, pressureRoundtrip) {
      auto const pressure {1.1_atm};
      auto const a {converter_.toLbm(pressure)};
      auto const b {converter_.toPhysical<lbt::unit::Pressure>(a)};
      EXPECT_DOUBLE_EQ(b.get(), pressure.get());
    }

  }
}
