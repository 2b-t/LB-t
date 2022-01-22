#ifndef LBT_BOUNDARY_NORMAL_UNITTEST
#define LBT_BOUNDARY_NORMAL_UNITTEST
#pragma once

/**
 * \file     normal_test.hpp
 * \mainpage Tests for normal structs for boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <cmath>
#include <utility>

#include <gtest/gtest.h>

#include "../../../src/population/boundary/normal.hpp"


namespace lbt {
  namespace test {

    /**\class OrientationWrapper
     * \brief Wrapper class for an enum so it can be used in a typed test
     * 
     * \tparam O   The orientation to be wrapped
    */
    template <lbt::boundary::Orientation O>
    class OrientationWrapper {
    };

    /**\class BoundaryNormalTest
     * \brief Test class for boundary orientation struct
     * 
     * \tparam T   The wrapper class of the corresponding lbt::boundary::Orientation
    */
    template <typename T>
    class BoundaryNormalTest: public ::testing::Test {
      protected:
        /**\fn     helper
         * \brief  Helper function for getting the value of the enum wrapped as a template argument by the wrapper class
         *
         * \tparam W   The wrapper class
         * \tparam O   The orientation wrapped inside the enum
        */
        template <template <lbt::boundary::Orientation> typename W, lbt::boundary::Orientation O>
        static constexpr lbt::boundary::Orientation helper(W<O> const&) noexcept {
          return O;
        }
      public:
        /**\fn     getBoundaryOrientation
         * \brief  Helper function for getting the value of the enum wrapped as a template argument by the wrapper class
         *
         * \tparam WO  A wrapper class of a boundary orientation
        */
        template <typename WO = T>
        static constexpr lbt::boundary::Orientation getBoundaryOrientation() noexcept {
          return helper(WO{});
        }
    };

    using BoundaryNormalTypes = ::testing::Types<OrientationWrapper<lbt::boundary::Orientation::Left>,
                                                 OrientationWrapper<lbt::boundary::Orientation::Right>, 
                                                 OrientationWrapper<lbt::boundary::Orientation::Front>, 
                                                 OrientationWrapper<lbt::boundary::Orientation::Back>,
                                                 OrientationWrapper<lbt::boundary::Orientation::Bottom>, 
                                                 OrientationWrapper<lbt::boundary::Orientation::Top>>;

    TYPED_TEST_SUITE(BoundaryNormalTest, BoundaryNormalTypes);

    TYPED_TEST(BoundaryNormalTest, normalised) {
      constexpr auto O = this->getBoundaryOrientation();

      double const length {std::sqrt(std::pow(lbt::boundary::Normal<O>::x,2) + 
                                     std::pow(lbt::boundary::Normal<O>::y,2) + 
                                     std::pow(lbt::boundary::Normal<O>::z,2))};
      EXPECT_DOUBLE_EQ(length, 1.0);
    }

    TYPED_TEST(BoundaryNormalTest, sumsToZero) {
      constexpr auto O = this->getBoundaryOrientation();
      constexpr auto I {!O};

      std::int32_t const dx {lbt::boundary::Normal<O>::x + lbt::boundary::Normal<I>::x};
      std::int32_t const dy {lbt::boundary::Normal<O>::y + lbt::boundary::Normal<I>::y};
      std::int32_t const dz {lbt::boundary::Normal<O>::z + lbt::boundary::Normal<I>::z};
      EXPECT_EQ(dx, 0);
      EXPECT_EQ(dy, 0);
      EXPECT_EQ(dz, 0);
    }

  }
}

#endif // LBT_BOUNDARY_NORMAL_UNITTEST
