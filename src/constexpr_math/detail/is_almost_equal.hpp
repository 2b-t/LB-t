/**
 * \file     is_almost_equal.hpp
 * \mainpage Constexpr functions for floating-point number comparison at compile-time with different criteria.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IS_ALMOST_EQUAL
#define LBT_CEM_IS_ALMOST_EQUAL
#pragma once

#include "is_almost_equal_eps_abs.hpp"
#include "is_almost_equal_eps_rel.hpp"

#if __cplusplus >= 202002L
  /// Constexpr ULPS comparison only possible from C++20 onwards
  #include "is_almost_equal_ulps.hpp"
#endif

#endif // LBT_CEM_IS_ALMOST_EQUAL
