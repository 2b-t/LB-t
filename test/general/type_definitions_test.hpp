#ifndef LBT_TYPE_DEFINITIONS_UNITTEST
#define LBT_TYPE_DEFINITIONS_UNITTEST
#pragma once

/**\file     type_defintions_test.hpp
 * \mainpage Contains unit-tests for type and function definitions that depend on the compiler
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <type_traits>

#include <gtest/gtest.h>

#include "../../src/general/type_definitions.hpp"
#include "../testing_utilities/array_utilities.hpp"


namespace lbt {
  namespace test {

    TEST(StackArrayTest, initialisation) {
      constexpr lbt::StackArray<int,3> arr {1, 2, 3};
      EXPECT_EQ(arr[0], 1);
      EXPECT_EQ(arr[1], 2);
      EXPECT_EQ(arr[2], 3);
    }
    TEST(StackArrayTest, accessOperator) {
      constexpr std::size_t N {21};
      lbt::StackArray<int,N> arr {};
      for (std::size_t i = 0; i < N; ++i) {
        arr[i] = static_cast<int>(i)+1;
      }
      for (std::size_t i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], static_cast<int>(i)+1);
      }
    }
    TEST(StackArrayTest, size) {
      constexpr std::size_t N {21};
      constexpr lbt::StackArray<int,N> arr {};
      EXPECT_EQ(arr.size(), N);
    }

    class AlignedAllocTestHelper : public ::testing::Test, public ::testing::WithParamInterface<std::int64_t> {
    };
    TEST_P(AlignedAllocTestHelper, alignment) {
      auto const alignment {GetParam()};
      constexpr std::int64_t memory_size {10};
      auto const ptr {lbt::aligned_alloc<int>(memory_size, alignment)};
      bool const is_aligned {isAligned(ptr,alignment)};
      EXPECT_TRUE(is_aligned);
      lbt::aligned_free(ptr);
    }
    INSTANTIATE_TEST_SUITE_P(AlignedAllocTest, AlignedAllocTestHelper, ::testing::Values(8,16,32,64,128));

    TEST(AlignedAllocTest, dataType) {
      auto const alignment {64};
      constexpr std::int64_t memory_size {10};
      auto const ptr {lbt::aligned_alloc<int>(memory_size, alignment)};
      bool const is_same {std::is_same_v<std::remove_const_t<decltype(ptr)>,int*>};
      EXPECT_TRUE(is_same);
      lbt::aligned_free(ptr);
    }

    TEST(AlignedArrayTest, initialisation) {
      constexpr std::size_t N {21};
      lbt::AlignedArray<int> arr (N);
      for (std::size_t i = 0; i < N; ++i) {
        arr[i] = static_cast<int>(i)+1;
      }
      for (std::size_t i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], static_cast<int>(i)+1);
      }
    }
    TEST(AlignedArrayTest, size) {
      constexpr std::size_t N {21};
      lbt::AlignedArray<int> arr (N);
      EXPECT_EQ(arr.size(), N);
    }

    TEST(HeapArrayTest, initialisation) {
      constexpr std::size_t N {21};
      lbt::HeapArray<int> arr (N);
      for (std::size_t i = 0; i < N; ++i) {
        arr[i] = static_cast<int>(i)+1;
      }
      for (std::size_t i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], static_cast<int>(i)+1);
      }
    }
    TEST(HeapArrayTest, size) {
      constexpr std::size_t N {21};
      lbt::HeapArray<int> arr (N);
      EXPECT_EQ(arr.size(), N);
    }
  }
}

#endif // LBT_TYPE_DEFINITIONS_UNITTEST
