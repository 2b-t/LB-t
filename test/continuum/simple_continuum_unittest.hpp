#ifndef LBT_SIMPLE_CONTINUUM_UNITTEST
#define LBT_SIMPLE_CONTINUUM_UNITTEST
#pragma once

/**
 * \file     simple_continuum_unittest.hpp
 * \mainpage Tests for continuum class
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <ostream>
#include <string>
#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

#include "../../src/continuum/simple_continuum.hpp"

template <typename T>
class SimpleContinuumTest : public ::testing::Test {
  public:
    SimpleContinuumTest(std::int32_t const NX = 7, std::int32_t const NY = 9, std::int32_t const NZ = 11, 
                      std::filesystem::path const& output_path = "") noexcept
      : NX{NX}, NY{NX}, NZ{NZ}, output_path{output_path}, simple_continuum{NX, NY, NZ, output_path} {
      return;
    }
  protected:
    std::int32_t NX;
    std::int32_t NY;
    std::int32_t NZ;
    std::filesystem::path output_path;
    lbt::SimpleContinuum<T> simple_continuum;
};
using SimpleContinuumDataTypes = ::testing::Types<double,float>;
TYPED_TEST_CASE(SimpleContinuumTest, SimpleContinuumDataTypes);

TYPED_TEST(SimpleContinuumTest, setAndGetPressure) {
  std::int64_t i {0};
  for (std::int32_t x = 0; x < this->NX; ++x) {
    for (std::int32_t y = 0; y < this->NY; ++y) {
      for (std::int32_t z = 0; z < this->NZ; ++z) {
        this->simple_continuum.setP(x,y,z,static_cast<TypeParam>(i));
        ++i;
      }
    }
  }

  i = 0;
  for (std::int32_t x = 0; x < this->NX; ++x) {
    for (std::int32_t y = 0; y < this->NY; ++y) {
      for (std::int32_t z = 0; z < this->NZ; ++z) {
        if constexpr (std::is_same_v<TypeParam, double>) {
          EXPECT_DOUBLE_EQ(this->simple_continuum.getP(x,y,z), static_cast<double>(i));
        } else if constexpr (std::is_same_v<TypeParam, float>) {
          EXPECT_FLOAT_EQ(this->simple_continuum.getP(x,y,z), static_cast<float>(i));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
        ++i;
      }
    }
  }
}
TYPED_TEST(SimpleContinuumTest, setAndGetVelocityX) {
  std::int64_t i {0};
  for (std::int32_t x = 0; x < this->NX; ++x) {
    for (std::int32_t y = 0; y < this->NY; ++y) {
      for (std::int32_t z = 0; z < this->NZ; ++z) {
        this->simple_continuum.setU(x,y,z,static_cast<TypeParam>(i));
        ++i;
      }
    }
  }

  i = 0;
  for (std::int32_t x = 0; x < this->NX; ++x) {
    for (std::int32_t y = 0; y < this->NY; ++y) {
      for (std::int32_t z = 0; z < this->NZ; ++z) {
        if constexpr (std::is_same_v<TypeParam, double>) {
          EXPECT_DOUBLE_EQ(this->simple_continuum.getU(x,y,z), static_cast<double>(i));
        } else if constexpr (std::is_same_v<TypeParam, float>) {
          EXPECT_FLOAT_EQ(this->simple_continuum.getU(x,y,z), static_cast<float>(i));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
        ++i;
      }
    }
  }
}
TYPED_TEST(SimpleContinuumTest, setAndGetVelocityY) {
  std::int64_t i {0};
  for (std::int32_t x = 0; x < this->NX; ++x) {
    for (std::int32_t y = 0; y < this->NY; ++y) {
      for (std::int32_t z = 0; z < this->NZ; ++z) {
        this->simple_continuum.setV(x,y,z,static_cast<TypeParam>(i));
        ++i;
      }
    }
  }

  i = 0;
  for (std::int32_t x = 0; x < this->NX; ++x) {
    for (std::int32_t y = 0; y < this->NY; ++y) {
      for (std::int32_t z = 0; z < this->NZ; ++z) {
        if constexpr (std::is_same_v<TypeParam, double>) {
          EXPECT_DOUBLE_EQ(this->simple_continuum.getV(x,y,z), static_cast<double>(i));
        } else if constexpr (std::is_same_v<TypeParam, float>) {
          EXPECT_FLOAT_EQ(this->simple_continuum.getV(x,y,z), static_cast<float>(i));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
        ++i;
      }
    }
  }
}
TYPED_TEST(SimpleContinuumTest, setAndGetVelocityZ) {
  std::int64_t i {0};
  for (std::int32_t x = 0; x < this->NX; ++x) {
    for (std::int32_t y = 0; y < this->NY; ++y) {
      for (std::int32_t z = 0; z < this->NZ; ++z) {
        this->simple_continuum.setW(x,y,z,static_cast<TypeParam>(i));
        ++i;
      }
    }
  }

  i = 0;
  for (std::int32_t x = 0; x < this->NX; ++x) {
    for (std::int32_t y = 0; y < this->NY; ++y) {
      for (std::int32_t z = 0; z < this->NZ; ++z) {
        if constexpr (std::is_same_v<TypeParam, double>) {
          EXPECT_DOUBLE_EQ(this->simple_continuum.getW(x,y,z), static_cast<double>(i));
        } else if constexpr (std::is_same_v<TypeParam, float>) {
          EXPECT_FLOAT_EQ(this->simple_continuum.getW(x,y,z), static_cast<float>(i));
        } else {
          GTEST_SKIP() << "Test not implemented for given data type!";
        }
        ++i;
      }
    }
  }
}
TYPED_TEST(SimpleContinuumTest, exportToMhd) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}
TYPED_TEST(SimpleContinuumTest, exportToVtk) {
  GTEST_SKIP() << "Unit tests involving file import and export not implemented yet!";
}

#endif // LBT_SIMPLE_CONTINUUM_UNITTEST
