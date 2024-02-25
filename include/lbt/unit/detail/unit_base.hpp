/**
 * \file     unit_base.hpp
 * \mainpage Contains the base unit for all derived units
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_UNIT_BASE
#define LBT_UNIT_UNIT_BASE
#pragma once

#include <iostream>
#include <type_traits>


namespace lbt {
  namespace unit {

    namespace detail {

      /**\class UnitBase
       * \brief Base class for all physical units
       *        Uses a special form of the Curiously Recurring Template Pattern (CRTP) for
       *        not having to re-declare the different operators between the same physical units
       * \tparam The child class inheriting from UnitBase
      */
      template <typename T>
      class UnitBase {
        public:
          /// All the operators for the derived class

          /**\fn    operator-
           * \brief Operator for inverting sign
           * 
           * \param[in] a   The derived unit class
           * \return A new unit class of the same type holding a value with inverted sign
          */
          friend constexpr T operator- (T const& a) noexcept {
            return T{-a.value};
          }

          /**\fn    operator+
           * \brief Addition operator between two derived unit classes of the same type
           * 
           * \param[in] a   One derived unit class
           * \param[in] b   Another derived unit class
           * \return The sum of the two units
          */
          friend constexpr T operator+ (T const& a, T const& b) noexcept {
            return T{a.value + b.value};
          }

          /**\fn    operator-
           * \brief Subtraction operator between two derived unit classes of the same type
           * 
           * \param[in] a   One derived unit class
           * \param[in] b   Another derived unit class
           * \return The difference between the two units
          */
          friend constexpr T operator- (T const& a, T const& b) noexcept {
            return T{a.value - b.value};
          }

          /**\fn    operator/
           * \brief Division operator between two derived classes
           * 
           * \param[in] t1   One derived unit class
           * \param[in] t2   Another derived unit class
           * \return    The dimensionless resulting number
          */
          friend constexpr long double operator/ (T const& t1, T const& t2) noexcept {
            return t1.get()/t2.get();
          }

          /**\fn    operator*
           * \brief Multiplication operator between a constant and a derived class
           * 
           * \param[in] c   A floating number constant
           * \param[in] a   One derived unit class
           * \return The derived class scaled by the constant
          */
          friend constexpr T operator* (long double const c, T const& t) noexcept {
            return T{c*t.value};
          }
          friend constexpr T operator* (T const& t, long double const c) noexcept {
            return T{c*t.value};
          }

          /**\fn    operator/
           * \brief Division operator between a derived class and a constant
           * 
           * \param[in] t   One derived unit class
           * \param[in] c   A floating number constant
           * \return The derived class scaled by the constant
          */
          friend constexpr T operator/ (T const& t, long double const c) noexcept {
            return T{t.value/c};
          }

          /**\fn    operator<<
           * \brief Output stream operator between an output stream and the derived unit class
           * 
           * \param[in] out    The output stream
           * \param[in] t      One derived unit class
           * \return The output stream containing the outputted unit class
          */
          friend std::ostream& operator<< (std::ostream& os, T const& t) noexcept {
            os << t.value;
            return os;
          }

          /**\fn    set
           * \brief Setter for value stored inside
           * 
           * \param[in] Value stored inside the unit class given in its base unit
          */
          constexpr void set(long double const val) noexcept {
            value = val;
            return;
          }

          /**\fn    get
           * \brief Getter for value stored inside
           * 
           * \tparam T   The data type that should be returned from this function
           * \tparam Template parameter used for SFINAE of function only for arithmetic data types
           * \return Value stored inside the unit class in its base unit
          */
          template <typename U = long double, typename std::enable_if_t<std::is_arithmetic_v<U>>* = nullptr>
          constexpr auto get() const noexcept {
            return static_cast<U>(value);
          }

        protected:
          /**\fn    UnitBase
           * \brief Constructor of base class for all physical units
           * 
           * \param[in] value   The value to be stored inside the class
          */
          constexpr UnitBase(long double const value) noexcept 
            : value{value} {
            return;
          }
          UnitBase(UnitBase const&) = default;
          UnitBase& operator= (UnitBase const&) = default;
          UnitBase(UnitBase&&) = default;
          UnitBase& operator= (UnitBase&&) = default;

          template <typename U> friend class UnitBase; // Currently unused, maybe useful in the future

          long double value; // Stored unit value
      };

      constexpr std::false_type is_unit_impl(...) noexcept;
      template <typename T>
      constexpr std::true_type is_unit_impl(UnitBase<T> const volatile&) noexcept;
    }

    /**\fn     is_unit
     * \brief  Type trait for checking if a type \p T is derived from the UnitBase template class
     *
     * \tparam T   The data type to be checked to be a unit
    */
    template <typename T>
    using is_unit = decltype(lbt::unit::detail::is_unit_impl(std::declval<T&>()));
    // Convient alias for is_unit<T>::value
    template <typename T>
    static constexpr bool is_unit_v = is_unit<T>::value;

  }
}

#endif // LBT_UNIT_UNIT_BASE
