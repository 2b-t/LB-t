/**
 * \file     definitions.hpp
 * \mainpage Algorithmic constants for constexpr function library.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_DEFINITIONS
#define LBT_CEM_DEFINITIONS
#pragma once

#include <cstdint>


namespace lbt {
  namespace cem {

    /// Recursion depth for recursive algorithms
    inline constexpr std::int64_t default_max_recursion_depth = 9999;

  }
}

#endif // LBT_CEM_DEFINITIONS
