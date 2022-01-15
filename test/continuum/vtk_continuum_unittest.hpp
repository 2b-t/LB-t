#ifndef LBT_VTK_CONTINUUM_UNITTEST
#define LBT_VTK_CONTINUUM_UNITTEST
#pragma once

/**
 * \file     vtk_continuum_unittest.hpp
 * \mainpage Tests for continuum class based on VTK library
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <filesystem>
#include <ostream>
#include <string>
#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

#include "../../src/general/tuple_utilities.hpp"
#include "../testing_utilities/testing_utilities.hpp"
#include "continuum_unittest.hpp"

#if __has_include (<vtkSmartPointer.h>)
  #include "../../src/continuum/vtk_continuum.hpp"

  namespace lbt {
    namespace test {
      template <typename C>
      class VtkContinuumTest: public ContinuumTest<C>, public ::testing::Test {
        public:
          VtkContinuumTest(std::int32_t const NX = 7, std::int32_t const NY = 9, std::int32_t const NZ = 11, 
                           std::filesystem::path const& output_path = "") noexcept
            : ContinuumTest<C>{NX, NY, NZ, output_path} {
            return;
          }
      };

      template <typename T>
      using VtkContinuumTuple = std::tuple<lbt::VtkContinuum<T>>;
      using VtkContinuumDataTypes = std::tuple<float,double>;
      using VtkContinuumTypes = ToTestingTypes_t<lbt::CartesianProductApply_t<VtkContinuumTuple, VtkContinuumDataTypes>>;
      TYPED_TEST_CASE(VtkContinuumTest, VtkContinuumTypes);

      TYPED_TEST(VtkContinuumTest, setAndGetPressure) {
        EXPECT_TRUE(this->testSetAndGetPressure());
      }
      TYPED_TEST(VtkContinuumTest, setAndGetVelocityX) {
        EXPECT_TRUE(this->testSetAndGetVelocityX());
      }
      TYPED_TEST(VtkContinuumTest, setAndGetVelocityY) {
        EXPECT_TRUE(this->testSetAndGetVelocityY());
      }
      TYPED_TEST(VtkContinuumTest, setAndGetVelocityZ) {
        EXPECT_TRUE(this->testSetAndGetVelocityZ());
      }

      TYPED_TEST(VtkContinuumTest, exportToMhd) {
        GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
      }
      TYPED_TEST(VtkContinuumTest, exportToVtk) {
        GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
      }
    }
  }

#endif

#endif // LBT_VTK_CONTINUUM_UNITTEST
