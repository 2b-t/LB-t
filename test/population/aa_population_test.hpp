/**
 * \file     aa_population_test.hpp
 * \mainpage Tests for a population with A-A access pattern
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_AA_POPULATION_UNITTEST
#define LBT_AA_POPULATION_UNITTEST
#pragma once

#include <cstdint>
#include <limits>
#include <utility>

#include <gtest/gtest.h>

#include "../../src/population/aa_population.hpp"
#include "../lattice/lattice_testing_types.hpp"
#include "population_base_test.hpp"


namespace lbt {
  namespace test {

    template <typename LT, std::int32_t NP = 1>
    class AaPopulationTest;

    template <class LT, std::int32_t NP = 1>
    class AaPopulation: public lbt::AaPopulation<LT,NP> {
      public:
        using lbt::AaPopulation<LT,NP>::AaPopulation;
        friend class AaPopulationTest<LT,NP>;
    };

    template <typename LT, std::int32_t NP>
    class AaPopulationTest: public PopulationTest<AaPopulation,LT,NP> {
      public:
        bool testPopulationSize() noexcept override {
          if (this->population.A.size() == this-> expected_size) {
            return true;
          }
          return false;
        }
    };
    TYPED_TEST_SUITE(AaPopulationTest, LatticeTestTypes);

    TYPED_TEST(AaPopulationTest, arraySize) {
      EXPECT_TRUE(this->testPopulationSize());
    }
    TYPED_TEST(AaPopulationTest, writeEvenReadOdd) {
      bool const is_success = this->template testPopulationReadWrite<lbt::Timestep::Even>();
      EXPECT_TRUE(is_success);
    }
    TYPED_TEST(AaPopulationTest, writeOddReadEven) {
      bool const is_success = this->template testPopulationReadWrite<lbt::Timestep::Odd>();
      EXPECT_TRUE(is_success);
    }

  }
}

#endif // LBT_AA_POPULATION_UNITTEST
