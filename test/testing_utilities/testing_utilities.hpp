#ifndef LBT_TESTING_UTILITIES
#define LBT_TESTING_UTILITIES
#pragma once

/**
 * \file     testing_utilities.hpp
 * \mainpage Utility tools related to googletest
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <type_traits>

#include <gtest/gtest.h>

#include "../../src/general/tuple_utilities.hpp"


namespace lbt {
  namespace test {
    /**\class  ToTestingTypes
     * \brief  Helper class for converting a tuple of classes to a testing types construct
     *
     * \tparam T   The tuple of classes
     * \tparam Dummy parameter used for SFINAE
    */
    template <class T,
              typename std::enable_if_t<lbt::is_tuple_v<T>>* = nullptr>
    class ToTestingTypes {
      protected:
        ToTestingTypes() = delete;
        ToTestingTypes(ToTestingTypes const&) = delete;
        ToTestingTypes(ToTestingTypes&&) = delete;
        ToTestingTypes& operator=(ToTestingTypes const&) = delete;
        ToTestingTypes& operator=(ToTestingTypes&&) = delete;

        /**\fn     helper
         * \brief  Helper function for converting a tuple of classes to a testing types construct
         *
         * \tparam Ts   The different types
         * \return The classes inside a ::testing::Types type list
        */
        template <typename... Ts>
        static constexpr auto helper(std::tuple<Ts...>&&) noexcept {
          return ::testing::Types<Ts...>{};
        }

      public:
        using type = decltype(helper(std::declval<T>()));
    };
    /// Convenient alias
    template<typename T>
    using ToTestingTypes_t = typename ToTestingTypes<T>::type;
  }
}

#endif // LBT_TESTING_UTILITIES
