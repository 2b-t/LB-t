/**
 * \file     log.hpp
 * \mainpage Constexpr function for calculating the natural logarithm at compile-time.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_LOG
#define LBT_CEM_LOG
#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

#include "definitions.hpp"
#include "exp.hpp"
#include "is_inf.hpp"
#include "is_nan.hpp"
#include "is_nearly_equal_eps_abs.hpp"
#include "is_nearly_equal_eps_rel.hpp"
#include "mathematical_constants.hpp"


namespace lbt {
  namespace cem {

    namespace detail {
      /**\fn        cem::detail::logNewton
       * \brief     Natural logarithm https://en.wikipedia.org/wiki/Natural_logarithm#High_precision calculated by means of 
       *            Halley-Newton approximation method to be evaluated as a constant expression at compile time
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
      if (cem::isNearlyEqualEpsAbs<T>(x, static_cast<T>(0.0))) {
        return -std::numeric_limits<T>::infinity();
      } else if (cem::isNearlyEqualEpsAbs<T>(x, static_cast<T>(1.0))) {
        return static_cast<T>(0.0);
      } else if (x < static_cast<T>(0.0)) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isNegInf(x)) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isPosInf(x)) {
        return std::numeric_limits<T>::infinity();
      } else if (cem::isNan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isNearlyEqualEpsAbs<T>(x, cem::e<T>)) {
        return static_cast<T>(1.0);
      }

      return cem::detail::logNewton(x, static_cast<T>(0.0), 0);
    }

  }
}

#endif // LBT_CEM_LOG
