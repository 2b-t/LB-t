/**
 * \file     ipow.hpp
 * \mainpage Constexpr function for calculating the power function with an integer exponent at compile-time.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IPOW
#define LBT_CEM_IPOW
#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

#include "is_inf.hpp"
#include "is_nan.hpp"
#include "is_nearly_equal_eps_rel.hpp"


namespace lbt {
  namespace cem {

    /**\fn        cem::ipow
     * \brief     Computes the the base \p x raised to the power \p y at compile-time by simple recursion
     *
     * \tparam    T   Floating point data type of the corresponding number
     * \param[in] x   The base of the exponentiation
     * \param[in] y   The integer exponent of the exponentiation
     * \return    The base \p x raised to the exponent \p y
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr T ipow(T const x, std::int64_t const y) noexcept {
      auto constexpr pos_inf {std::numeric_limits<T>::infinity()};
      auto constexpr neg_inf {-std::numeric_limits<T>::infinity()};
      auto constexpr nan {-std::numeric_limits<T>::quiet_NaN()};

      bool const is_base_nearly_zero {cem::isNearlyEqualEpsRel(x, static_cast<T>(0.0))};
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
      } else if (cem::isNearlyEqualEpsRel(x, static_cast<T>(1.0))) {
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
        return static_cast<T>(1.0) / cem::ipow(x, -y);
      } else if (y == 0) {
        return static_cast<T>(1.0);
      } else if (y == 1) {
        return x;
      }
      return is_exp_odd ? x*cem::ipow(x, y-1) : cem::ipow(x, y/2)*cem::ipow(x, y/2);
    }

  }
}

#endif // LBT_CEM_IPOW
