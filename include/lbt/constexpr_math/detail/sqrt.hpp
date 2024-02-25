/**
 * \file     sqrt.hpp
 * \mainpage Constexpr function for calculating the square root at compile-time.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_SQRT
#define LBT_CEM_SQRT
#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

#include "lbt/constexpr_math/detail/definitions.hpp"
#include "lbt/constexpr_math/detail/is_inf.hpp"
#include "lbt/constexpr_math/detail/is_nan.hpp"
#include "lbt/constexpr_math/detail/is_almost_equal_eps_rel.hpp"


namespace lbt {
  namespace cem {

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

        return cem::isAlmostEqualEpsRel(curr, prev)
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
      } else if (cem::isAlmostEqualEpsRel(x, static_cast<T>(0.0))) {
        return 0.0;
      } else if (cem::isAlmostEqualEpsRel(x, static_cast<T>(1.0))) {
        return 1.0;
      }

      return cem::detail::sqrtNewton(x, x, static_cast<T>(0), 0);
    }

  }
}

#endif // LBT_CEM_SQRT
