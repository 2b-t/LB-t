#ifndef LBT_LATTICE_UNITTEST
#define LBT_LATTICE_UNITTEST
#pragma once

/**
 * \file     lattice_unittest.hpp
 * \mainpage Tests for different discretisation lattices
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cmath>
#include <limits>
#include <numeric>
#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

#include "../../src/general/constexpr_math.hpp"
#include "../../src/general/type_definitions.hpp"
#include "../../src/lattice/D2Q9.hpp"
#include "../../src/lattice/D3Q15.hpp"
#include "../../src/lattice/D3Q19.hpp"
#include "../../src/lattice/D3Q27.hpp"
#include "../../src/general/tuple_utilities.hpp"
#include "../testing_utilities/testing_utilities.hpp"


namespace lbt {
  namespace test {

    /// The different lattices to be tested
    template <typename T> using LatticeTypes = std::tuple<lbt::lattice::D2Q9P10<T>,  lbt::lattice::D2Q9P12<T>, 
                                                          lbt::lattice::D3Q15P16<T>, lbt::lattice::D3Q19P20<T>, 
                                                          lbt::lattice::D3Q27P28<T>, lbt::lattice::D3Q27PC<T>>;
    /// The different floating data types for the lattices
    using LatticeFloatingTypes = std::tuple<double, float>;

    // Merge data types and lattices data types
    using LatticeTestTypes = ToTestingTypes_t<lbt::CartesianProductApply_t<LatticeTypes, LatticeFloatingTypes>>;

    /// Templated tests for each lattice
    template <typename L>
    struct DdQqTest: public ::testing::Test {
      using DdQq = L;
    };

    TYPED_TEST_SUITE(DdQqTest, LatticeTestTypes);

    /// Test that class contains all relevant class members
    TYPED_TEST(DdQqTest, containsAllVariables) {
      using T = typename TypeParam::type;
      EXPECT_TRUE(std::is_floating_point_v<T>);

      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::DIM)>);
      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::SPEEDS)>);
      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::HSPEED)>);
      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::PAD)>);
      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::ND)>);
      EXPECT_TRUE(std::is_integral_v<decltype(TypeParam::OFF)>);

      using A = typename lbt::array<T, TypeParam::ND>;
      constexpr bool has_dx {std::is_same_v<std::decay_t<decltype(TypeParam::DX)>, A>};
      EXPECT_TRUE(has_dx);
      constexpr bool has_dy {std::is_same_v<std::decay_t<decltype(TypeParam::DY)>, A>};
      EXPECT_TRUE(has_dy);
      constexpr bool has_dz {std::is_same_v<std::decay_t<decltype(TypeParam::DZ)>, A>};
      EXPECT_TRUE(has_dz);
      constexpr bool has_w {std::is_same_v<std::decay_t<decltype(TypeParam::W)>, A>};
      EXPECT_TRUE(has_w);
      constexpr bool has_mask {std::is_same_v<std::decay_t<decltype(TypeParam::MASK)>, A>};
      EXPECT_TRUE(has_mask);

      constexpr bool is_same_cs {std::is_same_v<std::decay_t<decltype(TypeParam::CS)>, T>};
      EXPECT_TRUE(is_same_cs);
    }

    /// Tests for discretisation dimensions
    TYPED_TEST(DdQqTest, discretisationDimensions) {
      EXPECT_TRUE(TypeParam::DIM > 0);
      EXPECT_TRUE(TypeParam::DIM <= 3);
    }

    /// Tests correlation between array size parameters
    TYPED_TEST(DdQqTest, speedsCorrelations) {
      EXPECT_TRUE(TypeParam::SPEEDS >= 2*TypeParam::DIM);
      EXPECT_TRUE(TypeParam::HSPEED > TypeParam::DIM);
      EXPECT_TRUE(TypeParam::PAD >= 0);
      EXPECT_TRUE(TypeParam::ND > TypeParam::SPEEDS);

      EXPECT_TRUE(TypeParam::HSPEED == (TypeParam::SPEEDS + 1)/2);
      EXPECT_TRUE(TypeParam::ND > TypeParam::SPEEDS);
      EXPECT_TRUE(TypeParam::ND == TypeParam::SPEEDS + TypeParam::PAD);
      EXPECT_TRUE(TypeParam::OFF == TypeParam::ND/2);
    }

    /**\fn        isSymmetric
     * \brief     Test if an array respects the lattice symmetries
     *
     * \tparam    T     The data type of the array
     * \tparam    N     The number of elements inside the array
     * \param[in] arr   The array to be tested for symmetry
     * \return    Boolean expression that signals whether the array \p arr respects the lattice symmetries or not
    */
    template <typename T, std::size_t N, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr bool isSymmetric(lbt::array<T,N> const& arr) noexcept {
      constexpr std::size_t hspeed {N/2};
      bool is_symmetric {true};
      is_symmetric &= (arr[0] == arr[hspeed]);
      for (std::size_t i = 1; i < hspeed; ++i) {
        is_symmetric &= lbt::cem::nearlyEqual(arr[i], arr[i + hspeed]);
      }
      return is_symmetric;
    }

    /**\fn        isAntimetric
     * \brief     Test if an array respects the lattice symmetries
     *
     * \tparam    T     The data type of the array
     * \tparam    N     The number of elements inside the array
     * \param[in] arr   The array to be tested for symmetry
     * \return    Boolean expression that signals whether the array \p arr respects the lattice symmetries or not
    */
    template <typename T, std::size_t N, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr bool isAntimetric(lbt::array<T,N> const& arr) noexcept {
      constexpr std::size_t hspeed {N/2};
      bool is_antimetric {true};
      is_antimetric &= (arr[0] == arr[hspeed]);
      for (std::size_t i = 1; i < hspeed; ++i) {
        is_antimetric &= lbt::cem::nearlyEqual(arr[i], -arr[i + hspeed]);
      }
      return is_antimetric;
    }

    /// Test that first entry is the same and other are mirrored
    TYPED_TEST(DdQqTest, speedsSymmetries) {
      bool is_correct {true};
      is_correct &= isAntimetric(TypeParam::DX);
      is_correct &= isAntimetric(TypeParam::DY);
      is_correct &= isAntimetric(TypeParam::DZ);
      is_correct &= isSymmetric(TypeParam::W);
      EXPECT_TRUE(is_correct);
    }

    /**\fn        sumsTo
     * \brief     Test if an array sums to a given value
     *
     * \tparam    T              The data type of the array
     * \tparam    N              The number of elements inside the array
     * \param[in] arr            The array to be tested for summation
     * \param[in] expected_sum   The expected sum value to be checked against
     * \return    Boolean expression that signals whether the array \p arr sums to the given value \p expected_sum or not
    */
    template <typename T, std::size_t N, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr bool sumsTo(lbt::array<T,N> const& arr, T const expected_sum) noexcept {
      T const sum = std::accumulate(std::begin(arr), std::end(arr), static_cast<T>(0));
      return lbt::cem::nearlyEqual(sum, expected_sum);
    }

    /// Array sum
    TYPED_TEST(DdQqTest, arraySum) {
      bool is_correct {true};
      is_correct &= sumsTo(TypeParam::DX, static_cast<typename TypeParam::type>(0));
      is_correct &= sumsTo(TypeParam::DY, static_cast<typename TypeParam::type>(0));
      is_correct &= sumsTo(TypeParam::DZ, static_cast<typename TypeParam::type>(0));
      is_correct &= sumsTo(TypeParam::W,  static_cast<typename TypeParam::type>(1) + TypeParam::W[0]);

      EXPECT_TRUE(is_correct);
    }

    /// Test that mask is correct
    TYPED_TEST(DdQqTest, logicalMask) {
      bool is_success {true};
      constexpr std::size_t hspeed {TypeParam::ND/2};
      for (std::size_t i = 0; i < TypeParam::ND; ++i) {
        if (i == hspeed) {
          TypeParam::MASK[i] == 0;
        } else {
          TypeParam::MASK[i] == 1;
        }
      }

      EXPECT_TRUE(is_success);
    }

    /// Test that speed of sound positive
    TYPED_TEST(DdQqTest, speedOfSound) {
      EXPECT_TRUE(TypeParam::CS > 0);
      EXPECT_TRUE(TypeParam::CS <= 1);
    }

    /**\fn        isAligned
     * \brief     Test if a given pointer is aligned to a given block size
     *
     * \param[in] p           Pointer to be checked for alignment
     * \param[in] alignment   Alignment to be checked
     * \return    Boolean expression that signals whether the pointer \p p is aligned by \p alignment or not
    */
    constexpr bool isAligned(void const* const p, std::size_t const alignment) noexcept {
      return (reinterpret_cast<std::uintptr_t>(p) % alignment == 0);
    }

    /// Test alignment of the different arrays
    TYPED_TEST(DdQqTest, cacheLineAlignment) {
      EXPECT_TRUE(isAligned(&TypeParam::DX[0], LBT_CACHE_LINE_SIZE));
      EXPECT_TRUE(isAligned(&TypeParam::DY[0], LBT_CACHE_LINE_SIZE));
      EXPECT_TRUE(isAligned(&TypeParam::DZ[0], LBT_CACHE_LINE_SIZE));
      EXPECT_TRUE(isAligned(&TypeParam::W[0], LBT_CACHE_LINE_SIZE));
      EXPECT_TRUE(isAligned(&TypeParam::MASK[0], LBT_CACHE_LINE_SIZE));
    }

  }
}

#endif // LBT_LATTICE_UNITTEST
