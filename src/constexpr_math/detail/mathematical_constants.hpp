/**
 * \file     mathematical_constants.hpp
 * \mainpage Header containing mathematical constant definitions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_MATHEMATICAL_CONSTANTS
#define LBT_CEM_MATHEMATICAL_CONSTANTS
#pragma once

#include <type_traits>


namespace lbt {
  namespace cem {

    /// Variable templates for Euler's identity pi and Euler's number e
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    inline constexpr T pi = static_cast<T>(3.1415926535897932385L);

    /// Variable templates for Euler's number e
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    inline constexpr T e = static_cast<T>(2.71828182845904523536L);

  }
}

#endif // LBT_CEM_MATHEMATICAL_CONSTANTS
