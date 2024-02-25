/**
 * \file     dynamic_viscosity_literals.hpp
 * \mainpage Contains definitions for user-defined literals for dynamic viscosity
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_DYNAMIC_VISCOSITY_LITERALS
#define LBT_UNIT_DYNAMIC_VISCOSITY_LITERALS
#pragma once

#include "lbt/unit/detail/dynamic_viscosity.hpp"
#include "lbt/unit/detail/operators.hpp"
#include "lbt/unit/detail/pressure.hpp"
#include "lbt/unit/detail/pressure_literals.hpp"
#include "lbt/unit/detail/time.hpp"
#include "lbt/unit/detail/time_literals.hpp"


namespace lbt {
  namespace literals {

    /**\fn        operator "" _Pas
     * \brief     User-defined literal for a dynamic viscosity given in Pascal seconds
     * 
     * \param[in] p   The dynamic viscosity in Pascal seconds
     * \return    A dynamic viscosity in the base unit Pascal seconds
    */
    constexpr lbt::unit::DynamicViscosity operator "" _Pas(long double const c) noexcept {
      return c*lbt::unit::DynamicViscosity{1.0};
    }
    /**\fn        operator "" _mPas
     * \brief     User-defined literal for a dynamic viscosity given in milli Pascal seconds
     * 
     * \param[in] p   The dynamic viscosity in milli Pascal seconds
     * \return    A dynamic viscosity in the base unit Pascal seconds
    */
    constexpr lbt::unit::DynamicViscosity operator "" _mPas(long double const c) noexcept {
      return c*lbt::unit::DynamicViscosity{1.0_mPa*1.0_s};
    }
    /**\fn        operator "" _uPas
     * \brief     User-defined literal for a dynamic viscosity given in micro Pascal seconds
     * 
     * \param[in] p   The dynamic viscosity in micro Pascal seconds
     * \return    A dynamic viscosity in the base unit Pascal seconds
    */
    constexpr lbt::unit::DynamicViscosity operator "" _uPas(long double const c) noexcept {
      return c*lbt::unit::DynamicViscosity{1.0_uPa*1.0_s};
    }
    /**\fn        operator "" _P
     * \brief     User-defined literal for a dynamic viscosity given in Poise
     * 
     * \param[in] p   The dynamic viscosity in Poise
     * \return    A dynamic viscosity in the base unit Pascal seconds
    */
    constexpr lbt::unit::DynamicViscosity operator "" _P(long double const c) noexcept {
      return c*lbt::unit::DynamicViscosity{1.0e-1};
    }
    /**\fn        operator "" _cP
     * \brief     User-defined literal for a dynamic viscosity given in Centipoise
     * 
     * \param[in] p   The dynamic viscosity in Centipoise
     * \return    A dynamic viscosity in the base unit Pascal seconds
    */
    constexpr lbt::unit::DynamicViscosity operator "" _cP(long double const c) noexcept {
      return c*lbt::unit::DynamicViscosity{1.0_mPa*1.0_s};
    }

  }
}

#endif // LBT_UNIT_DYNAMIC_VISCOSITY_LITERALS
