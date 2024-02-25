/**
 * \file     time.hpp
 * \mainpage Contains unit definition for a time
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_TIME
#define LBT_UNIT_TIME
#pragma once

#include "lbt/unit/detail/unit_base.hpp"


namespace lbt {
  namespace unit {

    /**\class Time
     * \brief Unit class for time and duration
    */
    class Time : public lbt::unit::detail::UnitBase<Time> {
      public:
        /**\fn    Time
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit seconds
        */
        explicit constexpr Time(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        Time(Time const&) = default;
        Time& operator= (Time const&) = default;
        Time(Time&&) = default;
        Time& operator= (Time&&) = default;
    };
    using Duration = Time;

  }
}

#endif // LBT_UNIT_TIME
