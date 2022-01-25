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
#include <cstdint>
#include <limits>
#include <type_traits>


namespace lbt {
  namespace cem {

    /// Variable templates for Euler's identity pi and Euler's number e
    template <typename T>
    constexpr T pi = static_cast<T>(3.1415926535897932385L);
    template <typename T>
    constexpr T e = static_cast<T>(2.71828182845904523536L);

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

    namespace detail {
      /**\fn        cem::sqrtNewton
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
              : sqrtNewton(x, static_cast<F>(0.5) * (curr + x / curr), curr);
      }
    }
    /**\fn        cem::sqrt
     * \brief     Square root implementation with recursive Newton-Raphson method that can
     *            be evaluated as a constant expression at compile time
     *
     * \tparam    T      Floating point data type of the corresponding number
     * \param[in] x      The number of interest
     * \return    The square root of \p x
    */
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
             ? cem::detail::sqrtNewton<F>(x, x, static_cast<F>(0))
             : std::numeric_limits<F>::quiet_NaN();
    }

    /**\fn        cem::ceil
     * \brief     Ceiling function that can be evaluated as a constant expression at compile time
     *
     * \tparam    T     Data type of the corresponding number
     * \param[in] num   The number that should be ceiled
     * \return    The ceiled number \p num
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr std::decay_t<T> ceil(T x) noexcept {
      using F = std::decay_t<T>;

      if (cem::isNan(x) || cem::isPosInf(x) || cem::isNegInf(x)) {
        return x;
      }
      
      return cem::nearlyEqual(static_cast<F>(static_cast<std::int64_t>(x)), x)
             ? static_cast<F>(static_cast<std::int64_t>(x))
             : static_cast<F>(static_cast<std::int64_t>(x)) + ((x > 0) ? 1 : 0);
    }

    // TODO: Add documentation and tests (comparison to std as well as corner cases)
    // https://en.wikipedia.org/wiki/Exponential_function
    // exp with Taylor expansion
    namespace detail {
      template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
      constexpr std::decay_t<T> expTaylor(T x, T sum, T prod, T n, int const i) noexcept {
        return cem::nearlyEqual(sum, sum + prod/n) ? sum : expTaylor(x, sum + prod/n, prod*x, n*i, i+1);
      }
    }
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr std::decay_t<T> exp(T x) noexcept {
      using F = std::decay_t<T>;

      if (cem::isNan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::nearlyEqual<F>(x, static_cast<F>(0.0))) {
        return static_cast<F>(1.0);
      } else if (cem::nearlyEqual<F>(x, static_cast<F>(1.0))) {
        return cem::e<F>;
      } else if (cem::isNegInf(x)) {
        return static_cast<F>(0.0);
      } else if (cem::isPosInf(x)) {
        return std::numeric_limits<T>::infinity();
      }

      return cem::detail::expTaylor<F>(x, static_cast<F>(1.0), x, static_cast<F>(1.0), 2);
    }

    // Natural logarithm: https://en.wikipedia.org/wiki/Natural_logarithm#High_precision
    namespace detail {
      template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
      constexpr std::decay_t<T> logNewtonIteration(T x, T y) noexcept {
        using F = std::decay_t<T>;
        return y + static_cast<F>(2.0)*(x-cem::exp(y))/(x+cem::exp(y));
      }
      template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
      constexpr std::decay_t<T> logNewton(T x, T y) noexcept {
        auto const result_iteration = logNewtonIteration(x, y);
        return cem::nearlyEqual(y, result_iteration) ? y : logNewton(x, result_iteration);
      }
    }
    // Numerical stability 0.25 to 1000: Break-down to numbers of adequate size: Not implemented
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr std::decay_t<T> log(T x) noexcept {
      using F = std::decay_t<T>;

      if (x < 0) {
        return std::numeric_limits<F>::quiet_NaN();
      } else if (cem::nearlyEqual<F>(x, static_cast<F>(1.0))) {
        return static_cast<F>(0.0);
      } else if (cem::nearlyEqual<F>(x, cem::e<F>)) {
        return static_cast<F>(1.0);
      } else if (cem::isNegInf(x)) {
        return std::numeric_limits<F>::quiet_NaN();
      } else if (cem::isPosInf(x)) {
        return std::numeric_limits<F>::infinity();
      } else if (cem::isNan(x)) {
        return std::numeric_limits<F>::quiet_NaN();
      }

      return cem::detail::logNewton(x, static_cast<F>(0.0));
    }

    // Implement another version for an integer exponent!
    // x^y = exp(y*log(x))
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr std::decay_t<T> pow(T x, T y) noexcept {
      using F = std::decay_t<T>;

      // 0 ^ NaN -> NaN
      // 0 ^ 0 -> NaN
      // Infinity ^ 0 -> NaN
      // -Infinity ^ 0 -> NaN
      // 0 ^ -Infinity -> Infinity
      // 0 ^ Infinity -> NaN
      // NaN ^ 0 -> 1
      // NaN ^ NaN -> NaN
      // 3.6 ^ NaN -> NaN
      // NaN ^ 3.5 -> NaN
      // -Infinity ^ NaN -> NaN
      // Infinity ^ NaN -> NaN
      // NaN ^ -Infinity -> NaN
      // NaN ^ Infinity -> NaN
      // -Infinity ^ 3.8 -> odd: negative infinity, even: positive infinity
      // -Infinity ^ -3.8 -> 0
      // Infinity ^ 2.1 -> Infinity
      // 0.7 ^ -Infinity -> Infinity
      // 4.3 ^ -Infinity -> 0
      // 0.7 ^ Infinity -> 0
      // 4.3 ^ Infinity -> Infinity
      // 9.6 ^ -4 = 8493.4656
      // Infinity ^ Infinity -> Infinity
      // Infinity ^ 0 -> 1
      // 0 ^ Infinity -> 0
      // -Infinity ^ Infinity -> Infinity
      // Infinity ^ -Infinity -> 0
      // -Infinity ^ -Infinity -> 0
      // -Infinity ^ 0 -> 1
      // 0 ^ -Infinity -> Inf
      // 0 ^ 0 -> NaN
      // 7.3 ^ 0 -> 1
      // 0 ^ 3.6 -> 0

      if (cem::isNan(x) || cem::isNan(y)) {
        return std::numeric_limits<F>::quiet_NaN();
      }
      else if (cem::nearlyEqual<F>(x, static_cast<F>(1.0))) {
        return static_cast<F>(1.0);
      } 
      else if (x < 0) {
        // Warning: Does not work with integer exponents!
        return std::numeric_limits<F>::quiet_NaN();
      } else if (cem::isNegInf(x)) {
        return std::numeric_limits<F>::quiet_NaN();
      } else if (cem::isPosInf(x)) {
        return std::numeric_limits<F>::infinity();
      }
      
      return cem::exp(y*cem::log(x));
    }

  }
}

#endif // LBT_CONSTEXPR_MATH
