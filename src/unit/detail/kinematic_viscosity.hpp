/**
 * \file     kinematic_viscosity.hpp
 * \mainpage Contains unit definition for a kinematic_viscosity
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_KINEMATIC_VISCOSITY
#define LBT_UNIT_KINEMATIC_VISCOSITY
#pragma once

#include "unit_base.hpp"


namespace lbt {
  namespace unit {

    /**\class KinematicViscosity
     * \brief Unit class for fluid kinematic viscosity
    */
    class KinematicViscosity : public lbt::unit::detail::UnitBase<KinematicViscosity> {
      public:
        /**\fn    KinematicViscosity
         * \brief Constructor
         * 
         * \param[in] value   The value to be stored inside the class in the base unit meter squared per second
        */
        constexpr KinematicViscosity(long double const value = 0.0) noexcept
          : UnitBase{value} {
          return;
        }
        KinematicViscosity(KinematicViscosity const&) = default;
        KinematicViscosity& operator= (KinematicViscosity const&) = default;
        KinematicViscosity(KinematicViscosity&&) = default;
        KinematicViscosity& operator= (KinematicViscosity&&) = default;
    };

  }
}

#endif // LBT_UNIT_KINEMATIC_VISCOSITY
