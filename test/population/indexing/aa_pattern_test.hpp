/**
 * \file     aa_pattern_test.hpp
 * \mainpage Tests for indexing of population via the one-population AA-access pattern
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_AA_PATTERN_UNITTEST
#define LBT_AA_PATTERN_UNITTEST
#pragma once

#include <cstdint>
#include <limits>
#include <utility>

#include <gtest/gtest.h>

#include "../../../src/general/type_definitions.hpp"
#include "../../../src/population/indexing/aa_pattern.hpp"
#include "../../lattice/lattice_testing_types.hpp"


namespace lbt {
  namespace test {

    template <typename LT, std::int32_t NP = 1>
    class AaPatternTest: public ::testing::Test {
      public:
        AaPatternTest(std::int32_t const NX = 7, std::int32_t const NY = 9, std::int32_t const NZ = 11) noexcept
          : NX{NX}, NY{NY}, NZ{NZ}, aa_pattern{NX,NY,NZ} {
          if constexpr (LT::DIM == 2) {
            this->NZ = 1;
          }
          return;
        }
      protected:
        std::int32_t NX;
        std::int32_t NY;
        std::int32_t NZ;
        lbt::AaPattern<LT,NP> aa_pattern;
    };
    TYPED_TEST_SUITE(AaPatternTest, LatticeTestTypes);

    TYPED_TEST(AaPatternTest, oddEvenIndex) {
      constexpr std::int32_t odd_index {1};
      constexpr std::int32_t even_index {2};
      auto const odd_result {this->aa_pattern.template oddEven<lbt::Timestep::Odd>(odd_index, even_index)};
      auto const even_result {this->aa_pattern.template oddEven<lbt::Timestep::Even>(odd_index, even_index)};
      EXPECT_EQ(odd_result, odd_index);
      EXPECT_EQ(even_result, even_index);
    }

    TYPED_TEST(AaPatternTest, indexReadIsIndexWrite) {
      for (std::int32_t z = 0; z < this->NZ; ++z) {
        for (std::int32_t y = 0; y < this->NY; ++y) {
          for (std::int32_t x = 0; x < this->NX; ++x) {
            for(std::int32_t n = 0; n <= 1; ++n) {
              for(std::int32_t d = 0; d < TypeParam::OFF; ++d) {
                {
                  auto const index_write {this->aa_pattern.template indexWrite<lbt::Timestep::Even>(x,y,z,n,d,0)};
                  auto const index_read {this->aa_pattern.template indexRead<lbt::Timestep::Odd>(x,y,z,n,d,0)};
                  EXPECT_EQ(index_read, index_write);
                }
                {
                  auto const index_write {this->aa_pattern.template indexWrite<lbt::Timestep::Odd>(x,y,z,n,d,0)};
                  auto const index_read {this->aa_pattern.template indexRead<lbt::Timestep::Even>(x,y,z,n,d,0)};
                  EXPECT_EQ(index_read, index_write);
                }
              }
            }
          }
        }
      }
    }

    TYPED_TEST(AaPatternTest, indexArrayReadIsIndexArrayWrite) {
      for (std::int32_t z = 0; z < this->NZ; ++z) {
        lbt::StackArray<std::int32_t,3> const z_n = {(this->NZ+z-1)%this->NZ, z, (z+1)%this->NZ};
        for (std::int32_t y = 0; y < this->NY; ++y) {
          lbt::StackArray<std::int32_t,3> const y_n = {(this->NY+y-1)%this->NY, y, (y+1)%this->NY};
          for (std::int32_t x = 0; x < this->NX; ++x) {
            lbt::StackArray<std::int32_t,3> const x_n = {(this->NX+x-1)%this->NX, x, (x+1)%this->NX};
            for(std::int32_t n = 0; n <= 1; ++n) {
              for(std::int32_t d = 0; d < TypeParam::OFF; ++d) {
                {
                  auto const index_write {this->aa_pattern.template indexWrite<lbt::Timestep::Even>(x_n,y_n,z_n,n,d,0)};
                  auto const index_read {this->aa_pattern.template indexRead<lbt::Timestep::Odd>(x_n,y_n,z_n,n,d,0)};
                  EXPECT_EQ(index_read, index_write);
                }
                {
                  auto const index_write {this->aa_pattern.template indexWrite<lbt::Timestep::Odd>(x_n,y_n,z_n,n,d,0)};
                  auto const index_read {this->aa_pattern.template indexRead<lbt::Timestep::Even>(x_n,y_n,z_n,n,d,0)};
                  EXPECT_EQ(index_read, index_write);
                }
              }
            }
          }
        }
      }
    }

    TYPED_TEST(AaPatternTest, indexIsIndexArray) {
      for (std::int32_t z = 0; z < this->NZ; ++z) {
        lbt::StackArray<std::int32_t,3> const z_n = {(this->NZ+z-1)%this->NZ, z, (z+1)%this->NZ};
        for (std::int32_t y = 0; y < this->NY; ++y) {
          lbt::StackArray<std::int32_t,3> const y_n = {(this->NY+y-1)%this->NY, y, (y+1)%this->NY};
          for (std::int32_t x = 0; x < this->NX; ++x) {
            lbt::StackArray<std::int32_t,3> const x_n = {(this->NX+x-1)%this->NX, x, (x+1)%this->NX};
            for(std::int32_t n = 0; n <= 1; ++n) {
              for(std::int32_t d = 0; d < TypeParam::OFF; ++d) {
                {
                  auto const index {this->aa_pattern.template indexRead<lbt::Timestep::Even>(x,y,z,n,d,0)};
                  auto const index_array {this->aa_pattern.template indexRead<lbt::Timestep::Even>(x_n,y_n,z_n,n,d,0)};
                  EXPECT_EQ(index, index_array);
                }
                {
                  auto const index {this->aa_pattern.template indexRead<lbt::Timestep::Odd>(x,y,z,n,d,0)};
                  auto const index_array {this->aa_pattern.template indexRead<lbt::Timestep::Odd>(x_n,y_n,z_n,n,d,0)};
                  EXPECT_EQ(index, index_array);
                }
                {
                  auto const index {this->aa_pattern.template indexWrite<lbt::Timestep::Even>(x,y,z,n,d,0)};
                  auto const index_array {this->aa_pattern.template indexWrite<lbt::Timestep::Even>(x_n,y_n,z_n,n,d,0)};
                  EXPECT_EQ(index, index_array);
                }
                {
                  auto const index {this->aa_pattern.template indexWrite<lbt::Timestep::Odd>(x,y,z,n,d,0)};
                  auto const index_array {this->aa_pattern.template indexWrite<lbt::Timestep::Odd>(x_n,y_n,z_n,n,d,0)};
                  EXPECT_EQ(index, index_array);
                }
              }
            }
          }
        }
      }
    }

  }
}

#endif // LBT_AA_PATTERN_UNITTEST
