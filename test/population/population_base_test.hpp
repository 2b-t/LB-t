/**
 * \file     population_base_test.hpp
 * \mainpage Tests for a population with arbitrary access patterns and underlying memory layout
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_POPULATION_BASE_UNITTEST
#define LBT_POPULATION_BASE_UNITTEST
#pragma once

#include <cstdint>
#include <limits>
#include <utility>

#include <gtest/gtest.h>

#include "lbt/constexpr_math/constexpr_math.hpp"
#include "../lattice/lattice_testing_types.hpp"


namespace lbt {
  namespace test {

    template <template <typename, std::int32_t> typename P, typename LT, std::int32_t NP = 1>
    class PopulationTest: public ::testing::Test {
      public:
        PopulationTest(std::int32_t const NX_ = 7, std::int32_t const NY_ = 9, std::int32_t const NZ_ = 13) noexcept
          : NX{NX_}, NY{NY_}, NZ{LT::DIM != 2 ? NZ_ : 1}, population{NX,NY,NZ}, expected_size{static_cast<std::int64_t>(NZ)*NY*NX*NP*LT::ND} {
          return;
        }

        template <lbt::Timestep TS>
        bool testPopulationReadWrite() noexcept {
          using T = typename LT::type;
          
          std::vector<T> indices {};
          indices.reserve(expected_size);
          std::int64_t i = 0;
          for (std::int32_t z = 0; z < this->NZ; ++z) {
            for (std::int32_t y = 0; y < this->NY; ++y) {
              for (std::int32_t x = 0; x < this->NX; ++x) {
                for(std::int32_t n = 0; n <= 1; ++n) {
                  for(std::int32_t d = 0; d < LT::OFF; ++d) {
                    indices.push_back(i);
                    this->population.template write<TS>(x,y,z,n,d,0) = static_cast<T>(i);
                    ++i;
                  }
                }
              }
            }
          }

          bool is_success {true};
          i = 0;
          for (std::int32_t z = 0; z < this->NZ; ++z) {
            for (std::int32_t y = 0; y < this->NY; ++y) {
              for (std::int32_t x = 0; x < this->NX; ++x) {
                for(std::int32_t n = 0; n <= 1; ++n) {
                  for(std::int32_t d = 0; d < LT::OFF; ++d) {
                    auto const expected_result = indices.at(i);
                    T const result = this->population.template read<!TS>(x,y,z,n,d,0);
                    is_success &= lbt::cem::isAlmostEqualEpsAbs(result, static_cast<T>(expected_result));
                    ++i;
                  }
                }
              }
            }
          }

          return is_success;
        }

        virtual bool testPopulationSize() noexcept = 0;
      protected:
        std::int32_t NX;
        std::int32_t NY;
        std::int32_t NZ;
        P<LT,NP> population;
        std::int64_t expected_size;
    };

  }
}

#endif // LBT_POPULATION_BASE_UNITTEST
