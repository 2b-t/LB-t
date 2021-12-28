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
      return lbt::unit::Length{1000.0*k};
    }
    /**\fn        operator "" _m
     * \brief     User-defined literal for a length given in meters
     * 
     * \param[in] m   The distance in meters
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _m(long double const m) noexcept {
      return lbt::unit::Length{m};
    }
    /**\fn        operator "" _dm
     * \brief     User-defined literal for a length given in decimeters
     * 
     * \param[in] d   The distance in decimeters
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _dm(long double const d) noexcept {
      return lbt::unit::Length{d/10.0};
    }
    /**\fn        operator "" _cm
     * \brief     User-defined literal for a length given in centimeters
     * 
     * \param[in] c   The distance in centimeters
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _cm(long double const c) noexcept {
      return lbt::unit::Length{c/100.0};
    }
    /**\fn        operator "" _mm
     * \brief     User-defined literal for a length given in millimeters
     * 
     * \param[in] m   The distance in millimetres
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _mm(long double const m) noexcept {
      return lbt::unit::Length{m/1000.0};
    }
    /**\fn        operator "" _um
     * \brief     User-defined literal for a length given in micrometers
     * 
     * \param[in] u   The distance in micrometers
     * \return    A length in the base unit meters
    */
    constexpr lbt::unit::Length operator "" _um(long double const u) noexcept {
      return lbt::unit::Length{u/1.0e6};
    }

    /**\fn        operator "" _h
     * \brief     User-defined literal for a time given in hours
     * 
     * \param[in] h   The time in hours
     * \return    A time in the base unit seconds
    */
    constexpr lbt::unit::Time operator "" _h(long double const h) noexcept {
      return lbt::unit::Time{3600.0*h};
    }
    /**\fn        operator "" _min
     * \brief     User-defined literal for a time given in minutes
     * 
     * \param[in] m   The time in minutes
     * \return    A time in the base unit seconds
    */
    constexpr lbt::unit::Time operator "" _min(long double const m) noexcept {
      return lbt::unit::Time{60.0*m};
    }
    /**\fn        operator "" _s
     * \brief     User-defined literal for a time given in seconds
     * 
     * \param[in] s   The time in seconds
     * \return    A time in the base unit seconds
    */
    constexpr lbt::unit::Time operator "" _s(long double const s) noexcept {
      return lbt::unit::Time{s};
    }
    /**\fn        operator "" _ms
     * \brief     User-defined literal for a time given in milliseconds
     * 
     * \param[in] m   The time in milliseconds
     * \return    A time in the base unit seconds
    */
    constexpr lbt::unit::Time operator "" _ms(long double const m) noexcept {
      return lbt::unit::Time{m/1000.0};
    }
    /**\fn        operator "" _us
     * \brief     User-defined literal for a time given in microseconds
     * 
     * \param[in] u   The time in microseconds
     * \return    A time in the base unit seconds
    */
    constexpr lbt::unit::Time operator "" _us(long double const u) noexcept {
      return lbt::unit::Time{u/1.0e6};
    }

    /**\fn        operator "" _t
     * \brief     User-defined literal for a mass given in tons
     * 
     * \param[in] h   The mass in tons
     * \return    A mass in the base unit kilograms
    */
    constexpr lbt::unit::Mass operator "" _t(long double const t) noexcept {
      return lbt::unit::Mass{1000.0*t};
    }
    /**\fn        operator "" _kg
     * \brief     User-defined literal for a mass given in kilograms
     * 
     * \param[in] h   The mass in kilograms
     * \return    A mass in the base unit kilograms
    */
    constexpr lbt::unit::Mass operator "" _kg(long double const k) noexcept {
      return lbt::unit::Mass{k};
    }
    /**\fn        operator "" _g
     * \brief     User-defined literal for a mass given in grams
     * 
     * \param[in] h   The mass in grams
     * \return    A mass in the base unit kilograms
    */
    constexpr lbt::unit::Mass operator "" _g(long double const g) noexcept {
      return lbt::unit::Mass{g/1000.0};
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
      return lbt::unit::Area{m};
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
      return lbt::unit::Volume{m};
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
      return lbt::unit::Velocity{m};
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

    /**\fn        operator "" _P
     * \brief     User-defined literal for a kinematic viscosity given in Poise
     * 
     * \param[in] p   The kinematic viscosity in Poise
     * \return    A kinematic viscosity in the base unit meters squared per second
    */
    constexpr lbt::unit::KinematicViscosity operator "" _P(long double const p) noexcept {
      return lbt::unit::KinematicViscosity{0.1*p};
    }
    /**\fn        operator "" _cP
     * \brief     User-defined literal for a kinematic viscosity given in Centipose
     * 
     * \param[in] c   The kinematic viscosity in Centipoise
     * \return    A kinematic viscosity in the base unit meters squared per second
    */
    constexpr lbt::unit::KinematicViscosity operator "" _cP(long double const c) noexcept {
      return lbt::unit::KinematicViscosity{c/1.0e3};
    }
    /**\fn        operator "" _St
     * \brief     User-defined literal for a kinematic viscosity given in Stokes
     * 
     * \param[in] p   The kinematic viscosity in Stokes
     * \return    A kinematic viscosity in the base unit meters squared per second
    */
    constexpr lbt::unit::KinematicViscosity operator "" _St(long double const s) noexcept {
      return lbt::unit::KinematicViscosity{s/1.0e4};
    }
    /**\fn        operator "" _cSt
     * \brief     User-defined literal for a kinematic viscosity given in Centistokes
     * 
     * \param[in] p   The kinematic viscosity in Centistokes
     * \return    A kinematic viscosity in the base unit meters squared per second
    */
    constexpr lbt::unit::KinematicViscosity operator "" _cSt(long double const c) noexcept {
      return lbt::unit::KinematicViscosity{c/1.0e6};
    }

  }
}

#endif // LBT_LITERALS
