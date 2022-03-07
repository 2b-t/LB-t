/**
 * \file     testing_types.hpp
 * \mainpage Type defintions for constexpr mathematic tests.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_TESTING_TYPES
#define LBT_CEM_TESTING_TYPES
#pragma once

#include <cstdint>

#include <gtest/gtest.h>


namespace lbt {
  namespace test {

    /// Define testing data types
    using IntegerDataTypes = ::testing::Types<std::int8_t,  std::int16_t,  std::int32_t,  std::int64_t, 
                                              std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>;

    using FloatingPointDataTypes = ::testing::Types<float, double>;

    using ArithmeticDataTypes = ::testing::Types<std::int8_t,  std::int16_t,  std::int32_t,  std::int64_t, 
                                                 std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t,
                                                 float, double>;

  }
}

#endif // LBT_CEM_TESTING_TYPES
