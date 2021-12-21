#ifndef LBT_OUTPUT_UTILITIES
#define LBT_OUTPUT_UTILITIES

/**
 * \file     output_utilities.hpp
 * \mainpage Tools for console and file output
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>


namespace lbt {
  /**\fn     is_streamable
   * \brief  Type trait for checking if a type \p T can be streamed into \p S via the stream operator <<
   *
   * \tparam S   The stream where \p T should be streamed into
   * \tparam T   The value type to be streamed into \p S
   * \tparam Dummy parameter used for SFINAE
  */
  template<typename S, typename T, typename = void>
  struct is_streamable : std::false_type {
  };
  // Specialised template for streamable types
  template<typename S, typename T>
  struct is_streamable<S, T, decltype(std::declval<S&>() << std::declval<T&>(), void())> : std::true_type {
  };
  // Convient alias for is_streamable<S,T>::value
  template <typename S, typename T>
  static constexpr bool is_streamable_v = is_streamable<S,T>::value;

  /**\fn        toString
   * \brief     Convert a value \p t to a std::string considering the precision \p digits
   *
   * \tparam    T        The type of the value to be converted to string
   * \tparam    Dummy parameter used for SFINAE
   * \param[in] t        The value to be converted to string
   * \param[in] digits   The number of digits that should be considered when converting to string
   * \return    The input value converted to a string
  */
  template <typename T, typename std::enable_if_t<is_streamable_v<std::ostringstream,T>>* = nullptr>
  std::string toString(T const t, std::int32_t const digits = 3) noexcept {
    std::ostringstream ss {};
    ss.precision(digits);
    ss << std::fixed << t;
    return ss.str();
  }
}

#endif // LBT_OUTPUT_UTILITIES
