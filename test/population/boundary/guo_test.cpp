/**
 * \file     guo_test.hpp
 * \mainpage Tests for helper functions for boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <type_traits>

#include <gtest/gtest.h>

#include "lbt/population/boundary/guo.hpp"


namespace lbt {
  namespace test {

    TEST(GetComponentTest, tangential) {
      constexpr std::int32_t a {1};
      constexpr std::int64_t b {2};
      auto const expected_result {a};
      auto const result = lbt::boundary::getComponent<0>(a,b);
      bool const is_same = std::is_same_v<decltype(result),decltype(expected_result)>;
      EXPECT_EQ(result, expected_result);
      EXPECT_TRUE(is_same);
    }
    TEST(GetComponentTest, normal) {
      constexpr std::int32_t a {1};
      constexpr std::int64_t b {2};
      auto const expected_result {b};
      auto const result = lbt::boundary::getComponent<1>(a,b);
      bool const is_same = std::is_same_v<decltype(result),decltype(expected_result)>;
      EXPECT_EQ(result, expected_result);
      EXPECT_TRUE(is_same);
    }

    // TODO: Test velocity and pressure boundary conditions for all values if correct values
    // Potentially separate density and velocity values

  }
}
