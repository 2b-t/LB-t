/**
 * \file     type_definitions.hpp
 * \mainpage Contains general type definitions depending on the compiler and operating system
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_TYPE_DEFINITIONS
#define LBT_TYPE_DEFINITIONS
#pragma once

#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <new>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
  #include <malloc.h>
#endif


/// Helper macros for stringification
#define LBT_TO_STRING_HELPER(X)   #X
#define LBT_TO_STRING(X)          LBT_TO_STRING_HELPER(X)

/// Cache-line alignment of arrays in Bytes
namespace lbt {
  #ifdef __cpp_lib_hardware_interference_size
    inline constexpr std::size_t alignment = std::hardware_constructive_interference_size;
  #else
    inline constexpr std::size_t alignment = 64;
  #endif
}

/// Definitions for unrolling, inlining and cache-alignment depending on compiler
#if defined(__ICC) || defined(__ICL)
  #define LBT_UNROLL(n)      _ Pragma(LBT_TO_STRING(unroll (n)))
  #define LBT_FORCE_INLINE   __forceinline
  #define LBT_ALIGN          __attribute__((aligned(lbt::alignment))) alignas(lbt::alignment)
#elif defined(__clang__)
  #define LBT_UNROLL(n)      _ Pragma(LBT_TO_STRING(unroll (n)))
  #define LBT_FORCE_INLINE   __attribute__((always_inline))
  #define LBT_ALIGN          __attribute__((aligned(lbt::alignment))) alignas(lbt::alignment)
#elif defined(__GNUC__) && !defined(__clang__)
  #define LBT_UNROLL(n)      _ Pragma(LBT_TO_STRING(GCC unroll (16)))
  #define LBT_FORCE_INLINE   __attribute__((always_inline))
  #define LBT_ALIGN          __attribute__((aligned(lbt::alignment))) alignas(lbt::alignment)
#elif defined(_MSC_BUILD)
  #pragma message ("LB-t loop unrolling not supported: Microsoft Visual C++ (MSVC) detected")
  #define LBT_UNROLL(n)
  #define LBT_FORCE_INLINE     forceinline
  #define LBT_ALIGN            alignas(lbt::alignment)
#else
  #warning "LB-t loop unrolling not supported: Unknown compiler"
  #define LBT_UNROLL(n)
  #warning "LB-t force inlining not supported: Unknown compiler"
  #define LBT_FORCE_INLINE
  #define LBT_ALIGN            alignas(lbt::alignment)
#endif


/// Define the type of arrays to be used
namespace lbt {
  // Convenient alias for stack-allocated arrays
  template <typename T, std::size_t N>
  using StackArray = std::array<T, N>;

  /**\fn        aligned_alloc
   * \brief     Wrapper for functions for allocation of aligned memory on the heap
   * 
   * \tparam    T           Data type of the resulting pointer
   * \param[in] N           Number of array elements
   * \param[in] alignment   Alignment size in Bytes
   * \return    Pointer to the first element of the allocated memory
  */
  template <typename T = void>
  T* aligned_alloc(std::size_t const N, std::size_t const alignment = lbt::alignment) noexcept {
    #ifdef _WIN32
      T* ptr = static_cast<T*>(_aligned_malloc(sizeof(T)*N, alignment));
    #else
      T* ptr = static_cast<T*>(std::aligned_alloc(alignment, sizeof(T)*N));
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

  /**\class  AlignedArray
   * \brief  Class for aligned heap-allocated array
   *
   * \tparam T   The data-type of the array
  */
  template <typename T>
  class AlignedArray {
    public:
      /**\fn        AlignedArray
       * \brief     Class constructor
       * 
       * \param[in] N           The number of elements that should be allocated
       * \param[in] alignment   The alignment that should be used for the heap-allocated array
      */
      AlignedArray(std::size_t const N, std::size_t const alignment = lbt::alignment) noexcept
        : N{N}, alignment{alignment}, ptr{aligned_alloc<T>(N, alignment)} {
        return;
      }
      AlignedArray() = delete;
      ~AlignedArray() noexcept {
        aligned_free(ptr);
        return;
      }
      AlignedArray(AlignedArray const& arr) noexcept
        : N{arr.N}, alignment{arr.alignment}, ptr{aligned_alloc<T>(N, alignment)} {
        std::memcpy(ptr, arr.ptr, sizeof(T)*N);
        return;
      }
      AlignedArray& operator= (AlignedArray const& arr) noexcept {
        this->N = arr.N;
        this->alignment = arr.alignment;
        this->ptr = aligned_alloc<T>(this->N, this->alignment);
        std::memcpy(ptr, arr.ptr, sizeof(T)*N);
        return *this;
      }
      AlignedArray(AlignedArray&& arr) noexcept
        : N{arr.N}, alignment{arr.alignment}, ptr{arr.ptr} {
        arr.ptr = nullptr;
        return; 
      }
      AlignedArray& operator= (AlignedArray&& arr) noexcept {
        this->N = arr.N;
        this->alignment = arr.alignment;
        this->ptr = arr.ptr;
        arr.ptr = nullptr;
        return *this; 
      }

      /**\fn        operator[]
       * \brief     Operator for accessing the \p i-th index of the array without checking the bounds
       * 
       * \param[in] i   The index of the array that should be accessed
       * \return    The i-th element of the underlying heap-allocated array
      */
      T const& operator[] (std::size_t const i) const noexcept {
        return ptr[i];
      }
      T& operator[] (std::size_t const i) noexcept {
        return ptr[i];
      }

      /**\fn        at
       * \brief     Function for accessing the \p i-th index of the array checking the bounds
       * 
       * \param[in] i   The index of the array that should be accessed
       * \return    The i-th element of the underlying heap-allocated array
      */
      T const& at(std::size_t const i) const {
        if (i < N) {
          return ptr[i];
        } else {
          std::stringstream ss {};
          ss << "Index i out of range (i >= N): " << i << " >= " << N << "!";
          throw std::out_of_range(ss.str());
        }
        
      }
      T& at(std::size_t const i) {
        if (i < N) {
          return ptr[i];
        } else {
          std::stringstream ss {};
          ss << "Index i out of range (i >= N): " << i << " >= " << N << "!";
          throw std::out_of_range(ss.str());
        }
      }

      /**\fn    size
       * \brief The size of the array
       * 
       * \return The number of elements inside the heap-allocated array
      */
      std::size_t size() const noexcept {
        return N;
      }

    protected:
      std::size_t N;
      std::size_t alignment;
      T* ptr;
  };

  // Convenient alias for heap-allocated arrays (possibly exchange with std::vector)
  template <typename T>
  using HeapArray = AlignedArray<T>;
}

#endif // LBT_TYPE_DEFINITIONS
