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
         * \brief  Helper method for converting a tuple of classes to a testing types construct
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


    /**\class  TemplateDataType
     * \brief  Helper class for determining template argument of a class with a single template argument
     *
     * \tparam C   The class that the template argument should be determined of
    */
    template <typename C>
    struct TemplateDataType {
      protected:
        TemplateDataType() = delete;
        TemplateDataType(TemplateDataType const&) = delete;
        TemplateDataType(TemplateDataType&&) = delete;
        TemplateDataType& operator=(TemplateDataType const&) = delete;
        TemplateDataType& operator=(TemplateDataType&&) = delete;

        /**\fn      helper
         * \brief   Helper method for determing template argument type
         * \warning Can only called in unevaluated context!
         *
         * \tparam  TC   The class template
         * \tparam  T    The class template's template argument type
         * \return  A value with the type of the template argument
        */
        template <template <typename T> class TC, typename T>
        static constexpr auto helper(TC<T> const&) -> std::decay_t<decltype(std::declval<T>())>;
      public:
        using type = decltype(helper(std::declval<C>()));
    };
    /// Convenient alias
    template <typename C>
    using TemplateDataType_t = typename TemplateDataType<C>::type;

  }
}

#endif // LBT_TESTING_UTILITIES
