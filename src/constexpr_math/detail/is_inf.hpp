/**
 * \file     is_inf.hpp
 * \mainpage Functions for identifying positive and negative infinity at compile-time.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IS_INF
#define LBT_CEM_IS_INF
#pragma once

#include <type_traits>


namespace lbt {
  namespace cem {

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

  }
}

#endif // LBT_CEM_IS_INF
