/**
 * \file     pressure.hpp
 * \mainpage Contains unit definition for a pressure
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_PRESSURE
#define LBT_UNIT_PRESSURE
#pragma once

#include "lbt/unit/detail/unit_base.hpp"


namespace lbt {
  namespace unit {

    /**\class Pressure
     * \brief Unit class for pressure
    */
    class Pressure : public lbt::unit::detail::UnitBase<Pressure> {
      public:
        /**\fn    Pressure
         * \brief Constructor, defaults to 101325 Pa
         * 
         * \param[in] value   The value to be stored inside the class in Pascal
        */
        explicit constexpr Pressure(long double const value = 101325) noexcept
          : UnitBase{value} {
          return;
        }
        Pressure(Pressure const&) = default;
        Pressure& operator= (Pressure const&) = default;
        Pressure(Pressure&&) = default;
        Pressure& operator= (Pressure&&) = default;
    };

  }
}

#endif // LBT_UNIT_PRESSURE
