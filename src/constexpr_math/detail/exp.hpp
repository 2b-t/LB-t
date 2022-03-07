/**
 * \file     exp.hpp
 * \mainpage Constexpr function for calculating the exponential function at compile-time.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_EXP
#define LBT_CEM_EXP
#pragma once

#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>

#include "definitions.hpp"
#include "is_inf.hpp"
#include "is_nan.hpp"
#include "is_nearly_equal_eps_abs.hpp"
#include "is_nearly_equal_eps_rel.hpp"
#include "mathematical_constants.hpp"


namespace lbt {
  namespace cem {

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
      if (cem::isNearlyEqualEpsAbs(x, static_cast<T>(0.0))) {
        return static_cast<T>(1.0);
      } else if (cem::isNegInf(x)) {
        return static_cast<T>(0.0);
      } else if (cem::isPosInf(x)) {
        return std::numeric_limits<T>::infinity();
      } else if (cem::isNan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isNearlyEqualEpsAbs(x, static_cast<T>(1.0))) {
        return cem::e<T>;
      } 

      return cem::detail::expTaylor(x, static_cast<T>(1.0), x, static_cast<T>(1.0), 2, 0);
    }

  }
}

#endif // LBT_CEM_EXP
