#ifndef LBT_ARRAY_UTILITIES
#define LBT_ARRAY_UTILITIES
#pragma once

/**
 * \file     array_utilities.hpp
 * \mainpage Utilities for arrays and memory alignment
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <limits>
#include <numeric>
#include <type_traits>

#include "../../src/general/constexpr_math.hpp"
#include "../../src/general/type_definitions.hpp"


namespace lbt {
  namespace test {

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
      is_symmetric &= lbt::cem::nearlyEqual(arr.at(0), arr.at(hspeed));
      for (std::size_t i = 1; i < hspeed; ++i) {
        is_symmetric &= lbt::cem::nearlyEqual(arr.at(i), arr.at(i + hspeed));
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
      is_antimetric &= lbt::cem::nearlyEqual(arr.at(0), arr.at(hspeed));
      for (std::size_t i = 1; i < hspeed; ++i) {
        is_antimetric &= lbt::cem::nearlyEqual(arr.at(i), -arr.at(i + hspeed));
      }
      return is_antimetric;
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

    /**\fn        isAligned
     * \brief     Test if a given pointer is aligned to a given block size
     *
     * \param[in] ptr         Pointer to be checked for alignment
     * \param[in] alignment   Alignment to be checked
     * \return    Boolean expression that signals whether the pointer \p p is aligned by \p alignment or not
    */
    constexpr bool isAligned(void const* const ptr, std::int64_t const alignment) noexcept {
      return (reinterpret_cast<std::uintptr_t>(ptr) % static_cast<std::size_t>(alignment) == 0);
    }

  }
}

#endif // LBT_ARRAY_UTILITIES
