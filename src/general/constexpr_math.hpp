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

    /**\fn        cem::isNan
     * \brief     Constexpr function for determining if a variable corresponds to Not-A-Number (NaN)
     *
     * \tparam    T   Data type of the corresponding number
     * \param[in] x   The number of interest
     * \return    Boolean variable indicating if the value \p x corresponds to Not-A-Number (NaN) or if it does not
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr bool isNan(T x) noexcept {
      return (x != x);
    }

    /**\fn        cem::isPosInf
     * \brief     Constexpr function for determining if a variable corresponds to positive infinity
     *
     * \tparam    T   Data type of the corresponding number
     * \param[in] x   The number of interest
     * \return    Boolean variable indicating if the value \p x corresponds to positive infinity or if it does not
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr bool isPosInf(T x) noexcept {
      return (x > 0 && x/x != x/x);
    }

    /**\fn        cem::isNegInf
     * \brief     Constexpr function for determining if a variable corresponds to negative infinity
     *
     * \tparam    T   Data type of the corresponding number
     * \param[in] x   The number of interest
     * \return    Boolean variable indicating if the value \p x corresponds to negative infinity or if it does not
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr bool isNegInf(T x) noexcept {
      return (x < 0 && x/x != x/x);
    }
    
    /**\fn        cem::abs
     * \brief     Constexpr function for absolute value
     *
     * \tparam    T   Data type of the corresponding number
     * \param[in] x   The number of interest
     * \return    The absolute value of \p x
    */
    template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    constexpr std::decay_t<T> abs(T x) noexcept {
      using F = std::decay_t<T>;

      if constexpr (std::is_floating_point_v<T>) {
        if (cem::isNan(x)) {
          return x;
        } else if (cem::isPosInf(x)) {
          return x;
        } else if (cem::isNegInf(x)) {
          return -x;
        }
      }

      return (x < 0) ? -x : x;
    }

    /**\fn        cem::nearlyEqual
     * \brief     Constexpr function for comparing two floating point numbers with a given tolerance
     *
     * \tparam    T         Data type of the corresponding number
     * \param[in] a         The first number to be compared
     * \param[in] b         The second number to be compared
     * \param[in] epsilon   The tolerance to be considered
     * \return    The absolute value of \p x
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr bool nearlyEqual(T a, T b, T epsilon = 10*std::numeric_limits<T>::epsilon()) noexcept {
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
     * \return    The square root of \p x
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr std::decay_t<T> sqrtNewton(T x, T curr, T prev) noexcept {
      using F = std::decay_t<T>;
      return cem::nearlyEqual(curr, prev)
             ? curr
             : cem::sqrtNewton(x, static_cast<F>(0.5) * (curr + x / curr), curr);
    }

    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr std::decay_t<T> sqrt(T x) noexcept {
      using F = std::decay_t<T>;

      if (x < 0) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isNan(x)) {
        return x;
      } else if (cem::isPosInf(x)) {
        return x;
      } else if (cem::isNegInf(x)) {
        return std::numeric_limits<T>::quiet_NaN();
      }

      return ((x >= static_cast<F>(0.0)) && (x < std::numeric_limits<F>::infinity()))
             ? cem::sqrtNewton(x, x, static_cast<F>(0))
             : std::numeric_limits<F>::quiet_NaN();
    }

    /**\fn        cem::ceil
     * \brief     Ceiling function that can be evaluated as a constant expression at compile time
     *
     * \tparam    T     Data type of the corresponding number
     * \param[in] num   The number that should be ceiled
     * \return    The ceiled number \p num
    */
    template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    constexpr std::decay_t<T> ceil(T x) noexcept {
      using F = std::decay_t<T>;

      if constexpr (std::is_floating_point_v<T>) {
        if (cem::isNan(x) || cem::isPosInf(x) || cem::isNegInf(x)) {
          return x;
        }
      }
      
      return cem::nearlyEqual(static_cast<F>(static_cast<std::int64_t>(x)), x)
             ? static_cast<F>(static_cast<std::int64_t>(x))
             : static_cast<F>(static_cast<std::int64_t>(x)) + ((x > 0) ? 1 : 0);
    }
  }
}

#endif // LBT_CONSTEXPR_MATH
