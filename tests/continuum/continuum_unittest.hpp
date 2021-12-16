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

    /**\class  CartesianProduct
     * \brief  Class for creating Cartesian product of two tuples
     *
     * \tparam T1   A tuple
     * \tparam T2   Another tuple
    */
    template <typename T1, typename T2>
    class CartesianProduct {
      protected:
        /**\fn     innerTuple
         * \brief  Helper function for creating inner tuple
         *
         * \tparam T    A single data type of T1 to be considered
         * \tparam Ts   The data types of T2
         * \return A tuple of tuples combining T and one of Ts
        */
        template <typename T, typename... Ts>
        static constexpr auto innerTuple(T const&, std::tuple<Ts...> const&) noexcept {
          // Not using 'std::make_tuple(std::declval<T>(), std::declval<Ts>()) ...' due to evaluated context in GCC
          return std::make_tuple(std::tuple<T,Ts>() ...);
        }

        /**\fn     outer
         * \brief  Helper function for creating outer loop
         *
         * \tparam Ts   All the data types of T1
         * \tparam T    The data types of T2
         * \return A tuple of tuples combining one of T1 and one of T2
        */
        template <typename... Ts, typename T>
        static constexpr auto outer(std::tuple<Ts...> const&, T const&) noexcept {
          // Not using 'innerTuple(std::declval<Ts>(), std::declval<T>()) ...' due to evaluated context in GCC
          return std::tuple_cat(innerTuple(Ts(), T()) ...);
        }

        /**\fn     toTestingTypes
         * \brief  Helper function for converting a tuple of classes to a testing types construct
         *
         * \tparam Ts   The different types
         * \return The classes as ::testing::Types
        */
        template <class... Ts>
        static constexpr auto toTestingTypes(std::tuple<Ts...>) noexcept {
          return ::testing::Types<Ts...>{};
        }
      public:
        using type = std::decay_t<decltype(toTestingTypes(outer(std::declval<T1>(), std::declval<T2>())))>;
    };

    /// Convenient alias
    template <typename T1, typename T2>
    using CartesianProduct_t = typename CartesianProduct<T1, T2>::type;
  }
}

// Test if Cartesian product of types works correctly
TEST(CartesianProductTest, typesEqual) {
  using TypesInner = std::tuple<int,double>;
  using TypesOuter = std::tuple<int,double, float>;
  using TypesResult = ::testing::Types<std::tuple<int,int>, std::tuple<int,double>, std::tuple<int,float>, 
                                       std::tuple<double,int>, std::tuple<double,double>, std::tuple<double,float>>;
  constexpr bool is_same = std::is_same_v<lbt::test::CartesianProduct_t<TypesInner, TypesOuter>, TypesResult>;
  EXPECT_TRUE(is_same);
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
