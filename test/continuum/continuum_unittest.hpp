#ifndef LBT_CONTINUUM_UNITTEST
#define LBT_CONTINUUM_UNITTEST
#pragma once

/**
 * \file     continuum_unittest.hpp
 * \mainpage Tests for continuum class
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <ostream>
#include <string>
#include <tuple>

#include <gtest/gtest.h>

#include "../../src/continuum/continuum.hpp"
#include "../utils/tuple_utils.hpp"


namespace lbt {
  namespace test {
    /**\class Streamable
     * \brief Class that can be streamed to an std::ostream
    */
    class Streamable {
      public:
        Streamable() = default;
        Streamable& operator = (Streamable&) = default;
        Streamable(Streamable&&) = default;
        Streamable& operator = (Streamable&&) = default;

        /**\fn        operator <<
         * \brief     The outstream operator for this class
         *
         * \param[in] os   The output stream
         * \param[in] The class itself
         * \return    The unmodified output stream
        */
        friend std::ostream& operator << (std::ostream& os, Streamable const&) noexcept {
          return os;
        }
    };

    /**\class NotStreamable
     * \brief Class that can't be streamed to an std::ostream
    */
    class NotStreamable {
      public:
        NotStreamable() = default;
        NotStreamable& operator = (NotStreamable&) = default;
        NotStreamable(NotStreamable&&) = default;
        NotStreamable& operator = (NotStreamable&&) = default;
    };
  }
}

/// Test streamable type trait
TEST(IsStreamableTrait, areStreamableDataTypesStreamable) {
  constexpr bool is_double_streamable = lbt::is_streamable_v<std::ostream, double>;
  EXPECT_TRUE(is_double_streamable);
  constexpr bool is_string_streamable = lbt::is_streamable_v<std::ostream, std::string>;
  EXPECT_TRUE(is_string_streamable);
  constexpr bool is_streamable_object_streamable = lbt::is_streamable_v<std::ostream, lbt::test::Streamable>;
  EXPECT_TRUE(is_streamable_object_streamable);
}
TEST(IsStreamableTrait, areNotStreamableDataTypesNotStreamable) {
  constexpr bool is_notstreamable_object_streamable = lbt::is_streamable_v<std::ostream, lbt::test::NotStreamable>;
  EXPECT_FALSE(is_notstreamable_object_streamable);
}

/// Tests conversion to string with optional precision
TEST(ToStringWithPrecisionTest, singleDigitToTwoDigitsDouble) {
  constexpr double val {0.1};
  constexpr std::int32_t digits {2};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_EQ(str, "0.10");
}
TEST(ToStringWithPrecisionTest, threeDigitsToTwoDigitsDouble) {
  constexpr double val {0.123};
  constexpr std::int32_t digits {2};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_EQ(str, "0.12");
}
TEST(ToStringWithPrecisionTest, singleDigitToThreeDigitsDouble) {
  constexpr double val {0.1};
  constexpr std::int32_t digits {3};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_EQ(str, "0.100");
}
TEST(ToStringWithPrecisionTest, fourDigitsToThreeDigitsDouble) {
  constexpr double val {0.1234};
  constexpr std::int32_t digits {3};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_EQ(str, "0.123");
}
TEST(ToStringWithPrecisionTest, singleDigitToTwoDigitsFloat) {
  constexpr float val {0.1f};
  constexpr std::int32_t digits {2};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_EQ(str, "0.10");
}
TEST(ToStringWithPrecisionTest, noDigitsInteger) {
  constexpr std::int32_t val {1};
  constexpr std::int32_t digits {3};
  auto const str = lbt::toString(val, digits);
  
  EXPECT_EQ(str, "1");
}

// Test setters and getters of continuum class

#endif // LBT_CONTINUUM_UNITTEST
