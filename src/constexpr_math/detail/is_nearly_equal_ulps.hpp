/**
 * \file     is_nearly_equal_ulps.hpp
 * \brief    Constexpr implementation of a floating number point comparison based on units in last place (ULPs)
 * \mainpage Contains traits and a constexpr function for comparing two floating point numbers based on units 
 *           in the last place (ULP) similar to the implementation in GoogleTest but more modular and constexpr:
 *           https://github.com/google/googletest/blob/main/googletest/include/gtest/internal/gtest-internal.h
 * \warning  Requires C++20 or later for compilation
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_IS_NEARLY_EQUAL_ULPS
#define LBT_CEM_IS_NEARLY_EQUAL_ULPS
#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>


namespace lbt {
  namespace cem {

    namespace detail {
      // Trait for excluding incomplete types: See https://stackoverflow.com/a/44229779/9938686
      template <typename T, std::size_t = sizeof(T)>
      std::true_type is_complete(T *);

      std::false_type is_complete(...);
    }

    template <typename T>
    using is_complete = decltype(detail::is_complete(std::declval<T*>()));

    template <typename T>
    static constexpr bool is_complete_v = is_complete<T>::value;

    namespace detail {

      // Class for determining the corresponding unsigned integer type with equal length
      template <std::size_t N>
      class UIntEquiv {
        protected:
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
      
      // Potentially add specialisation of UIntEquiv for longer unsigned integer types here
      // e.g. GCC's __uint128_t: https://gcc.gnu.org/onlinedocs/gcc/_005f_005fint128.html
      // or Boost: https://www.boost.org/doc/libs/1_67_0/libs/multiprecision/doc/html/boost_multiprecision/tut/ints/cpp_int.html

      template <std::size_t N>
      using UIntEquiv_t = typename UIntEquiv<N>::type;

      // You can provide specialisations of this trait for proprietary floating types
      // e.g. Boost float128: https://www.boost.org/doc/libs/1_65_1/libs/multiprecision/doc/html/boost_multiprecision/tut/floats/float128.html)
      template <typename T>
      class FloatTrait;

      // Specialised trait for floating point number types according to IEEE754
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

          static constexpr bool isNan(T const t) noexcept {
            auto const bytes {std::bit_cast<Bytes>(t)};
            auto const exponent_bytes {extractExponent(bytes)};
            auto const fraction_bytes {extractFraction(bytes)};
            return (exponent_bytes == exponent_mask) && (fraction_bytes != 0);
          }

          static constexpr bool isPosInf(T const t) noexcept {
            return isPos(t) && isInf(t);
          }

          static constexpr bool isNegInf(T const t) noexcept {
            return isNeg(t) && isInf(t);
          }

          static constexpr bool isNeg(T const t) noexcept {
            auto const bytes {std::bit_cast<Bytes>(t)};
            auto const sign_bytes {extractSign(bytes)};
            return sign_bytes != 0;
          }

          // Optional helper functions
          static constexpr bool isPos(T const t) noexcept {
            auto const bytes {std::bit_cast<Bytes>(t)};
            auto const sign_bytes {extractSign(bytes)};
            return sign_bytes == 0;
          }

          static constexpr bool isInf(T const t) noexcept {
            auto const bytes {std::bit_cast<Bytes>(t)};
            auto const exponent_bytes {extractExponent(bytes)};
            auto const fraction_bytes {extractFraction(bytes)};
            return (exponent_bytes == exponent_mask) && (fraction_bytes == 0);
          }

          static constexpr Bytes extractSign(Bytes const bytes) noexcept {
            return bytes & sign_mask;
          }

          static constexpr Bytes extractExponent(Bytes const bytes) noexcept {
            return bytes & exponent_mask;
          }

          static constexpr Bytes extractFraction(Bytes const bytes) noexcept {
            return bytes & fraction_mask;
          }

        protected:
          FloatTrait() = delete;
          FloatTrait(FloatTrait const&) = delete;
          FloatTrait(FloatTrait&&) = delete;
          FloatTrait& operator= (FloatTrait const&) = delete;
          FloatTrait& operator= (FloatTrait&&) = delete;
      };

      template <typename T>
      requires is_complete_v<FloatTrait<T>> && is_complete_v<UIntEquiv_t<sizeof(T)>>
      class FloatView {
        public:
          using Trait = FloatTrait<T>;
          using Bytes = UIntEquiv_t<sizeof(T)>;

          explicit constexpr FloatView(T const t) noexcept
            : value{t} {
            return;
          }
          FloatView() = default;
          FloatView(FloatView const&) = default;
          FloatView(FloatView&&) = default;
          FloatView& operator= (FloatView const&) = default;
          FloatView& operator= (FloatView&&) = default;

          constexpr bool isNearlyEqual(FloatView const rhs, std::uint8_t const max_distance = 4) const noexcept {
            if (Trait::isNan(value) || Trait::isNan(rhs.value)) {
              return false;
            // Forces infinity to only be equal to inifinity and not also to very large numbers
            } else if (Trait::isNegInf(value) != Trait::isNegInf(rhs.value)) {
              return false;
            } else if (Trait::isPosInf(value) != Trait::isPosInf(rhs.value)) {
              return false;
            }
            return computeDistance(value, rhs.value) <= max_distance;
          }

        protected:
          T value;

          static constexpr Bytes signMagnitudeToBiased(T const t) noexcept {
            auto const b {std::bit_cast<Bytes>(t)};
            if (Trait::isNeg(t)) {
              return ~b + Bytes{1};
            } else {
              return Trait::sign_mask | b;
            }
          }

          static constexpr Bytes computeDistance(T const a, T const b) noexcept {
            auto const biased1 = signMagnitudeToBiased(a);
            auto const biased2 = signMagnitudeToBiased(b);
            return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
          }
      };

    }

    template <typename T>
    constexpr bool isNearlyEqualUlps(T const lhs, T const rhs, std::uint8_t const max_distance = 4) noexcept {
      detail::FloatView<T> const a {lhs};
      detail::FloatView<T> const b {rhs};
      return a.isNearlyEqual(b, max_distance);
    }

  }
}

#endif // LBT_CEM_IS_NEARLY_EQUAL_ULPS
