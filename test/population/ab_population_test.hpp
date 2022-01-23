#ifndef LBT_AB_POPULATION_UNITTEST
#define LBT_AB_POPULATION_UNITTEST
#pragma once

/**
 * \file     ab_population_test.hpp
 * \mainpage Tests for a population with A-B access pattern
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <cstdint>
#include <limits>
#include <utility>

#include <gtest/gtest.h>

#include "../../src/population/ab_population.hpp"
#include "../lattice/lattice_testing_types.hpp"
#include "population_base_test.hpp"


namespace lbt {
  namespace test {

    template <typename LT, std::int32_t NP = 1>
    class AbPopulationTest;
    
    template <class LT, std::int32_t NP = 1>
    class AbPopulation: public lbt::AbPopulation<LT,NP> {
      public:
        using lbt::AbPopulation<LT,NP>::AbPopulation;
        friend class AbPopulationTest<LT,NP>;
    };

    template <typename LT, std::int32_t NP>
    class AbPopulationTest: public PopulationTest<AbPopulation,LT,NP> {
      public:
       bool testPopulationSize() noexcept override {
          if ((this->population.A.size() == this-> expected_size) && 
              (this->population.B.size() == this-> expected_size)) {
            return true;
          }
          return false;
        }
    };
    TYPED_TEST_SUITE(AbPopulationTest, LatticeTestTypes);

    TYPED_TEST(AbPopulationTest, arraySize) {
      EXPECT_TRUE(this->testPopulationSize());
    }
    TYPED_TEST(AbPopulationTest, writeEvenReadOdd) {
      bool const is_success = this->template testPopulationReadWrite<lbt::Timestep::Even>();
      EXPECT_TRUE(is_success);
    }
    TYPED_TEST(AbPopulationTest, writeOddReadEven) {
      bool const is_success = this->template testPopulationReadWrite<lbt::Timestep::Odd>();
      EXPECT_TRUE(is_success);
    }

  }
}

#endif // LBT_AB_POPULATION_UNITTEST
