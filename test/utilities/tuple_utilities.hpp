#ifndef LBT_TUPLE_UTILITIES
#define LBT_TUPLE_UTILITIES
#pragma once

/**
 * \file     tuple_utilities.hpp
 * \mainpage Utility tools for std::tuple for convenient unit-testing
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>


namespace lbt {
  namespace test {
    // Implementation of is_tuple type trait
    namespace detail {
      constexpr std::false_type is_tuple_impl(...) noexcept;
      template <typename... Ts>
      constexpr std::true_type is_tuple_impl(std::tuple<Ts...> const volatile&) noexcept;
    }
    /**\fn     is_tuple
     * \brief  Type trait for checking if a type \p T is an std::tuple of arbitrary types
     *
     * \tparam T   The data type to be checked to be an std::tuple
    */
    template <typename T>
    using is_tuple = decltype(detail::is_tuple_impl(std::declval<T&>()));
    // Convient alias for is_tuple<T>::value
    template <typename T>
    static constexpr bool is_tuple_v = is_tuple<T>::value;

    /**\class  CartesianProduct
     * \brief  Class for creating Cartesian product of two tuples
     *
     * \tparam T1   A tuple
     * \tparam T2   Another tuple
     * \tparam Dummy argument used for SFINAE
     * \tparam Dummy argument used for SFINAE
    */
    template <typename T1, typename T2,
              typename std::enable_if_t<is_tuple_v<T1>>* = nullptr,
              typename std::enable_if_t<is_tuple_v<T2>>* = nullptr>
    class CartesianProduct {
      protected:
        CartesianProduct() = delete;
        CartesianProduct(CartesianProduct const&) = delete;
        CartesianProduct(CartesianProduct&&) = delete;
        CartesianProduct& operator=(CartesianProduct const&) = delete;
        CartesianProduct& operator=(CartesianProduct&&) = delete;

        /**\fn     innerHelper
         * \brief  Helper function for creating inner tuple
         *
         * \tparam T    A single data type of T1 to be considered
         * \tparam Ts   The data types of T2
         * \return A tuple of tuples combining T and one of Ts
        */
        template <typename T, typename... Ts>
        static constexpr auto innerHelper(T&&, std::tuple<Ts...>&&) noexcept
        -> decltype(std::make_tuple(std::make_tuple(std::declval<T>(), std::declval<Ts>()) ...));

        /**\fn     outerHelper
         * \brief  Helper function for creating outer loop
         *
         * \tparam Ts   All the data types of T1
         * \tparam T    The data types of T2
         * \return A tuple of tuples combining one of T1 and one of T2
        */
        template <typename... Ts, typename T>
        static constexpr auto outerHelper(std::tuple<Ts...>&&, T&&) noexcept 
        -> decltype(std::tuple_cat(innerHelper(std::declval<Ts>(), std::declval<T>()) ...));

      public:
        using type = decltype(outerHelper(std::declval<T1>(), std::declval<T2>()));
    };
    /// Convenient alias
    template <typename T1, typename T2>
    using CartesianProduct_t = typename CartesianProduct<T1, T2>::type;

    /**\class  CartesianProductApply
     * \brief  Class for creating product of tuples
     *
     * \tparam TC   Templated tuple of class-types to be instantiated with the data types \p TT
     * \tparam TT   Tuple containing the different types that \p C should be instantiated with
     * \tparam Dummy argument used for SFINAE
    */
    template <template <typename> class TC, typename TT,
              typename std::enable_if_t<is_tuple_v<TT>>* = nullptr>
    class CartesianProductApply {
      protected:
        CartesianProductApply() = delete;
        CartesianProductApply(CartesianProductApply const&) = delete;
        CartesianProductApply(CartesianProductApply&&) = delete;
        CartesianProductApply& operator=(CartesianProductApply const&) = delete;
        CartesianProductApply& operator=(CartesianProductApply&&) = delete;

        /**\fn     helper
         * \brief  Helper function for creating the cartesian product of the class \p C and the types \p Ts
         *
         * \tparam Ts   The different types that the class \p C should be instantiated with
         * \tparam Dummy argument used for SFINAE
         * \return The merged tuple
        */
        template <typename... Ts,
                  typename std::enable_if_t<(std::is_default_constructible_v<TC<Ts>> && ...)>* = nullptr>
        static constexpr auto helper(std::tuple<Ts...>&&) noexcept
        -> decltype(std::tuple_cat(std::declval<TC<Ts>>() ...));

      public:
        using type = decltype(helper(std::declval<TT>()));
    };
    /// Convenient alias
    template<template <typename> class TC, typename... Ts>
    using CartesianProductApply_t = typename CartesianProductApply<TC,Ts...>::type;

    /**\class  ToTestingTypes
     * \brief  Helper class for converting a tuple of classes to a testing types construct
     *
     * \tparam T   The tuple of classes
    */
    template <class T>
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

#endif // LBT_TUPLE_UTILITIES
