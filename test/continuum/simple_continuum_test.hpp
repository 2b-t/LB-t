#ifndef LBT_SIMPLE_CONTINUUM_UNITTEST
#define LBT_SIMPLE_CONTINUUM_UNITTEST
#pragma once

/**
 * \file     simple_continuum_test.hpp
 * \mainpage Tests for continuum class
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <cstdint>
#include <filesystem>
#include <ostream>
#include <string>
#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

#include "../../src/continuum/simple_continuum.hpp"
#include "../../src/general/tuple_utilities.hpp"
#include "../testing_utilities/testing_utilities.hpp"
#include "continuum_base_test.hpp"


namespace lbt {
  namespace test {
    template <typename C>
    class SimpleContinuumTest: public ContinuumBaseTest<C>, public ::testing::Test {
      public:
        SimpleContinuumTest(std::int32_t const NX = 7, std::int32_t const NY = 9, std::int32_t const NZ = 11, 
                            std::filesystem::path const& output_path = "") noexcept
          : ContinuumBaseTest<C>{NX, NY, NZ, output_path} {
          return;
        }
    };
    
    template <typename T>
    using SimpleContinuumTuple = std::tuple<lbt::SimpleContinuum<T>>;
    using SimpleContinuumDataTypes = std::tuple<float,double>;
    using SimpleContinuumTypes = ToTestingTypes_t<lbt::CartesianProductApply_t<SimpleContinuumTuple, SimpleContinuumDataTypes>>;
    TYPED_TEST_CASE(SimpleContinuumTest, SimpleContinuumTypes);

    TYPED_TEST(SimpleContinuumTest, setAndGetPressure) {
      EXPECT_TRUE(this->testSetAndGetPressure());
    }
    TYPED_TEST(SimpleContinuumTest, setAndGetVelocityX) {
      EXPECT_TRUE(this->testSetAndGetVelocityX());
    }
    TYPED_TEST(SimpleContinuumTest, setAndGetVelocityY) {
      EXPECT_TRUE(this->testSetAndGetVelocityY());
    }
    TYPED_TEST(SimpleContinuumTest, setAndGetVelocityZ) {
      EXPECT_TRUE(this->testSetAndGetVelocityZ());
    }

    TYPED_TEST(SimpleContinuumTest, exportToVtk) {
      GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
    }
  }
}

#endif // LBT_SIMPLE_CONTINUUM_UNITTEST
