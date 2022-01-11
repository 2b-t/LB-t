#ifndef LBT_TYPE_DEFINITIONS_UNITTEST
#define LBT_TYPE_DEFINITIONS_UNITTEST
#pragma once

/**\file     type_defintions_unittest.hpp
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

    // TODO: Add very basic tests for stack-allocated arrays: initialisation, size, at, operator []

    // TODO: Look for better names for these tests
    class AlignedArrayTest : public ::testing::Test, public ::testing::WithParamInterface<std::int64_t> {
      public:
    };
    TEST_P(AlignedArrayTest, alignment) {
      auto const alignment {GetParam()};
      constexpr std::int64_t memory_size {10};
      auto const ptr {lbt::aligned_alloc<int>(memory_size, alignment)};
      bool const is_aligned {isAligned(ptr,alignment)};
      EXPECT_TRUE(is_aligned);
      lbt::aligned_free(ptr);
    }
    INSTANTIATE_TEST_SUITE_P(AlignedHeapAllocatedArrayTest, AlignedArrayTest, ::testing::Values(8,16,32,64,128));

    TEST(AlignedHeapAllocatedArrayTest, dataType) {
      auto const alignment {64};
      constexpr std::int64_t memory_size {10};
      auto const ptr {lbt::aligned_alloc<int>(memory_size, alignment)};
      bool const is_same {std::is_same_v<std::remove_const_t<decltype(ptr)>,int*>};
      EXPECT_TRUE(is_same);
      lbt::aligned_free(ptr);
    }

    // TODO: Test aligned array allocation
    // TODO: Test copy and move constructor
    // TODO: Test operator[]
    // TODO: Test size()

    // TODO: Add very basic tests for heap-allocated arrays: initialisation, size, at, operator []
  }
}

#endif // LBT_TYPE_DEFINITIONS_UNITTEST
