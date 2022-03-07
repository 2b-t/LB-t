/**
 * \file     constexpr_math_utilities.hpp
 * \mainpage Helper functions for generating random numbers for testing of constexpr functions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_CEM_UTILITIES
#define LBT_CEM_UTILITIES
#pragma once

#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>


namespace lbt {
  namespace test {

    /**\fn        generateRandomNumber
     * \brief     Generate a random number of floating type \p T between \p lower_bound and \p upper_bound
     *
     * \tparam    T             The corresponding floating point data type
     * \param[in] lower_bound   The smallest possible random floating point number
     * \param[in] upper_bound   The largest possible random floating point number
    * \return    A random floating point number between \p lower_bound and \p upper_bound
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    auto generateRandomNumber(T const lower_bound, T const upper_bound) noexcept {
      std::uniform_real_distribution<T> uniform_distribution {lower_bound, upper_bound};
      std::random_device random_device {};
      std::default_random_engine random_engine {random_device()};
      T const random_number {uniform_distribution(random_engine)};
      return random_number;
    }

    /**\fn        generateRandomNumber
     * \brief     Generate a random number of integer type \p T between \p lower_bound and \p upper_bound
     *
     * \tparam    T             The corresponding integral data type
     * \param[in] lower_bound   The smallest possible random integer number
     * \param[in] upper_bound   The largest possible random integer number
     * \return    A integer random number between \p lower_bound and \p upper_bound
    */
    template <typename T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
    auto generateRandomNumber(T const lower_bound, T const upper_bound) {
      if (lower_bound > upper_bound) {
        throw std::invalid_argument("Range invalid: lower_bound > upper_bound (" + std::to_string(lower_bound) + " > " + std::to_string(upper_bound) + ")!");
      }
      std::uniform_int_distribution<T> uniform_distribution {lower_bound, upper_bound};
      std::random_device random_device {};
      std::default_random_engine random_engine {random_device()};
      T const random_number {uniform_distribution(random_engine)};
      return random_number;
    }

  }
}

#endif // LBT_CEM_UTILITIES
