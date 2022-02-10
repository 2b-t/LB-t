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

    constexpr std::int64_t default_max_recursion_depth = 9999;

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
    constexpr bool isNan(T const x) noexcept {
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
    constexpr bool isPosInf(T const x) noexcept {
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
    constexpr bool isNegInf(T const x) noexcept {
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
    constexpr T abs(T const x) noexcept {
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

    /**\fn        cem::nearlyEqualEpsAbs
     * \brief     Constexpr function for comparing two floating point numbers with a given absolute tolerance
     *
     * \tparam    T         Data type of the corresponding number
     * \param[in] a         The first number to be compared
     * \param[in] b         The second number to be compared
     * \param[in] epsilon   The absolute tolerance to be considered
     * \return    Boolean value signaling whether the two values \p a and \p b are equal considering an absolute tolerance
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr bool isNearlyEqual(T const a, T const b, T epsilon = 10*std::numeric_limits<T>::epsilon()) noexcept {
      return (cem::abs(a - b) <= epsilon);
    }
    /**\fn        cem::nearlyEqualEpsRel
     * \brief     Constexpr function for comparing two floating point numbers with a given relative (scaled) tolerance
     *
     * \tparam    T         Data type of the corresponding number
     * \param[in] a         The first number to be compared
     * \param[in] b         The second number to be compared
     * \param[in] epsilon   The relative tolerance to be considered
     * \return    Boolean value signaling whether the two values \p a and \p b are equal considering a relative tolerance
    */
    template <typename T>
    constexpr bool isNearlyEqualEpsRel(T const a, T const b, T const epsilon = 128*std::numeric_limits<T>::epsilon()) noexcept {
      auto const diff {cem::abs(a-b)};
      auto const sum_mag {cem::abs(a) + cem::abs(b)};
      constexpr auto max {std::numeric_limits<T>::max()};
      auto const norm {sum_mag < max ? sum_mag : max};
      return diff < epsilon*norm;
    }

    namespace detail {
      /**\fn        cem::detail::sqrtNewton
       * \brief     Square root implementation with recursive Newton-Raphson method that can
       *            be evaluated as a constant expression at compile time
       *
       * \tparam    T       Floating point data type of the corresponding number
       * \tparam    RD      Maximum recursion depth
       * \param[in] x       The number of interest
       * \param[in] curr    The result from the current iteration
       * \param[in] prev    The result from the previous iteration
       * \param[in] depth   Current recursion depth
       * \return    The square root of \p x
      */
      template <typename T, std::int64_t RD = cem::default_max_recursion_depth, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
      constexpr T sqrtNewton(T const x, T const curr, T const prev, std::int64_t const depth = 0) noexcept {
        if (depth >= RD) {
          return curr;
        }

        return cem::isNearlyEqualEpsRel(curr, prev)
              ? curr
              : sqrtNewton(x, static_cast<T>(0.5) * (curr + x / curr), curr, depth+1);
      }
    }
    /**\fn        cem::sqrt
     * \brief     Square root implementation with recursive Newton-Raphson method that can
     *            be evaluated as a constant expression at compile time
     *
     * \tparam    T    Floating point data type of the corresponding number
     * \param[in] x    The number of interest
     * \return    The square root of \p x
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr T sqrt(T const x) noexcept {
      if (x < 0) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isNan(x)) {
        return x;
      } else if (cem::isPosInf(x)) {
        return x;
      } else if (cem::isNegInf(x)) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isNearlyEqual(x, static_cast<T>(0.0))) {
        return x;
      }

      return ((x >= static_cast<T>(0.0)) && (x < std::numeric_limits<T>::infinity()))
             ? cem::detail::sqrtNewton(x, x, static_cast<T>(0), 0)
             : std::numeric_limits<T>::quiet_NaN();
    }

    /**\fn        cem::ceil
     * \brief     Ceiling function that can be evaluated as a constant expression at compile time
     * \warning   Problematic for very large numbers that can't fit into an std::int64_t
     *
     * \tparam    T     Data type of the corresponding number
     * \param[in] num   The number that should be ceiled
     * \return    The ceiled number \p num
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr T ceil(T const x) noexcept {
      if (cem::isNan(x) || cem::isPosInf(x) || cem::isNegInf(x)) {
        return x;
      }
      
      return cem::isNearlyEqual(static_cast<T>(static_cast<std::int64_t>(x)), x)
             ? static_cast<T>(static_cast<std::int64_t>(x))
             : static_cast<T>(static_cast<std::int64_t>(x)) + ((x > 0) ? 1 : 0);
    }

    namespace detail {
      /**\fn        cem::detail::expTaylor
       * \brief     Exponential function https://en.wikipedia.org/wiki/Exponential_function calculated by Taylor series expansion 
       *            to be evaluated as a constant expression at compile time
       * \warning   Implementation does not work well for negative numbers yet
       *
       * \tparam    T       Floating point data type of the corresponding number
       * \tparam    RD      Maximum recursion depth
       * \param[in] x       The number of interest
       * \param[in] sum     The sum of the different Taylor terms
       * \param[in] prod    The product of the point of reference
       * \param[in] n       The factorial for the current term
       * \param[in] i       The product of the point of reference
       * \param[in] depth   Current recursion depth
       * \return    The sum of the Taylor terms up to the corresponding degree
      */
      template <typename T, std::int64_t RD = cem::default_max_recursion_depth, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
      constexpr T expTaylor(T const x, T const sum, T prod, T n, std::int64_t const i, std::int64_t const depth = 0) noexcept {
        if ((depth >= RD) || cem::isPosInf(prod) || cem::isNegInf(prod)) {
          return sum;
        }
        // Dirty hack: Avoid drifting away to infinity individually
        constexpr T division_factor {1e4};
        if ((prod > division_factor) && (n > division_factor)) {
          prod /= division_factor;
          n /= division_factor;
        }
        auto const sum_iteration {sum + prod/n};
        return cem::isNearlyEqualEpsRel(sum, sum_iteration) ? sum : expTaylor(x, sum_iteration, prod*x, n*i, i+1, depth+1);
      }
    }
    /**\fn        cem::exp
     * \brief     Exponential function calculated by Taylor series expansion that can
     *            be evaluated as a constant expression at compile time
     *
     * \tparam    T   Floating point data type of the corresponding number
     * \param[in] x   The number of interest
     * \return    The resulting exponential function evaluated at point \p x
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr T exp(T const x) noexcept {
      if (cem::isNearlyEqual(x, static_cast<T>(0.0))) {
        return static_cast<T>(1.0);
      } else if (cem::isNegInf(x)) {
        return static_cast<T>(0.0);
      } else if (cem::isPosInf(x)) {
        return std::numeric_limits<T>::infinity();
      } else if (cem::isNan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isNearlyEqual(x, static_cast<T>(1.0))) {
        return cem::e<T>;
      } 

      return cem::detail::expTaylor(x, static_cast<T>(1.0), x, static_cast<T>(1.0), 2, 0);
    }

    namespace detail {
      /**\fn        cem::detail::logNewton
       * \brief     Natural logarithm https://en.wikipedia.org/wiki/Natural_logarithm#High_precision calculated by means of 
       *            Newton method to be evaluated as a constant expression at compile time
       *
       * \tparam    T       Floating point data type of the corresponding number
       * \tparam    RD      Maximum recursion depth
       * \param[in] x       The number to take the logarithm of
       * \param[in] prev    The result from the previous iteration
       * \param[in] depth   Current recursion depth
       * \return    The result from the current iteration
      */
      template <typename T, std::int64_t RD = cem::default_max_recursion_depth, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
      constexpr T logNewton(T const x, T const prev, std::int64_t const depth = 0) noexcept {
        if (depth >= RD) {
          return prev;
        }
        auto const curr = prev + static_cast<T>(2.0)*(x-cem::exp(prev))/(x+cem::exp(prev));
        return cem::isNearlyEqualEpsRel(prev, curr) ? curr : logNewton(x, curr, depth+1);
      }
    }
    /**\fn        cem::log
     * \brief     Natural logarithm https://en.wikipedia.org/wiki/Natural_logarithm#High_precision calculated by means of 
     *            Newton method to be evaluated as a constant expression at compile time
     * \warning   Numerical stability only between numbers of around 0.25 to 1000: Else a break-down to numbers of adequate size
     *            is required which currently is not implemented.
     *
     * \tparam    T   Floating point data type of the corresponding number
     * \param[in] x   The number to take the logarithm of
     * \return    The logarithm evaluated at point \p x
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr T log(T const x) noexcept {
      if (cem::isNearlyEqual<T>(x, static_cast<T>(0.0))) {
        return -std::numeric_limits<T>::infinity();
      } else if (cem::isNearlyEqual<T>(x, static_cast<T>(1.0))) {
        return static_cast<T>(0.0);
      } else if (x < static_cast<T>(0.0)) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isNegInf(x)) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isPosInf(x)) {
        return std::numeric_limits<T>::infinity();
      } else if (cem::isNan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isNearlyEqual<T>(x, cem::e<T>)) {
        return static_cast<T>(1.0);
      }

      return cem::detail::logNewton(x, static_cast<T>(0.0), 0);
    }

    /**\fn        cem::pow
     * \brief     Computes the the base \p x raised to the power \p y at compile-time using the identity x^y = exp(y*log(x))
     *
     * \tparam    T   Floating point data type of the corresponding number
     * \param[in] x   The base of the exponentiation
     * \param[in] y   The exponent of the exponentiation
     * \return    The base \p x raised to the exponent \p y
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr T pow(T const x, std::int64_t const y) noexcept {
      auto constexpr pos_inf {std::numeric_limits<T>::infinity()};
      auto constexpr neg_inf {-std::numeric_limits<T>::infinity()};
      auto constexpr nan {-std::numeric_limits<T>::quiet_NaN()};

      bool const is_base_nearly_zero {cem::isNearlyEqual(x, static_cast<T>(0.0))};
      bool const is_base_pos {(x > static_cast<T>(0.0))};
      bool const is_base_neg {(x < static_cast<T>(0.0))};
      bool const is_base_neg_inf {cem::isNegInf(x)};
      bool const is_base_pos_inf {cem::isPosInf(x)};
      bool const is_base_nan {cem::isNan(x)};

      bool const is_exp_odd = (y & 1);
      bool const is_exp_pos = (y > 0);
      bool const is_exp_neg = (y < 0);

      if (is_base_nearly_zero && is_base_pos && is_exp_neg && is_exp_odd) {
        return pos_inf;
      } else if (is_base_nearly_zero && is_base_neg && is_exp_neg && is_exp_odd) {
        return neg_inf;
      } else if (is_base_nearly_zero && is_exp_neg && !is_exp_odd) {
        return pos_inf;
      } else if (is_base_nearly_zero && is_base_pos && is_exp_pos && is_exp_odd) {
        return static_cast<T>(+0.0);
      } else if (is_base_nearly_zero && is_base_neg && is_exp_pos && is_exp_odd) {
        return static_cast<T>(-0.0);
      } else if (is_base_nearly_zero && is_exp_pos && !is_exp_odd) {
        return static_cast<T>(+0.0);
      } else if (cem::isNearlyEqual(x, static_cast<T>(1.0))) {
        return static_cast<T>(1.0);
      } else if (y == 0) {
        return static_cast<T>(1.0);
      } else if (is_base_neg_inf && is_exp_neg && is_exp_odd) {
        return static_cast<T>(-0.0);
      } else if (is_base_neg_inf && is_exp_neg && !is_exp_odd) {
        return static_cast<T>(+0.0);
      } else if (is_base_neg_inf && is_exp_pos && is_exp_odd) {
        return neg_inf;
      } else if (is_base_neg_inf && is_exp_pos && !is_exp_odd) {
        return pos_inf;
      } else if (is_base_pos_inf && is_exp_neg) {
        return static_cast<T>(+0.0);
      } else if (is_base_pos_inf && is_exp_pos) {
        return pos_inf;
      } else if (is_base_nan) {
        return nan;
      }

      if (is_exp_neg) {
        return static_cast<T>(1.0) / cem::pow(x, -y);
      } else if (y == 0) {
        return static_cast<T>(1.0);
      } else if (y == 1) {
        return x;
      }
      return is_exp_odd ? x*cem::pow(x, y-1) : cem::pow(x, y/2)*cem::pow(x, y/2);
    }
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr T pow(T const x, T const y) noexcept {
      auto constexpr pos_inf {std::numeric_limits<T>::infinity()};
      auto constexpr neg_inf {-std::numeric_limits<T>::infinity()};
      auto constexpr nan {-std::numeric_limits<T>::quiet_NaN()};
      
      bool const is_base_nearly_zero {cem::isNearlyEqual(x, static_cast<T>(0.0))};
      bool const is_base_pos {(x > static_cast<T>(0.0))};
      bool const is_base_neg {(x < static_cast<T>(0.0))};
      bool const is_base_neg_inf {cem::isNegInf(x)};
      bool const is_base_pos_inf {cem::isPosInf(x)};
      bool const is_base_inf {is_base_neg_inf || is_base_pos_inf};
      bool const is_base_nan {cem::isNan(x)};
      bool const is_base_finite {!is_base_nan && !is_base_inf};

      bool const is_exp_pos {(y > static_cast<T>(0.0))};
      bool const is_exp_neg {(y < static_cast<T>(0.0))};
      bool const is_exp_neg_inf {cem::isNegInf(y)};
      bool const is_exp_pos_inf {cem::isPosInf(y)};
      bool const is_exp_inf {is_exp_neg_inf || is_exp_pos_inf};
      bool const is_exp_nan {cem::isNan(y)};
      bool const is_exp_finite {!is_exp_nan && !is_exp_inf};
      bool const is_exp_int {cem::isNearlyEqual(y, cem::ceil(y)) && is_exp_finite};

      // Avoid overflow with integer version and ceil function but reducing possible max and min!
      if ((y > static_cast<T>(std::numeric_limits<std::int64_t>::max()-1)) && (!is_exp_pos_inf)) {
        return pow(x, pos_inf);
      } else if ((y < static_cast<T>(std::numeric_limits<std::int64_t>::min())) && (!is_exp_neg_inf)) {
        return pow(x, neg_inf);
      }

      if (is_exp_int) {
        return pow(x, static_cast<std::int64_t>(y));
      } else if (is_base_nearly_zero && is_exp_neg && is_exp_finite) {
        return pos_inf;
      } else if (is_base_nearly_zero && is_exp_neg_inf) {
        return pos_inf;
      } else if (is_base_nearly_zero && is_exp_pos) {
        return static_cast<T>(+0.0);
      } else if (cem::isNearlyEqual(x, static_cast<T>(-1.0)) && is_exp_inf) {
        return static_cast<T>(1.0);
      } else if (cem::isNearlyEqual(x, static_cast<T>(1.0))) {
        return static_cast<T>(1.0);
      } else if (cem::isNearlyEqual(y, static_cast<T>(0.0))) {
        return static_cast<T>(1.0);
      } else if (is_base_finite && is_base_neg && is_exp_finite) {
        return nan;
      } else if ((cem::abs(x) < static_cast<T>(1.0)) && is_exp_neg_inf) {
        return pos_inf;
      } else if ((cem::abs(x) > static_cast<T>(1.0)) && is_exp_neg_inf) {
        return static_cast<T>(+0);
      } else if ((cem::abs(x) < static_cast<T>(1.0)) && is_exp_pos_inf) {
        return static_cast<T>(+0);
      } else if ((cem::abs(x) > static_cast<T>(1.0)) && is_exp_pos_inf) {
        return pos_inf;
      } else if (is_base_neg_inf && is_exp_neg) {
        return static_cast<T>(+0.0);
      } else if (is_base_neg_inf && is_exp_pos) {
        return pos_inf;
      } else if (is_base_pos_inf && is_exp_neg) {
        return static_cast<T>(+0.0);
      } else if (is_base_pos_inf && is_exp_pos) {
        return pos_inf;
      } else if (is_base_nan || is_exp_nan) {
        return nan;
      }
      
      return cem::exp(y*cem::log(x));
    }

  }
}

#endif // LBT_CONSTEXPR_MATH
