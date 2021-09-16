#ifndef LBT_TYPE_DEFINITIONS
#define LBT_TYPE_DEFINITIONS
#pragma once

/**\file     type_definitions.hpp
 * \mainpage Contains general type definitions depending on the compiler
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <array>
#include <cstdlib>
#include <memory>


/// Helper macros for stringification
#define TO_STRING_HELPER(X)   #X
#define TO_STRING(X)          TO_STRING_HELPER(X)

/// Cache-line alignment of arrays
#define CACHE_LINE_SIZE 64

/// Definitions for unrolling, inlining and cache-alignment depending on compiler
#if defined(__ICC) || defined(__ICL)
  #define LBT_UNROLL(n)      _Pragma(TO_STRING(unroll (n)))
  #define LBT_FORCE_INLINE   __attribute__((always_inline))
  #define LBT_ALIGN          __attribute__((aligned(CACHE_LINE_SIZE))) alignas(CACHE_LINE_SIZE)
#elif defined(__GNUC__)
  #define LBT_UNROLL(n)      _Pragma(TO_STRING(GCC unroll (16)))
  #define LBT_FORCE_INLINE   __attribute__((always_inline))
  #define LBT_ALIGN          __attribute__((aligned(CACHE_LINE_SIZE))) alignas(CACHE_LINE_SIZE)
#else
  #define LBT_UNROLL(n)
  #define LBT_FORCE_INLINE
  #define LBT_ALIGN          alignas(CACHE_LINE_SIZE)
#endif


/// Define the type of arrays to be used
namespace lbt {
  #ifdef LBT_USE_C_ARRAYS
    template <typename T, std::size_t N>
    using array = T [N];
  #else
    template <typename T, std::size_t N>
    using array = std::array<T, N>;
  #endif
}

#endif // LBT_TYPE_DEFINITIONS
