#ifndef LBT_VTK_CONTINUUM_UNITTEST
#define LBT_VTK_CONTINUUM_UNITTEST
#pragma once

/**
 * \file     vtk_continuum_unittest.hpp
 * \mainpage Tests for continuum class based on VTK library
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <ostream>
#include <string>
#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

#if __has_include (<vtkSmartPointer.h>)
  #include "../../src/continuum/vtk_continuum.hpp"

  template <typename T>
  class VtkContinuumTest : public ::testing::Test {
    public:
      VtkContinuumTest(std::int32_t const NX = 7, std::int32_t const NY = 9, std::int32_t const NZ = 11, 
                       std::filesystem::path const& output_path = "") noexcept
        : NX{NX}, NY{NX}, NZ{NZ}, output_path{output_path}, vtk_continuum{NX, NY, NZ, output_path} {
        return;
      }
    protected:
      std::int32_t NX;
      std::int32_t NY;
      std::int32_t NZ;
      std::filesystem::path output_path;
      lbt::VtkContinuum<T> vtk_continuum;
  };
  using VtkContinuumDataTypes = ::testing::Types<double,float>;
  TYPED_TEST_CASE(VtkContinuumTest, VtkContinuumDataTypes);

  TYPED_TEST(VtkContinuumTest, setAndGetPressure) {
    std::int64_t i {0};
    for (std::int32_t x = 0; x < this->NX; ++x) {
      for (std::int32_t y = 0; y < this->NY; ++y) {
        for (std::int32_t z = 0; z < this->NZ; ++z) {
          this->vtk_continuum.setP(x,y,z,static_cast<TypeParam>(i));
          ++i;
        }
      }
    }

    i = 0;
    for (std::int32_t x = 0; x < this->NX; ++x) {
      for (std::int32_t y = 0; y < this->NY; ++y) {
        for (std::int32_t z = 0; z < this->NZ; ++z) {
          if constexpr (std::is_same_v<TypeParam, double>) {
            EXPECT_DOUBLE_EQ(this->vtk_continuum.getP(x,y,z), static_cast<double>(i));
          } else if constexpr (std::is_same_v<TypeParam, float>) {
            EXPECT_FLOAT_EQ(this->vtk_continuum.getP(x,y,z), static_cast<float>(i));
          } else {
            GTEST_SKIP() << "Test not implemented for given data type!";
          }
          ++i;
        }
      }
    }
  }
  TYPED_TEST(VtkContinuumTest, setAndGetVelocityX) {
    std::int64_t i {0};
    for (std::int32_t x = 0; x < this->NX; ++x) {
      for (std::int32_t y = 0; y < this->NY; ++y) {
        for (std::int32_t z = 0; z < this->NZ; ++z) {
          this->vtk_continuum.setU(x,y,z,static_cast<TypeParam>(i));
          ++i;
        }
      }
    }

    i = 0;
    for (std::int32_t x = 0; x < this->NX; ++x) {
      for (std::int32_t y = 0; y < this->NY; ++y) {
        for (std::int32_t z = 0; z < this->NZ; ++z) {
          if constexpr (std::is_same_v<TypeParam, double>) {
            EXPECT_DOUBLE_EQ(this->vtk_continuum.getU(x,y,z), static_cast<double>(i));
          } else if constexpr (std::is_same_v<TypeParam, float>) {
            EXPECT_FLOAT_EQ(this->vtk_continuum.getU(x,y,z), static_cast<float>(i));
          } else {
            GTEST_SKIP() << "Test not implemented for given data type!";
          }
          ++i;
        }
      }
    }
  }
  TYPED_TEST(VtkContinuumTest, setAndGetVelocityY) {
    std::int64_t i {0};
    for (std::int32_t x = 0; x < this->NX; ++x) {
      for (std::int32_t y = 0; y < this->NY; ++y) {
        for (std::int32_t z = 0; z < this->NZ; ++z) {
          this->vtk_continuum.setV(x,y,z,static_cast<TypeParam>(i));
          ++i;
        }
      }
    }

    i = 0;
    for (std::int32_t x = 0; x < this->NX; ++x) {
      for (std::int32_t y = 0; y < this->NY; ++y) {
        for (std::int32_t z = 0; z < this->NZ; ++z) {
          if constexpr (std::is_same_v<TypeParam, double>) {
            EXPECT_DOUBLE_EQ(this->vtk_continuum.getV(x,y,z), static_cast<double>(i));
          } else if constexpr (std::is_same_v<TypeParam, float>) {
            EXPECT_FLOAT_EQ(this->vtk_continuum.getV(x,y,z), static_cast<float>(i));
          } else {
            GTEST_SKIP() << "Test not implemented for given data type!";
          }
          ++i;
        }
      }
    }
  }
  TYPED_TEST(VtkContinuumTest, setAndGetVelocityZ) {
    std::int64_t i {0};
    for (std::int32_t x = 0; x < this->NX; ++x) {
      for (std::int32_t y = 0; y < this->NY; ++y) {
        for (std::int32_t z = 0; z < this->NZ; ++z) {
          this->vtk_continuum.setW(x,y,z,static_cast<TypeParam>(i));
          ++i;
        }
      }
    }

    i = 0;
    for (std::int32_t x = 0; x < this->NX; ++x) {
      for (std::int32_t y = 0; y < this->NY; ++y) {
        for (std::int32_t z = 0; z < this->NZ; ++z) {
          if constexpr (std::is_same_v<TypeParam, double>) {
            EXPECT_DOUBLE_EQ(this->vtk_continuum.getW(x,y,z), static_cast<double>(i));
          } else if constexpr (std::is_same_v<TypeParam, float>) {
            EXPECT_FLOAT_EQ(this->vtk_continuum.getW(x,y,z), static_cast<float>(i));
          } else {
            GTEST_SKIP() << "Test not implemented for given data type!";
          }
          ++i;
        }
      }
    }
  }
  TYPED_TEST(VtkContinuumTest, exportToMhd) {
    GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
  }
  TYPED_TEST(VtkContinuumTest, exportToVtk) {
    GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
  }
#endif

#endif // LBT_VTK_CONTINUUM_UNITTEST
