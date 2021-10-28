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
#define LBT_TO_STRING_HELPER(X)   #X
#define LBT_TO_STRING(X)          LBT_TO_STRING_HELPER(X)

/// Cache-line alignment of arrays
#define LBT_CACHE_LINE_SIZE 64

/// Definitions for unrolling, inlining and cache-alignment depending on compiler
#if defined(__ICC) || defined(__ICL)
  #define LBT_UNROLL(n)      _Pragma(LBT_TO_STRING(unroll (n)))
  #define LBT_FORCE_INLINE   __forceinline
  #define LBT_ALIGN          __attribute__((aligned(LBT_CACHE_LINE_SIZE))) alignas(LBT_CACHE_LINE_SIZE)
#elif defined(__clang__)
  #define LBT_UNROLL(n)      _Pragma(LBT_TO_STRING(unroll (16)))
  #define LBT_FORCE_INLINE   __attribute__((always_inline))
  #define LBT_ALIGN          __attribute__((aligned(LBT_CACHE_LINE_SIZE))) alignas(LBT_CACHE_LINE_SIZE)
#elif defined(__GNUC__) && !defined(__clang__)
  #define LBT_UNROLL(n)      _Pragma(LBT_TO_STRING(GCC unroll (16)))
  #define LBT_FORCE_INLINE   __attribute__((always_inline))
  #define LBT_ALIGN          __attribute__((aligned(LBT_CACHE_LINE_SIZE))) alignas(LBT_CACHE_LINE_SIZE)
#elif defined(_MSC_BUILD)
  #pragma message ("LB-t loop unrolling not supported: Microsoft Visual C++ (MSVC) detected")
  #define LBT_UNROLL(n)
  #define LBT_FORCE_INLINE   __forceinline
  #define LBT_ALIGN          alignas(LBT_CACHE_LINE_SIZE)
#else
  #warning "LB-t loop unrolling not supported: Unknown compiler"
  #define LBT_UNROLL(n)
  #warning "LB-t force inlining not supported: Unknown compiler"
  #define LBT_FORCE_INLINE
  #define LBT_ALIGN          alignas(LBT_CACHE_LINE_SIZE)
#endif


/// Define the type of arrays to be used
namespace lbt {
  template <typename T, std::size_t N>
  using array = std::array<T, N>;
}

#endif // LBT_TYPE_DEFINITIONS
