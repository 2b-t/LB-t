/**
 * \file     velocity.hpp
 * \mainpage Contains unit definition for a velocity
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_VELOCITY
#define LBT_UNIT_VELOCITY
#pragma once

#include "lbt/unit/detail/unit_base.hpp"


namespace lbt {
  namespace unit {

    /**\class Velocity
     * \brief Unit class for velocity
    */
    class Velocity : public lbt::unit::detail::UnitBase<Velocity> {
      public:
        /**\fn    Velocity
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit meters per seconds
        */
        explicit constexpr Velocity(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        Velocity(Velocity const&) = default;
        Velocity& operator= (Velocity const&) = default;
        Velocity(Velocity&&) = default;
        Velocity& operator= (Velocity&&) = default;
    };

  }
}

#endif // LBT_UNIT_VELOCITY
