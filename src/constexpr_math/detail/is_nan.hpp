/**
 * \file     is_nan.hpp
 * \mainpage Constexpr function for comparing a number to NaN at compile-time.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IS_NAN
#define LBT_CEM_IS_NAN
#pragma once

#include <type_traits>


namespace lbt {
  namespace cem {

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

  }
}

#endif // LBT_CEM_IS_NAN
