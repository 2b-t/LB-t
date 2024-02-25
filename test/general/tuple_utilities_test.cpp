/**
 * \file     tuple_utilities_test.cpp
 * \mainpage Tests for utility tools for handling tuples
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <tuple>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

#include "lbt/general/tuple_utilities.hpp"


namespace lbt {
  namespace test {

    /**\class  SomeTemplateClass
     * \brief  A variadically templated helper class
     *
     * \tparam Ts   Arbitrary data types
    */
    template <typename... Ts>
    class SomeTemplateClass {
      protected:
        SomeTemplateClass() = delete;
        SomeTemplateClass(SomeTemplateClass const&) = delete;
        SomeTemplateClass(SomeTemplateClass&&) = delete;
        SomeTemplateClass& operator=(SomeTemplateClass const&) = delete;
        SomeTemplateClass& operator=(SomeTemplateClass&&) = delete;
    };

    // Test custom tuple trait
    TEST(IsTupleTraitTest, tupleIsTuple) {
      constexpr bool is_tuple = lbt::is_tuple_v<std::tuple<int,double>>;
      EXPECT_TRUE(is_tuple);
    }

    TEST(IsTupleTraitTest, integerIsNotTuple) {
      constexpr bool is_tuple = lbt::is_tuple_v<int>;
      EXPECT_FALSE(is_tuple);
    }

    TEST(IsTupleTraitTest, pairIsNotTuple) {
      constexpr bool is_tuple = lbt::is_tuple_v<std::pair<int,double>>;
      EXPECT_FALSE(is_tuple);
    }

    TEST(IsTupleTraitTest, someTemplateClassIsNotTuple) {
      constexpr bool is_tuple = lbt::is_tuple_v<lbt::test::SomeTemplateClass<int,double>>;
      EXPECT_FALSE(is_tuple);
    }

    // Test if Cartesian product of types works correctly
    TEST(CartesianProductTest, typesEqual) {
      using TypesInner = std::tuple<int,double>;
      using TypesOuter = std::tuple<int,double,float>;
      using TypesResult = std::tuple<std::tuple<int,int>, std::tuple<int,double>, std::tuple<int,float>, 
                                    std::tuple<double,int>, std::tuple<double,double>, std::tuple<double,float>>;
      constexpr bool is_same = std::is_same_v<lbt::CartesianProduct_t<TypesInner, TypesOuter>, TypesResult>;
      EXPECT_TRUE(is_same);
    }

    // Test if Cartesian product of types works correctly
    template <typename T>
    using CartesianProductClassTemplate = std::tuple<std::tuple<T>, std::tuple<T, T>>;
    TEST(CartesianProductApplyTest, typesEqual) {
      using TemplateDataTypes = std::tuple<double,float>;
      using ExpectedDataType = std::tuple<std::tuple<double>, std::tuple<double,double>, 
                                          std::tuple<float>,  std::tuple<float,float>>;
      constexpr bool is_same = std::is_same_v<lbt::CartesianProductApply_t<CartesianProductClassTemplate, TemplateDataTypes>,
                                              ExpectedDataType>;
      EXPECT_TRUE(is_same);
    }

  }
}
