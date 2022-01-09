#ifndef LBT_TYPE_DEFINITIONS
#define LBT_TYPE_DEFINITIONS
#pragma once

/**\file     type_definitions.hpp
 * \mainpage Contains general type definitions depending on the compiler and operating system
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>

#ifdef _WIN32
  #include <malloc.h>
#endif


/// Helper macros for stringification
#define LBT_TO_STRING_HELPER(X)   #X
#define LBT_TO_STRING(X)          LBT_TO_STRING_HELPER(X)

/// Cache-line alignment of arrays in Bytes
#define LBT_CACHE_LINE_SIZE    64

/// Definitions for unrolling, inlining and cache-alignment depending on compiler
#if defined(__ICC) || defined(__ICL)
  #define LBT_UNROLL(n)      _ Pragma(LBT_TO_STRING(unroll (n)))
  #define LBT_FORCE_INLINE   __forceinline
  #define LBT_ALIGN          __attribute__((aligned(LBT_CACHE_LINE_SIZE))) alignas(LBT_CACHE_LINE_SIZE)
#elif defined(__clang__)
  #define LBT_UNROLL(n)      _ Pragma(LBT_TO_STRING(unroll (16)))
  #define LBT_FORCE_INLINE   __attribute__((always_inline))
  #define LBT_ALIGN          __attribute__((aligned(LBT_CACHE_LINE_SIZE))) alignas(LBT_CACHE_LINE_SIZE)
#elif defined(__GNUC__) && !defined(__clang__)
  #define LBT_UNROLL(n)      _ Pragma(LBT_TO_STRING(GCC unroll (16)))
  #define LBT_FORCE_INLINE   __attribute__((always_inline))
  #define LBT_ALIGN          __attribute__((aligned(LBT_CACHE_LINE_SIZE))) alignas(LBT_CACHE_LINE_SIZE)
#elif defined(_MSC_BUILD)
  #pragma message ("LB-t loop unrolling not supported: Microsoft Visual C++ (MSVC) detected")
  #define LBT_UNROLL(n)
  #define LBT_FORCE_INLINE     forceinline
  #define LBT_ALIGN            alignas(LBT_CACHE_LINE_SIZE)
#else
  #warning "LB-t loop unrolling not supported: Unknown compiler"
  #define LBT_UNROLL(n)
  #warning "LB-t force inlining not supported: Unknown compiler"
  #define LBT_FORCE_INLINE
  #define LBT_ALIGN            alignas(LBT_CACHE_LINE_SIZE)
#endif


/// Define the type of arrays to be used
namespace lbt {
  // Alias for arrays
  template <typename T, std::size_t N>
  using array = std::array<T, N>;

  /**\fn        aligned_alloc
   * \brief     Wrapper for functions for allocation of aligned memory on the heap
   * 
   * \tparam    T             Data type of the resulting pointer
   * \param[in] memory_size   Size of the memory to be allocated in Bytes
   * \param[in] alignment     Alignment size in Bytes
   * \return    Pointer to the first element of the allocated memory
  */
  template <typename T = void>
  T* aligned_alloc(std::int64_t const memory_size, std::int64_t const alignment = LBT_CACHE_LINE_SIZE) noexcept {
    #ifdef _WIN32
      T* ptr = static_cast<T*>(_aligned_malloc(static_cast<std::size_t>(memory_size), static_cast<std::size_t>(alignment)));
    #else
      T* ptr = static_cast<T*>(std::aligned_alloc(static_cast<std::size_t>(alignment), static_cast<std::size_t>(memory_size)));
    #endif

    if (ptr == nullptr) {
      std::cerr << "Fatal error: Aligned memory could not be allocated!" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    return ptr;
  }

  /**\fn        aligned_free
   * \brief     Wrapper for functions for freeing aligned heap memory
   * 
   * \param[in] ptr   The pointer to the heap-allocated memory that should be freed
  */
  void aligned_free(void* ptr) noexcept {
    #ifdef _WIN32
      _aligned_free(ptr);
    #else
      std::free(ptr);
    #endif
    return;
  }
}

#endif // LBT_TYPE_DEFINITIONS
