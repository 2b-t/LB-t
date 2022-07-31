/**
 * \file     operators.hpp
 * \mainpage Contains different operators for different mixed physical units
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_OPERATORS
#define LBT_UNIT_OPERATORS
#pragma once

#include "amount_of_substance.hpp"
#include "area.hpp"
#include "density.hpp"
#include "dynamic_viscosity.hpp"
#include "kinematic_viscosity.hpp"
#include "length.hpp"
#include "mass.hpp"
#include "molar_mass.hpp"
#include "pressure.hpp"
#include "time.hpp"
#include "unit_base.hpp"
#include "velocity.hpp"
#include "volume.hpp"


namespace lbt {
  namespace unit {

    /**\fn        operator*
     * \brief     Multiplication operator between two lengths resulting in an area
     * 
     * \param[in] a   A length
     * \param[in] b   Another length
     * \return    The area resulting from the product of two lengths
    */
    constexpr Area operator* (Length const& a, Length const& b) noexcept {
      return Area{a.get()*b.get()};
    }

    /**\fn        operator*
     * \brief     Multiplication operator between a length and an area resulting in an volume
     * 
     * \param[in] l   A length
     * \param[in] a   An area
     * \return    The volume resulting from the product of a length and an area
    */
    constexpr Volume operator* (Length const& l, Area const& a) noexcept {
      return Volume{a.get()*l.get()};
    }

    /**\fn        operator*
     * \brief     Multiplication operator between a length and an area resulting in an volume
     *            For symmetry reasons
     * 
     * \param[in] a   An area
     * \param[in] l   A length
     * \return    The volume resulting from the product of a length and an area
    */
    constexpr Volume operator* (Area const& a, Length const& l) noexcept {
      return Volume{a.get()*l.get()};
    }

    /**\fn        operator/
     * \brief     Division operator between a length and a time resulting in a velocity
     * 
     * \param[in] l   A distance
     * \param[in] t   The time it took to travel the distance
     * \return    The velocity resulting from the division of the length by the time
    */
    constexpr Velocity operator/ (Length const& l, Time const& t) noexcept {
      return Velocity{l.get()/t.get()};
    }

    /**\fn        operator/
     * \brief     Division operator between a mass and a volume resulting in a density
     * 
     * \param[in] m   A mass
     * \param[in] v   The volume that the mass takes up
     * \return    The density resulting from the division of the mass by the time
    */
    constexpr Density operator/ (Mass const& m, Volume const& v) noexcept {
      return Density{m.get()/v.get()};
    }

    /**\fn        operator/
     * \brief     Division operator between an area and a time resulting in a kinematic viscosity
     * 
     * \param[in] a   An area
     * \param[in] t   A time
     * \return    The kinematic viscosity resulting from the division of an area by a time
    */
    constexpr KinematicViscosity operator/ (Area const& a, Time const& t) noexcept {
      return KinematicViscosity{a.get()/t.get()};
    }

    /**\fn        operator*
     * \brief     Multiplication operator between a velocity and a length resulting in a kinematic viscosity
     * 
     * \param[in] v   A velocity
     * \param[in] l   A length
     * \return    The kinematic viscosity resulting from the multiplication of a velocity and a length
    */
    constexpr KinematicViscosity operator* (Velocity const& v, Length const& l) noexcept {
      return KinematicViscosity{v.get()*l.get()};
    }
    constexpr KinematicViscosity operator* (Length const& l, Velocity const& v) noexcept {
      return KinematicViscosity{v.get()*l.get()};
    }

    /**\fn        operator/
     * \brief     Division operator between a dynamic viscosity and a density resulting in a kinematic viscosity
     * 
     * \param[in] mu    A dynamic viscosity
     * \param[in] rho   A density
     * \return    The kinematic viscosity resulting from the division of a dynamic viscosity and a density
    */
    constexpr KinematicViscosity operator/ (DynamicViscosity const& mu, Density const& rho) noexcept {
      return KinematicViscosity{mu.get()/rho.get()};
    }

    /**\fn        operator*
     * \brief     Multiplication operator between a pressure and a time resulting in a dynamic viscosity
     * 
     * \param[in] p   A pressure
     * \param[in] t   A time
     * \return    The dynamic viscosity resulting from the multiplication of a pressure and a time
    */
    constexpr DynamicViscosity operator* (Pressure const& p, Time const& t) noexcept {
      return DynamicViscosity{p.get()*t.get()};
    }
    constexpr DynamicViscosity operator* (Time const& t, Pressure const& p) noexcept {
      return DynamicViscosity{p.get()*t.get()};
    }

    /**\fn        operator*
     * \brief     Multiplication operator between a kinematic viscosity and a density resulting in a dynamic viscosity
     * 
     * \param[in] nu    A kinematic viscosity
     * \param[in] rho   A density
     * \return    The dynamic viscosity resulting from the multiplication of a kinematic viscosity and a density
    */
    constexpr DynamicViscosity operator* (KinematicViscosity const& nu, Density const& rho) noexcept {
      return DynamicViscosity{nu.get()*rho.get()};
    }
    constexpr DynamicViscosity operator* (Density const& rho, KinematicViscosity const& nu) noexcept {
      return DynamicViscosity{nu.get()*rho.get()};
    }

    /**\fn        operator/
     * \brief     Division operator between a mass and an amount of substance resulting in a molar mass
     * 
     * \param[in] m   A mass
     * \param[in] a   An amount of substance
     * \return    The molar mass resulting from the division of an amount of substance and a molar mass
    */
    constexpr MolarMass operator/ (Mass const& m, AmountOfSubstance const& a) noexcept {
      return MolarMass{m.get()/a.get()};
    }

  }
}

#endif // LBT_UNIT_OPERATORS
