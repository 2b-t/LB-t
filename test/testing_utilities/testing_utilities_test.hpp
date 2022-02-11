/**
 * \file     testing_utilities_test.hpp
 * \mainpage Tests for utility tools related to googletest
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#ifndef LBT_TESTING_UTILITIES_UNITTEST
#define LBT_TESTING_UTILITIES_UNITTEST
#pragma once

#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

#include "testing_utilities.hpp"

namespace lbt {
  namespace test {

    // Test conversion to ::testing::Types
    TEST(ToTestingTypesConversionTest, typesEqual) {
      using SomeTuple = std::tuple<double,float>;
      using ExpectedTestingTypes = ::testing::Types<double,float>;
      constexpr bool is_same = std::is_same_v<ToTestingTypes_t<SomeTuple>,
                                              ExpectedTestingTypes>;
      EXPECT_TRUE(is_same);
    }

    TEST(TemplateDataTypeTest, doubleTupleTemplateType) {
      using Tup = std::tuple<double>;
      constexpr bool is_same = std::is_same_v<TemplateDataType_t<Tup>,
                                              double>;
      EXPECT_TRUE(is_same);
    }
  }
}

#endif // LBT_TESTING_UTILITIES_UNITTEST
