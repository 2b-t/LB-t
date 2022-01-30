#ifndef LBT_LITERALS
#define LBT_LITERALS
#pragma once

/**\file     literals.hpp
 * \mainpage Contains definitions for user-defined literals of different physical units
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <iostream>

#include "units.hpp"


namespace lbt {
  namespace literals {

    /**\fn        operator "" _km
     * \brief     User-defined literal for a length given in kilometers
     * 
     * \param[in] k   The distance in kilometers
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _km(long double const k) noexcept {
      return k*lbt::unit::Length{1.0e3};
    }
    /**\fn        operator "" _m
     * \brief     User-defined literal for a length given in meters
     * 
     * \param[in] m   The distance in meters
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _m(long double const m) noexcept {
      return m*lbt::unit::Length{1.0};
    }
    /**\fn        operator "" _dm
     * \brief     User-defined literal for a length given in decimeters
     * 
     * \param[in] d   The distance in decimeters
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _dm(long double const d) noexcept {
      return d*lbt::unit::Length{1.0e-1};
    }
    /**\fn        operator "" _cm
     * \brief     User-defined literal for a length given in centimeters
     * 
     * \param[in] c   The distance in centimeters
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _cm(long double const c) noexcept {
      return c*lbt::unit::Length{1.0e-2};
    }
    /**\fn        operator "" _mm
     * \brief     User-defined literal for a length given in millimeters
     * 
     * \param[in] m   The distance in millimetres
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _mm(long double const m) noexcept {
      return m*lbt::unit::Length{1.0e-3};
    }
    /**\fn        operator "" _um
     * \brief     User-defined literal for a length given in micrometers
     * 
     * \param[in] u   The distance in micrometers
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _um(long double const u) noexcept {
      return u*lbt::unit::Length{1.0e-6};
    }
    /**\fn        operator "" _pm
     * \brief     User-defined literal for a length given in picometers
     * 
     * \param[in] u   The distance in picometers
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _pm(long double const u) noexcept {
      return u*lbt::unit::Length{1.0e-12};
    }

    /**\fn        operator "" _d
     * \brief     User-defined literal for a time given in days
     * 
     * \param[in] d   The time in days
     * \return    A time in the base unit seconds
    */
    constexpr lbt::unit::Time operator "" _d(long double const d) noexcept {
      return d*lbt::unit::Time{86400.0};
    }
    /**\fn        operator "" _h
     * \brief     User-defined literal for a time given in hours
     * 
     * \param[in] h   The time in hours
     * \return    A time in the base unit seconds
    */
    constexpr lbt::unit::Time operator "" _h(long double const h) noexcept {
      return h*lbt::unit::Time{3600.0};
    }
    /**\fn        operator "" _min
     * \brief     User-defined literal for a time given in minutes
     * 
     * \param[in] m   The time in minutes
     * \return    A time in the base unit seconds
    */
    constexpr lbt::unit::Time operator "" _min(long double const m) noexcept {
      return m*lbt::unit::Time{60.0};
    }
    /**\fn        operator "" _s
     * \brief     User-defined literal for a time given in seconds
     * 
     * \param[in] s   The time in seconds
     * \return    A time in the base unit seconds
    */
    constexpr lbt::unit::Time operator "" _s(long double const s) noexcept {
      return s*lbt::unit::Time{1.0};
    }
    /**\fn        operator "" _ms
     * \brief     User-defined literal for a time given in milliseconds
     * 
     * \param[in] m   The time in milliseconds
     * \return    A time in the base unit seconds
    */
    constexpr lbt::unit::Time operator "" _ms(long double const m) noexcept {
      return m*lbt::unit::Time{1.0e-3};
    }
    /**\fn        operator "" _us
     * \brief     User-defined literal for a time given in microseconds
     * 
     * \param[in] u   The time in microseconds
     * \return    A time in the base unit seconds
    */
    constexpr lbt::unit::Time operator "" _us(long double const u) noexcept {
      return u*lbt::unit::Time{1.0e-6};
    }

    /**\fn        operator "" _t
     * \brief     User-defined literal for a mass given in tons
     * 
     * \param[in] h   The mass in tons
     * \return    A mass in the base unit kilograms
    */
    constexpr lbt::unit::Mass operator "" _t(long double const t) noexcept {
      return t*lbt::unit::Mass{1000.0};
    }
    /**\fn        operator "" _kg
     * \brief     User-defined literal for a mass given in kilograms
     * 
     * \param[in] h   The mass in kilograms
     * \return    A mass in the base unit kilograms
    */
    constexpr lbt::unit::Mass operator "" _kg(long double const k) noexcept {
      return k*lbt::unit::Mass{1.0};
    }
    /**\fn        operator "" _g
     * \brief     User-defined literal for a mass given in grams
     * 
     * \param[in] h   The mass in grams
     * \return    A mass in the base unit kilograms
    */
    constexpr lbt::unit::Mass operator "" _g(long double const g) noexcept {
      return g*lbt::unit::Mass{1.0e-3};
    }

    /**\fn        operator "" _km2
     * \brief     User-defined literal for an area given in square kilometers
     * 
     * \param[in] k   The area in square kilometers
     * \return    An area in the base unit square meters
    */
    constexpr lbt::unit::Area operator "" _km2(long double const k) noexcept {
      return k*lbt::unit::Area{1.0_km*1.0_km};
    }
    /**\fn        operator "" _m2
     * \brief     User-defined literal for an area given in square meters
     * 
     * \param[in] m   The area in square meters
     * \return    An area in the base unit square meters
    */
    constexpr lbt::unit::Area operator "" _m2(long double const m) noexcept {
      return m*lbt::unit::Area{1.0_m*1.0_m};
    }
    /**\fn        operator "" _cm2
     * \brief     User-defined literal for an area given in square centimeters
     * 
     * \param[in] c   The area in square centimeters
     * \return    An area in the base unit square meters
    */
    constexpr lbt::unit::Area operator "" _cm2(long double const c) noexcept {
      return c*lbt::unit::Area{1.0_cm*1.0_cm};
    }
    /**\fn        operator "" _mm2
     * \brief     User-defined literal for an area given in square millimeters
     * 
     * \param[in] m   The area in square millimeters
     * \return    An area in the base unit square meters
    */
    constexpr lbt::unit::Area operator "" _mm2(long double const m) noexcept {
      return m*lbt::unit::Area{1.0_mm*1.0_mm};
    }

    /**\fn        operator "" _km3
     * \brief     User-defined literal for a volume given in cubic kilometers
     * 
     * \param[in] k   The volume in cubic kilometers
     * \return    A volume in the base unit cubic meters
    */
    constexpr lbt::unit::Volume operator "" _km3(long double const k) noexcept {
      return k*lbt::unit::Volume{1.0_km*1.0_km*1.0_km};
    }
    /**\fn        operator "" _m3
     * \brief     User-defined literal for a volume given in cubic meters
     * 
     * \param[in] m   The volume in cubic meters
     * \return    A volume in the base unit cubic meters
    */
    constexpr lbt::unit::Volume operator "" _m3(long double const m) noexcept {
      return m*lbt::unit::Volume{1.0_m*1.0_m*1.0_m};
    }
    /**\fn        operator "" _cm3
     * \brief     User-defined literal for a volume given in cubic centimeters
     * 
     * \param[in] c   The volume in cubic centimeters
     * \return    A volume in the base unit cubic meters
    */
    constexpr lbt::unit::Volume operator "" _cm3(long double const c) noexcept {
      return c*lbt::unit::Volume{1.0_cm*1.0_cm*1.0_cm};
    }
    /**\fn        operator "" _mm3
     * \brief     User-defined literal for a volume given in cubic millimeters
     * 
     * \param[in] m   The volume in cubic millimeters
     * \return    A volume in the base unit cubic meters
    */
    constexpr lbt::unit::Volume operator "" _mm3(long double const m) noexcept {
      return m*lbt::unit::Volume{1.0_mm*1.0_mm*1.0_mm};
    }

    /**\fn        operator "" _kmph
     * \brief     User-defined literal for a velocity given in kilometers per hour
     * 
     * \param[in] k   The velocity in kilometers per hour
     * \return    A velocity in the base unit meters per second
    */
    constexpr lbt::unit::Velocity operator "" _kmph(long double const k) noexcept {
      return k*lbt::unit::Velocity{1.0_km/1.0_h};
    }
    /**\fn        operator "" _mps
     * \brief     User-defined literal for a velocity given in meters per second
     * 
     * \param[in] m   The velocity in meters per second
     * \return    A velocity in the base unit meters per second
    */
    constexpr lbt::unit::Velocity operator "" _mps(long double const m) noexcept {
      return m*lbt::unit::Velocity{1.0_m/1.0_s};
    }
    /**\fn        operator "" _cmps
     * \brief     User-defined literal for a velocity given in centimeters per second
     * 
     * \param[in] c   The velocity in centimeters per second
     * \return    A velocity in the base unit meters per second
    */
    constexpr lbt::unit::Velocity operator "" _cmps(long double const c) noexcept {
      return c*lbt::unit::Velocity{1.0_cm/1.0_s};
    }
    /**\fn        operator "" _mmps
     * \brief     User-defined literal for a velocity given in millimeters per second
     * 
     * \param[in] m   The velocity in millimeters per second
     * \return    A velocity in the base unit meters per second
    */
    constexpr lbt::unit::Velocity operator "" _mmps(long double const m) noexcept {
      return m*lbt::unit::Velocity{1.0_mm/1.0_s};
    }

    /**\fn        operator "" _K
     * \brief     User-defined literal for a temperature given in Kelvin
     * 
     * \param[in] t   The temperature in Kelvin
     * \return    A temperature in the base unit Kelvin
    */
    constexpr lbt::unit::Temperature operator "" _K(long double const t) noexcept {
      return lbt::unit::Temperature{t};
    }
    /**\fn        operator "" _deg
     * \brief     User-defined literal for a temperature given in degrees
     * 
     * \param[in] t   The temperature in degrees
     * \return    A temperature in the base unit Kelvin
    */
    constexpr lbt::unit::Temperature operator "" _deg(long double const t) noexcept {
      return lbt::unit::Temperature{t + 273.15};
    }

    /**\fn        operator "" _Pa
     * \brief     User-defined literal for a pressure given in Pascal
     * 
     * \param[in] p   The pressure in Pascal
     * \return    A pressure in the base unit Pascal
    */
    constexpr lbt::unit::Pressure operator "" _Pa(long double const p) noexcept {
      return p*lbt::unit::Pressure{1.0};
    }
    /**\fn        operator "" _mPa
     * \brief     User-defined literal for a pressure given in milli-Pascal
     * 
     * \param[in] p   The pressure in milli-Pascal
     * \return    A pressure in the base unit Pascal
    */
    constexpr lbt::unit::Pressure operator "" _mPa(long double const p) noexcept {
      return p*lbt::unit::Pressure{1.0e-3};
    }
    /**\fn        operator "" _hPa
     * \brief     User-defined literal for a pressure given in hecto-Pascal
     * 
     * \param[in] p   The pressure in hecto-Pascal
     * \return    A pressure in the base unit Pascal
    */
    constexpr lbt::unit::Pressure operator "" _hPa(long double const p) noexcept {
      return p*lbt::unit::Pressure{100.0};
    }
    /**\fn        operator "" _bar
     * \brief     User-defined literal for a pressure given in bar
     * 
     * \param[in] p   The pressure in bar
     * \return    A pressure in the base unit Pascal
    */
    constexpr lbt::unit::Pressure operator "" _bar(long double const p) noexcept {
      return p*lbt::unit::Pressure{1.0e5};
    }
    /**\fn        operator "" _atm
     * \brief     User-defined literal for a pressure given in atmospheres
     * 
     * \param[in] p   The pressure in atmospheres
     * \return    A pressure in the base unit Pascal
    */
    constexpr lbt::unit::Pressure operator "" _atm(long double const p) noexcept {
      return p*lbt::unit::Pressure{101325};
    }

    /**\fn        operator "" _m2ps
     * \brief     User-defined literal for a kinematic viscosity given in square meters per second
     * 
     * \param[in] p   The kinematic viscosity in square meters per second
     * \return    A kinematic viscosity in the base unit meters squared per second
    */
    constexpr lbt::unit::KinematicViscosity operator "" _m2ps(long double const m) noexcept {
      return m*lbt::unit::KinematicViscosity{1.0_m*1.0_m/1.0_s};
    }
    /**\fn        operator "" _St
     * \brief     User-defined literal for a kinematic viscosity given in Stokes
     * 
     * \param[in] p   The kinematic viscosity in Stokes
     * \return    A kinematic viscosity in the base unit meters squared per second
    */
    constexpr lbt::unit::KinematicViscosity operator "" _St(long double const s) noexcept {
      return s*lbt::unit::KinematicViscosity{1.0e-4};
    }
    /**\fn        operator "" _cSt
     * \brief     User-defined literal for a kinematic viscosity given in Centistokes
     * 
     * \param[in] p   The kinematic viscosity in Centistokes
     * \return    A kinematic viscosity in the base unit meters squared per second
    */
    constexpr lbt::unit::KinematicViscosity operator "" _cSt(long double const c) noexcept {
      return c*lbt::unit::KinematicViscosity{1.0e-6};
    }

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

#endif // LBT_LITERALS
