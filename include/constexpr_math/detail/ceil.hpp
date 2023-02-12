/**
 * \file     ceil.hpp
 * \mainpage Constexpr ceil function to be evaluated at compile-time
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_CEIL
#define LBT_CEM_CEIL
#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

#include "is_inf.hpp"
#include "is_nan.hpp"
#include "is_almost_equal_eps_rel.hpp"


namespace lbt {
  namespace cem {

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

      return cem::isAlmostEqualEpsRel(static_cast<T>(static_cast<std::int64_t>(x)), x)
             ? static_cast<T>(static_cast<std::int64_t>(x))
             : static_cast<T>(static_cast<std::int64_t>(x)) + ((x > 0) ? 1 : 0);
    }

  }
}

#endif // LBT_CEM_CEIL
