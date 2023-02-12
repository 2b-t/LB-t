/**
 * \file     is_almost_equal_ulps.hpp
 * \brief    Constexpr implementation of a floating number point comparison based on units in the last place (ULPs)
 * \mainpage Contains traits and a constexpr function for comparing two floating point numbers based on units 
 *           in the last place (ULP) similar to the implementation in GoogleTest but more modular and constexpr:
 *           https://github.com/google/googletest/blob/main/googletest/include/gtest/internal/gtest-internal.h
 * \warning  Requires C++20 or later for compilation
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IS_ALMOST_EQUAL_ULPS
#define LBT_CEM_IS_ALMOST_EQUAL_ULPS
#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>


namespace lbt {
  namespace cem {

    namespace detail {
      /// Trait for excluding incomplete types: See https://stackoverflow.com/a/44229779/9938686
      //  Warning: Has to be a pointer and not a reference!
      template <typename T, std::size_t = sizeof(T)>
      std::true_type is_complete(T*);

      std::false_type is_complete(...);
    }

    template <typename T>
    using is_complete = decltype(detail::is_complete(std::declval<T*>()));

    template <typename T>
    static constexpr bool is_complete_v = is_complete<T>::value;

    namespace detail {

      /**\class  UIntEquiv
       * \brief  Class for determining the corresponding unsigned integer type with equal length
       * 
       * \note   Potentially add specialisation of UIntEquiv for longer unsigned integer types here
       *         e.g. GCC's __uint128_t: https://gcc.gnu.org/onlinedocs/gcc/_005f_005fint128.html
       *         or Boost: https://www.boost.org/doc/libs/1_67_0/libs/multiprecision/doc/html/boost_multiprecision/tut/ints/cpp_int.html
       *
       * \tparam N   Size of the length of the data type in bytes
      */
      template <std::size_t N>
      class UIntEquiv {
        protected:
          /// Just a trait: Should not be necessary to instantiate this class, the important part is the type member variable
          UIntEquiv() = delete;
          UIntEquiv(UIntEquiv const&) = delete;
          UIntEquiv(UIntEquiv&&) = delete;
          UIntEquiv& operator= (UIntEquiv const&) = delete;
          UIntEquiv& operator= (UIntEquiv&&) = delete;

          template<std::size_t M, typename std::enable_if_t<(M==sizeof(std::uint8_t))>* = nullptr>
          static constexpr std::uint8_t determineUIntEquivalent() noexcept;

          template<std::size_t M, typename std::enable_if_t<(M==sizeof(std::uint16_t))>* = nullptr>
          static constexpr std::uint16_t determineUIntEquivalent() noexcept;

          template<std::size_t M, typename std::enable_if_t<(M==sizeof(std::uint32_t))>* = nullptr>
          static constexpr std::uint32_t determineUIntEquivalent() noexcept;

          template<std::size_t M, typename std::enable_if_t<(M==sizeof(std::uint64_t))>* = nullptr>
          static constexpr std::uint64_t determineUIntEquivalent() noexcept;

        public:
          using type = decltype(determineUIntEquivalent<N>());
      };

      template <std::size_t N>
      using UIntEquiv_t = typename UIntEquiv<N>::type;

      /**\class  FloatTrait
       * \brief  Class for determining the corresponding unsigned integer type with equal length
       * 
       * \note   You can provide specialisations of this trait for proprietary floating types that do not respect IEEE754
       *         e.g. Boost float128: https://www.boost.org/doc/libs/1_65_1/libs/multiprecision/doc/html/boost_multiprecision/tut/floats/float128.html)
       *
       * \tparam T   Floating data type
      */
      template <typename T>
      class FloatTrait;

      /// Specialised trait for floating point number types respecting IEEE754
      template <typename T>
      requires std::is_floating_point_v<T> && std::numeric_limits<T>::is_iec559 && (std::endian::native == std::endian::little)
      class FloatTrait<T> {
        public:
          static constexpr std::size_t number_of_bytes {sizeof(T)};
          static constexpr std::size_t number_of_bits {number_of_bytes*std::numeric_limits<std::uint8_t>::digits};
          using Bytes = UIntEquiv_t<number_of_bytes>;

          static constexpr std::size_t number_of_sign_bits {1};
          static constexpr std::size_t number_of_fraction_bits {std::numeric_limits<T>::digits-1};
          static constexpr std::size_t number_of_exponent_bits {number_of_bits - number_of_sign_bits - number_of_fraction_bits};

          static constexpr Bytes sign_mask {Bytes{1} << (number_of_bits - 1)};
          static constexpr Bytes fraction_mask {~Bytes{0} >> (number_of_exponent_bits + 1)};
          static constexpr Bytes exponent_mask {~(sign_mask | fraction_mask)};

          /**\fn        isNan
           * \brief     Function for detecting if a certain floating point number represents a Not a Number value according to the IEEE754 standard
           * \warning   The constexpr casting of bits (e.g. with std::bit_cast) requires C++20 or later!
           * 
           * \param[in] t   The floating point number to be checked for NaN
           * \return    True if the floating point number corresponds to NaN according to the IEEE754 standard, false else
          */
          static constexpr bool isNan(T const t) noexcept {
            auto const bytes {std::bit_cast<Bytes>(t)};
            auto const exponent_bytes {extractExponent(bytes)};
            auto const fraction_bytes {extractFraction(bytes)};
            return (exponent_bytes == exponent_mask) && (fraction_bytes != 0);
          }

          /**\fn        isPosInf
           * \brief     Function for detecting if a certain floating point number represents positive infinity according to the IEEE754 standard
           * 
           * \param[in] t   The floating point number to be checked for positive infinity
           * \return    True if the floating point number corresponds to positive infinity according to the IEEE754 standard, false else
          */
          static constexpr bool isPosInf(T const t) noexcept {
            return isPos(t) && isInf(t);
          }

          /**\fn        isNegInf
           * \brief     Function for detecting if a certain floating point number represents negative infinity according to the IEEE754 standard
           * 
           * \param[in] t   The floating point number to be checked for negative infinity
           * \return    True if the floating point number corresponds to negative infinity according to the IEEE754 standard, false else
          */
          static constexpr bool isNegInf(T const t) noexcept {
            return isNeg(t) && isInf(t);
          }

          /**\fn        isNeg
           * \brief     Function for detecting if a certain floating point number is negative according to the IEEE754 standard
           * \warning   The constexpr casting of bits (e.g. with std::bit_cast) requires C++20 or later!
           * 
           * \param[in] t   The floating point number to be checked if negative
           * \return    True if the floating point number is negative according to the IEEE754 standard, false else
          */
          static constexpr bool isNeg(T const t) noexcept {
            auto const bytes {std::bit_cast<Bytes>(t)};
            auto const sign_bytes {extractSign(bytes)};
            return sign_bytes != 0;
          }

          /**\fn        isPos
           * \brief     Function for detecting if a certain floating point number is positive according to the IEEE754 standard
           * \warning   The constexpr casting of bits (e.g. with std::bit_cast) requires C++20 or later!
           * 
           * \param[in] t   The floating point number to be checked if positive
           * \return    True if the floating point number is positive according to the IEEE754 standard, false else
          */
          static constexpr bool isPos(T const t) noexcept {
            auto const bytes {std::bit_cast<Bytes>(t)};
            auto const sign_bytes {extractSign(bytes)};
            return sign_bytes == 0;
          }

          /**\fn        isInf
           * \brief     Function for detecting if a certain floating point number represents positive or negative infinity according to the IEEE754 standard
           * \warning   The constexpr casting of bits (e.g. with std::bit_cast) requires C++20 or later!
           * 
           * \param[in] t   The floating point number to be checked for infinity
           * \return    True if the floating point number corresponds to positive or negative infinity according to the IEEE754 standard, false else
          */
          static constexpr bool isInf(T const t) noexcept {
            auto const bytes {std::bit_cast<Bytes>(t)};
            auto const exponent_bytes {extractExponent(bytes)};
            auto const fraction_bytes {extractFraction(bytes)};
            return (exponent_bytes == exponent_mask) && (fraction_bytes == 0);
          }

          /**\fn        extractSign
           * \brief     Function for extracting the sign bit from given bytes by overlaying a bitmask
           * 
           * \param[in] bytes   The bytes where the sign should be extracted from
           * \return    A byte sequence of the same type as the input byte type but where the bits not corresponding to the sign are replaced by zeros
          */
          static constexpr Bytes extractSign(Bytes const bytes) noexcept {
            return bytes & sign_mask;
          }

          /**\fn        extractExponent
           * \brief     Function for extracting the exponent bytes from given bytes by overlaying a bitmask
           * 
           * \param[in] bytes   The bytes where the exponent should be extracted from
           * \return    A byte sequence of the same type as the input byte type but where the bits not corresponding to the exponent are replaced by zeros
          */
          static constexpr Bytes extractExponent(Bytes const bytes) noexcept {
            return bytes & exponent_mask;
          }

          /**\fn        extractFraction
           * \brief     Function for extracting the fraction (mantissa) bytes from given bytes by overlaying a bitmask
           * 
           * \param[in] bytes   The bytes where the fraction should be extracted from
           * \return    A byte sequence of the same type as the input byte type but where the bits not corresponding to the fraction are replaced by zeros
          */
          static constexpr Bytes extractFraction(Bytes const bytes) noexcept {
            return bytes & fraction_mask;
          }

        protected:
          /// Just a trait: Should not be necessary to instantiate this class, the interface are the static methods
          FloatTrait() = delete;
          FloatTrait(FloatTrait const&) = delete;
          FloatTrait(FloatTrait&&) = delete;
          FloatTrait& operator= (FloatTrait const&) = delete;
          FloatTrait& operator= (FloatTrait&&) = delete;
      };

      /**\class  FloatView
       * \brief  Class for comparing two floating point numbers by creating a sign magnitude comparison and looking at the unit in last place (ULPs)
       *
       * \tparam T   Floating data type
      */
      template <typename T>
      requires is_complete_v<FloatTrait<T>> && is_complete_v<UIntEquiv_t<sizeof(T)>>
      class FloatView {
        public:
          using Trait = FloatTrait<T>;
          using Bytes = UIntEquiv_t<sizeof(T)>;

          /**\fn        FloatView
           * \brief     Class constructor from given floating point number
           * 
           * \param[in] t   The floating point number to be inspected on a bit-level
          */
          explicit constexpr FloatView(T const t) noexcept
            : value{t} {
            return;
          }
          FloatView() = default;
          FloatView(FloatView const&) = default;
          FloatView(FloatView&&) = default;
          FloatView& operator= (FloatView const&) = default;
          FloatView& operator= (FloatView&&) = default;

          /**\fn        isAlmostEqual
           * \brief     Function for comparing if the floating-point number represented by the float view is equal to another representation according to the
           *            units in last place (ULPs) criterion https://en.wikipedia.org/wiki/Unit_in_the_last_place
           * 
           * \param[in] rhs            The floating point number that the internally saved number should be compared to for equality
           * \param[in] max_distance   Maximum units in the last place (ULPs) to be tolerated
           * \return    Boolean value corresponding to true if the internally saved number corresponds to \p rhs under the given \p max_distance, false else
          */
          constexpr bool isAlmostEqual(FloatView const rhs, std::uint8_t const max_distance = 4) const noexcept {
            if (Trait::isNan(value) || Trait::isNan(rhs.value)) {
              return false;
            // Forces infinity to only be equal to infinity and not also to very large numbers
            } else if (Trait::isNegInf(value) != Trait::isNegInf(rhs.value)) {
              return false;
            } else if (Trait::isPosInf(value) != Trait::isPosInf(rhs.value)) {
              return false;
            }
            return computeDistance(value, rhs.value) <= max_distance;
          }

        protected:
          T value;

          /**\fn        signMagnitudeToBiased
           * \brief     Conversion function for converting a given floating point number given by a sign magnitude representation to a biased representation
           * 
           * \param[in] t   The floating point number to be converted
           * \return    A bit sequence in biased representation with the same number of bytes as the input floating point type
          */
          static constexpr Bytes signMagnitudeToBiased(T const t) noexcept {
            auto const b {std::bit_cast<Bytes>(t)};
            if (Trait::isNeg(t)) {
              return ~b + Bytes{1};
            } else {
              return Trait::sign_mask | b;
            }
          }

          /**\fn        computeDistance
           * \brief     Function for calculating the distance in biased representation between two floating point numbers
           * 
           * \param[in] a   The floating point number
           * \param[in] b   Another floating point number
           * \return    The distance of the two floating point numbers as an unsigned integer
          */
          static constexpr Bytes computeDistance(T const a, T const b) noexcept {
            auto const biased_a = signMagnitudeToBiased(a);
            auto const biased_b = signMagnitudeToBiased(b);
            return (biased_a >= biased_b) ? (biased_a - biased_b) : (biased_b - biased_a);
          }
      };

    }

    /**\fn        isAlmostEqualUlps
     * \brief     Function for comparing two floating point numbers according to the units in the last place (ULPs)
     * 
     * \param[in] lhs            A floating point number to be compared for equality
     * \param[in] rhs            Another floating point number to be compared for equality to \p lhs
     * \param[in] max_distance   Maximum units in the last place (ULPs) to be tolerated
     * \return    Boolean value corresponding to true if the two numbers \p lhs and \p rhs are equal under the given \p max_distance, false else
    */
    template <typename T>
    constexpr bool isAlmostEqualUlps(T const lhs, T const rhs, std::uint8_t const max_distance = 4) noexcept {
      detail::FloatView<T> const a {lhs};
      detail::FloatView<T> const b {rhs};
      return a.isAlmostEqual(b, max_distance);
    }

  }
}

#endif // LBT_CEM_IS_ALMOST_EQUAL_ULPS
