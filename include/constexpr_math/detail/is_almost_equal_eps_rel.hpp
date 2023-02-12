/**
 * \file     is_almost_equal_eps_rel.hpp
 * \mainpage Constexpr functions for floating-point number comparison at compile-time with a relative epsilon.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IS_ALMOST_EQUAL_EPS_REL
#define LBT_CEM_IS_ALMOST_EQUAL_EPS_REL
#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

#include "abs.hpp"


namespace lbt {
  namespace cem {

    /**\fn        cem::isAlmostEqualEpsRel
     * \brief     Constexpr function for comparing two floating point numbers with a given relative (scaled) tolerance
     *            For more information see https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
     *
     * \tparam    T              Data type of the corresponding number
     * \param[in] a              The first number to be compared
     * \param[in] b              The second number to be compared
     * \param[in] max_distance   Maximum units in the last place (ULPs) to be tolerated
     * \return    Boolean value signaling whether the two values \p a and \p b are equal considering a scaled relative tolerance
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    constexpr bool isAlmostEqualEpsRel(T const a, T const b, std::uint8_t const max_distance = 4) noexcept {
      auto const diff {cem::abs(a-b)};
      auto const sum {cem::abs(a+b)};
      // Required for correct handling of infinity values
      constexpr auto max {std::numeric_limits<T>::max()};
      auto const norm {sum < max ? sum : max};
      return diff <= std::numeric_limits<T>::epsilon()*norm*max_distance || 
             diff < std::numeric_limits<T>::min();
    }

  }
}

#endif // LBT_CEM_IS_ALMOST_EQUAL_EPS_REL
