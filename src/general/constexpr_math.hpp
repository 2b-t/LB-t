#ifndef LBT_CONSTEXPR_MATH
#define LBT_CONSTEXPR_MATH
#pragma once

/**
 * \file     constexpr_math.hpp
 * \mainpage A series of functions that can evaluate to a constant expression and thus
 *           can make use of evaluation and optimisation at compile time.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cassert>
#include <limits>
#include <type_traits>


namespace lbt {
  namespace cem {
    
    /**\fn        cem::abs
     * \brief     Constexpr function for absolute value
     *
     * \tparam    T   Data type of the corresponding number
     * \param[in] x   The number of interest
     * \return    The absolute value of \param x
    */
    template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    constexpr std::decay_t<T> abs(T const x) noexcept {
      return (x > static_cast<T>(0.0)) ? x : -x;
    }

    /**\fn        cem::nearlyEqual
     * \brief     Constexpr function for comparing two floating point numbers with a given tolerance
     *
     * \tparam    T         Data type of the corresponding number
     * \param[in] a         The first number to be compared
     * \param[in] b         The second number to be compared
     * \param[in] epsilon   The tolerance to be considered
     * \return    The absolute value of \param x
    */
    template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    constexpr bool nearlyEqual(T const a, T const b, T const epsilon = 10*std::numeric_limits<T>::epsilon()) noexcept {
      return (cem::abs(a - b) <= epsilon);
    }

    /**\fn        cem::sqrt
     * \brief     Square root implementation with recursive Newton-Raphson method that can
     *            be evaluated as a constant expression at compile time
     *
     * \tparam    T      Floating point data type of the corresponding number
     * \param[in] x      The number of interest
     * \param[in] curr   The result from the current iteration
     * \param[in] prev   The result from the previous iteration
     * \return    The square root of \param x
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr std::decay_t<T> sqrtNewton(T const x, T const curr, T const prev) noexcept {
      return cem::nearlyEqual(curr, prev)
             ? curr
             : cem::sqrtNewton(x, static_cast<T>(0.5) * (curr + x / curr), curr);
    }

    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr std::decay_t<T> sqrt(T const x) noexcept {
      assert(x >= static_cast<T>(0.0));

      return ((x >= static_cast<T>(0.0)) && (x < std::numeric_limits<T>::infinity()))
             ? cem::sqrtNewton(x, x, static_cast<T>(0.0))
             : std::numeric_limits<T>::quiet_NaN();
    }

    /**\fn        cem::ceil
     * \brief     Ceiling function that can be evaluated as a constant expression at compile time
     *
     * \tparam    T     Data type of the corresponding number
     * \param[in] num   The number that should be ceiled
     * \return    The ceiled number \param num
    */
    template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    constexpr std::int64_t ceil(T const num) noexcept {
      return cem::nearlyEqual(static_cast<T>(static_cast<std::int64_t>(num)), num)
             ? static_cast<std::int64_t>(num)
             : static_cast<std::int64_t>(num) + ((num > static_cast<T>(0.0)) ? 1 : 0);
    }
  }
}

#endif // LBT_CONSTEXPR_MATH
