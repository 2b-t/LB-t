/**
 * \file     mass.hpp
 * \mainpage Contains unit definition for a mass
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_MASS
#define LBT_UNIT_MASS
#pragma once

#include "unit_base.hpp"


namespace lbt {
  namespace unit {

    /**\class Mass
     * \brief Unit class for mass
    */
    class Mass : public lbt::unit::detail::UnitBase<Mass> {
      public:
        /**\fn    Mass
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit kilograms
        */
        constexpr Mass(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        Mass(Mass const&) = default;
        Mass& operator= (Mass const&) = default;
        Mass(Mass&&) = default;
        Mass& operator= (Mass&&) = default;
    };

  }
}

#endif // LBT_UNIT_MASS
