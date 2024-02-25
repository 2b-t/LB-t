/**
 * \file     area.hpp
 * \mainpage Contains unit definition for an area
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_AREA
#define LBT_UNIT_AREA
#pragma once

#include "lbt/unit/detail/unit_base.hpp"


namespace lbt {
  namespace unit {

    /**\class Area
     * \brief Unit class for areas
    */
    class Area : public lbt::unit::detail::UnitBase<Area> {
      public:
        /**\fn    Area
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit square meters
        */
        constexpr Area(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        Area(Area const&) = default;
        Area& operator= (Area const&) = default;
        Area(Area&&) = default;
        Area& operator= (Area&&) = default;
    };

  }
}

#endif // LBT_UNIT_AREA
