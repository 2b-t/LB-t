/**
 * \file     is_nearly_equal_eps_rel.hpp
 * \mainpage Constexpr functions for floating-point number comparison at compile-time with a relative epsilon.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IS_NEARLY_EQUAL_EPS_REL
#define LBT_CEM_IS_NEARLY_EQUAL_EPS_REL
#pragma once

#include <limits>
#include <type_traits>

#include "abs.hpp"


namespace lbt {
  namespace cem {

    /**\fn        cem::nearlyEqualEpsRel
     * \brief     Constexpr function for comparing two floating point numbers with a given relative (scaled) tolerance
     *
     * \tparam    T         Data type of the corresponding number
     * \param[in] a         The first number to be compared
     * \param[in] b         The second number to be compared
     * \param[in] epsilon   The relative tolerance to be considered
     * \return    Boolean value signaling whether the two values \p a and \p b are equal considering a relative tolerance
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr bool isNearlyEqualEpsRel(T const a, T const b, T const epsilon = 128*std::numeric_limits<T>::epsilon()) noexcept {
      auto const diff {cem::abs(a-b)};
      auto const sum_mag {cem::abs(a) + cem::abs(b)};
      constexpr auto max {std::numeric_limits<T>::max()};
      auto const norm {sum_mag < max ? sum_mag : max};
      return diff < epsilon*norm;
    }

  }
}

#endif // LBT_CEM_IS_NEARLY_EQUAL_EPS_REL
