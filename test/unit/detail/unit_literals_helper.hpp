/**
 * \file     unit_literals_helper.hpp
 * \mainpage Contains a helper class for setting up the unit literals tests
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_LITERALS_HELPER
#define LBT_UNIT_LITERALS_HELPER
#pragma once

#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

#include "lbt/unit/detail/unit_base.hpp"


namespace lbt {
  namespace literals {
    namespace test {

      /**\class UnitLiteralsHelper
       * \brief Helper class for parametrised unit with parameter interface
       * 
       * \tparam T   The type of physical unit (e.g. lbt::unit::Length)
       * \tparam Dummy parameter used for SFINAE
      */
      template <typename T, typename std::enable_if_t<lbt::unit::is_unit_v<T>>* = nullptr>
      class UnitLiteralsHelper : public testing::Test, public ::testing::WithParamInterface<std::pair<T,long double>> {
      };

    }
  }
}

#endif // LBT_UNIT_LITERALS_HELPER
