/**
 * \file     states_table_test.hpp
 * \mainpage Contains unit-tests for physical properties of fluids with tables containing thermodynamic states
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_STATES_TABLE_UNITTEST
#define LBT_MATERIAL_STATES_TABLE_UNITTEST
#pragma once

#include <cmath>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "../../../src/material/detail/states_table.hpp"


namespace lbt {
  namespace test {
    namespace material {
      using namespace lbt::literals;

      // Ficticious material with uniform density
      class UniformMaterial: public lbt::material::StatesTable<4> {
        public:
          static constexpr lbt::unit::Density equationOfState(lbt::unit::Pressure const p, lbt::unit::Temperature const t) noexcept {
            return interpolate<lbt::unit::Density>(p, t, UniformMaterial::states_);
          }

        protected:
          UniformMaterial() = default;
          UniformMaterial(UniformMaterial const&) = default;
          UniformMaterial& operator= (UniformMaterial const&) = default;
          UniformMaterial(UniformMaterial&&) = default;
          UniformMaterial& operator= (UniformMaterial&&) = default;

          static constexpr lbt::material::States<4> states_ = { lbt::material::State{33.14_Pa, 31.46_K, lbt::unit::Density{7.12L}},
                                                                lbt::material::State{16.81_Pa, 30.97_K, lbt::unit::Density{7.12L}},
                                                                lbt::material::State{12.09_Pa, 15.67_K, lbt::unit::Density{7.12L}},
                                                                lbt::material::State{37.93_Pa, 18.21_K, lbt::unit::Density{7.12L}} };
      };

      TEST(StatesTableTest, uniformMaterialIsUnityInside) {
        constexpr lbt::unit::Density expected_density {7.12L};
        // Inside of quadrilateral (determined manually)
        constexpr auto pressure_limits {std::make_pair<lbt::unit::Pressure, lbt::unit::Pressure>(16.81_Pa, 33.14_Pa)};
        constexpr auto temperature_limits {std::make_pair<lbt::unit::Temperature, lbt::unit::Temperature>(18.21_K, 30.97_K)};

        constexpr int number_of_tests_per_dimension {5};
        for (int i = 0; i < number_of_tests_per_dimension; ++i) {
          lbt::unit::Pressure const pressure {pressure_limits.first + (pressure_limits.second - pressure_limits.first)*
                                              static_cast<long double>(i+1)/(number_of_tests_per_dimension+1)};
          for (int j = 0; j < number_of_tests_per_dimension; ++j) {
            lbt::unit::Temperature const temperature {temperature_limits.first + (temperature_limits.second - temperature_limits.first)*
                                                      static_cast<long double>(j+1)/(number_of_tests_per_dimension+1)};
            auto const density = UniformMaterial::equationOfState(pressure, temperature);
            EXPECT_EQ(density.get(), expected_density.get());
          }
        }
      }

      TEST(StatesTableTest, uniformMaterialIsNanOutside) {
        // Random points outside of quadrilateral
        std::vector<lbt::unit::Pressure> const pressures = {2.65_Pa, 16.80_Pa, 33.15_Pa, 54.56_Pa};
        std::vector<lbt::unit::Temperature> const temperatures = {12.10_K, 18.20_K, 30.98_K, 41.83_K};

        for (auto const& p: pressures) {
          for (auto const& t: temperatures) {
            auto const density = UniformMaterial::equationOfState(p, t);
            EXPECT_TRUE(std::isnan(density.get()));
          }
        }
      }

      // Ficticious material with linear gradient in x-direction
      class LinearMaterialX: public lbt::material::StatesTable<4> {
        public:
          static constexpr lbt::unit::Density equationOfState(lbt::unit::Pressure const p, lbt::unit::Temperature const t) noexcept {
            return interpolate<lbt::unit::Density>(p, t, LinearMaterialX::states_);
          }

        protected:
          LinearMaterialX() = default;
          LinearMaterialX(LinearMaterialX const&) = default;
          LinearMaterialX& operator= (LinearMaterialX const&) = default;
          LinearMaterialX(LinearMaterialX&&) = default;
          LinearMaterialX& operator= (LinearMaterialX&&) = default;

          static constexpr lbt::material::States<4> states_ = { lbt::material::State{25.34_Pa, 112.32_K, lbt::unit::Density{8.89518623291L}},
                                                                lbt::material::State{16.63_Pa,  92.76_K, lbt::unit::Density{3.93857614333L}},
                                                                lbt::material::State{15.42_Pa,  42.53_K, lbt::unit::Density{3.25L}},
                                                                lbt::material::State{36.63_Pa,  56.74_K, lbt::unit::Density{15.32L}} };
      };

      TEST(LinearMaterialX, linearMaterialXIsIndependentOfY) {
        // Inside of quadrilateral (determined manually)
        constexpr auto pressure_limits {std::make_pair<lbt::unit::Pressure, lbt::unit::Pressure>(16.63_Pa, 25.34_Pa)};
        constexpr auto temperature_limits {std::make_pair<lbt::unit::Temperature, lbt::unit::Temperature>(56.74_K, 92.76_K)};

        constexpr int number_of_tests_per_dimension {5};
        std::vector<lbt::unit::Density> densities_along_isotherm {};
        for (int i = 0; i < number_of_tests_per_dimension; ++i) {
          std::vector<lbt::unit::Density> densities_along_isobar {};
          lbt::unit::Pressure const pressure {pressure_limits.first + (pressure_limits.second - pressure_limits.first)*
                                              static_cast<long double>(i+1)/(number_of_tests_per_dimension+1)};
          for (int j = 0; j < number_of_tests_per_dimension; ++j) {
            lbt::unit::Temperature const temperature {temperature_limits.first + (temperature_limits.second - temperature_limits.first)*
                                                      static_cast<long double>(j+1)/(number_of_tests_per_dimension+1)};
            densities_along_isobar.push_back(LinearMaterialX::equationOfState(pressure, temperature));
          }
          auto const expected_density_on_isobar = densities_along_isobar.at(0);
          for (std::size_t j = 1; j < densities_along_isobar.size(); ++j) {
            EXPECT_NEAR(densities_along_isobar.at(j).get(), expected_density_on_isobar.get(), 0.01);
          }
          densities_along_isotherm.push_back(expected_density_on_isobar);
        }

        lbt::unit::Density expected_lower_bound_density {3.93857614333L};
        for (auto const& d: densities_along_isotherm) {
          EXPECT_GT(d.get(), expected_lower_bound_density.get());
          expected_lower_bound_density = d;
        }
        EXPECT_GT(lbt::unit::Density{8.89518623291L}.get(),  expected_lower_bound_density.get());
      }

      // Ficticious material with linear gradient in y-direction
      class LinearMaterialY: public lbt::material::StatesTable<4> {
        public:
          static constexpr lbt::unit::Density equationOfState(lbt::unit::Pressure const p, lbt::unit::Temperature const t) noexcept {
            return interpolate<lbt::unit::Density>(p, t, LinearMaterialY::states_);
          }

        protected:
          LinearMaterialY() = default;
          LinearMaterialY(LinearMaterialY const&) = default;
          LinearMaterialY& operator= (LinearMaterialY const&) = default;
          LinearMaterialY(LinearMaterialY&&) = default;
          LinearMaterialY& operator= (LinearMaterialY&&) = default;

          static constexpr lbt::material::States<4> states_ = { lbt::material::State{112.32_Pa, 25.34_K, lbt::unit::Density{8.89518623291L}},
                                                                lbt::material::State{ 92.76_Pa, 16.63_K, lbt::unit::Density{3.93857614333L}},
                                                                lbt::material::State{ 42.53_Pa, 15.42_K, lbt::unit::Density{3.25L}},
                                                                lbt::material::State{ 56.74_Pa, 36.63_K, lbt::unit::Density{15.32L}} };
      };

      TEST(LinearMaterialY, linearMaterialYIsIndependentOfX) {
        // Inside of quadrilateral (determined manually)
        constexpr auto pressure_limits {std::make_pair<lbt::unit::Pressure, lbt::unit::Pressure>(56.74_Pa, 92.76_Pa)};
        constexpr auto temperature_limits {std::make_pair<lbt::unit::Temperature, lbt::unit::Temperature>(16.63_K, 25.34_K)};

        constexpr int number_of_tests_per_dimension {5};
        std::vector<lbt::unit::Density> densities_along_isobar {};
        for (int j = 0; j < number_of_tests_per_dimension; ++j) {
          std::vector<lbt::unit::Density> densities_along_isotherm {};
          lbt::unit::Temperature const temperature {temperature_limits.first + (temperature_limits.second - temperature_limits.first)*
                                                    static_cast<long double>(j+1)/(number_of_tests_per_dimension+1)};
          for (int i = 0; i < number_of_tests_per_dimension; ++i) {
            lbt::unit::Pressure const pressure {pressure_limits.first + (pressure_limits.second - pressure_limits.first)*
                                                static_cast<long double>(i+1)/(number_of_tests_per_dimension+1)};
            densities_along_isotherm.push_back(LinearMaterialY::equationOfState(pressure, temperature));
          }

          auto const expected_density_on_isotherm = densities_along_isotherm.at(0);
          for (std::size_t i = 1; i < densities_along_isotherm.size(); ++i) {
            EXPECT_NEAR(densities_along_isotherm.at(i).get(), expected_density_on_isotherm.get(), 0.01);
          }
          densities_along_isobar.push_back(expected_density_on_isotherm);
        }

        lbt::unit::Density expected_lower_bound_density {3.93857614333L};
        for (auto const& d: densities_along_isobar) {
          EXPECT_GT(d.get(), expected_lower_bound_density.get());
          expected_lower_bound_density = d;
        }
        EXPECT_GT(lbt::unit::Density{8.89518623291L}.get(),  expected_lower_bound_density.get());
      }

      // Ficticious axis-aligned material
      class AxisAlignedMaterial: public lbt::material::StatesTable<4> {
        public:
          static constexpr lbt::unit::Density equationOfState(lbt::unit::Pressure const p, lbt::unit::Temperature const t) noexcept {
            return interpolate<lbt::unit::Density>(p, t, AxisAlignedMaterial::states_);
          }

        protected:
          AxisAlignedMaterial() = default;
          AxisAlignedMaterial(AxisAlignedMaterial const&) = default;
          AxisAlignedMaterial& operator= (AxisAlignedMaterial const&) = default;
          AxisAlignedMaterial(AxisAlignedMaterial&&) = default;
          AxisAlignedMaterial& operator= (AxisAlignedMaterial&&) = default;

          static constexpr lbt::material::States<4> states_ = { lbt::material::State{23.54_Pa, 215.36_K, lbt::unit::Density{3.53L}},
                                                                lbt::material::State{23.54_Pa,  95.73_K, lbt::unit::Density{2.46L}},
                                                                lbt::material::State{37.43_Pa,  95.73_K, lbt::unit::Density{17.42L}},
                                                                lbt::material::State{37.43_Pa, 215.36_K, lbt::unit::Density{18.49L}} };
      };

      TEST(AxisAlignedMaterial, equalToLinearInterpolation) {
        // Inside of quadrilateral (determined manually)
        constexpr auto pressure_limits {std::make_pair<lbt::unit::Pressure, lbt::unit::Pressure>(23.54_Pa, 37.43_Pa)};
        constexpr auto temperature_limits {std::make_pair<lbt::unit::Temperature, lbt::unit::Temperature>(95.73_K, 215.36_K)};

        constexpr int number_of_tests_per_dimension {5};
        for (int i = 0; i < number_of_tests_per_dimension; ++i) {
          lbt::unit::Pressure const pressure {pressure_limits.first + (pressure_limits.second - pressure_limits.first)*
                                              static_cast<long double>(i+1)/(number_of_tests_per_dimension+1)};
          for (int j = 0; j < number_of_tests_per_dimension; ++j) {
            lbt::unit::Temperature const temperature {temperature_limits.first + (temperature_limits.second - temperature_limits.first)*
                                                      static_cast<long double>(j+1)/(number_of_tests_per_dimension+1)};
            auto const density {AxisAlignedMaterial::equationOfState(pressure, temperature)};
            auto const expected_density {lbt::unit::Density{2.46L} + 
                                         (lbt::unit::Density{17.42L} - lbt::unit::Density{2.46L})*((pressure - pressure_limits.first)/(pressure_limits.second - pressure_limits.first)) +
                                         (lbt::unit::Density{3.53L} - lbt::unit::Density{2.46L})*((temperature - temperature_limits.first)/(temperature_limits.second - temperature_limits.first))};
            EXPECT_NEAR(density.get(), expected_density.get(), 0.01);
          }
        }
      }
    }
  }
}

#endif // LBT_MATERIAL_STATES_TABLE_UNITTEST
