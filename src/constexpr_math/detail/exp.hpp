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

#include "abs.hpp"
#include "ceil.hpp"
#include "ipow.hpp"
#include "is_inf.hpp"
#include "is_nan.hpp"
#include "is_nearly_equal_eps_rel.hpp"
#include "mathematical_constants.hpp"


namespace lbt {
  namespace cem {

    /**\fn        cem::exp
     * \brief     Exponential function https://en.wikipedia.org/wiki/Exponential_function calculated by Taylor series expansion 
     *            with Horner's method https://www.pseudorandom.com/implementing-exp#section-7 that can be evaluated as a 
     *            constant expression at compile time
     *
     * \tparam    T   Floating point data type of the corresponding number
     * \param[in] x   The number of interest
     * \return    The resulting exponential function evaluated at point \p x
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr T exp(T const x) noexcept {
      if (cem::isNearlyEqualEpsRel(x, static_cast<T>(0.0))) {
        return static_cast<T>(1.0);
      } else if (cem::isNegInf(x)) {
        return static_cast<T>(0.0);
      } else if (cem::isPosInf(x)) {
        return std::numeric_limits<T>::infinity();
      } else if (cem::isNan(x)) {
        return std::numeric_limits<T>::quiet_NaN();
      } else if (cem::isNearlyEqualEpsRel(x, static_cast<T>(1.0))) {
        return cem::e<T>;
      } 

      T const abs_x {cem::abs(x)};
      constexpr std::int64_t multiplier {12}; // Heuristic constant
      std::int64_t const n {static_cast<std::int64_t>(cem::ceil(abs_x*cem::e<T>)*multiplier)};
      T taylor_series {1.0};
      for (std::int64_t i = n; i > 0; --i) {
        taylor_series = taylor_series*(abs_x / static_cast<T>(i)) + 1.0;
      };
      if (x < 0.0) {
        return 1.0 / taylor_series;
      }
      return taylor_series;
    }

  }
}

#endif // LBT_CEM_EXP
