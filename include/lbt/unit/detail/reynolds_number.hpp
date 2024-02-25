/**
 * \file     reynolds_number.hpp
 * \mainpage Contains definitions for Reynolds number
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_UNIT_REYNOLDS_NUMBER
#define LBT_UNIT_REYNOLDS_NUMBER
#pragma once

#include "lbt/unit/detail/length.hpp"
#include "lbt/unit/detail/kinematic_viscosity.hpp"
#include "lbt/unit/detail/velocity.hpp"


namespace lbt {
  namespace unit {

    /**\class ReynoldsNumber
     * \brief Reynolds number: dimensionless characteristic number for fluid flow
    */
    class ReynoldsNumber {
      public:
        /**\fn    compute
         * \brief Function for computing the Reynolds number according to velocity*length/kinematic_viscosity
         * 
         * \param[in] velocity              The characteristic velocity of the flow used to calculate the Reynolds number
         * \param[in] length                The characteristic length of the flow used to calculate the Reynolds number
         * \param[in] kinematic_viscosity   The kinematic viscosity of the flowing medium used to calculate the Reynolds number
         * \return    The dimensionless Reynolds number computed from the input parameters
        */
        static constexpr auto compute(lbt::unit::Velocity const& velocity, lbt::unit::Length const& length, 
                                      lbt::unit::KinematicViscosity const& kinematic_viscosity) noexcept {
          return velocity.get()*length.get()/kinematic_viscosity.get();
        }

        /**\fn    ReynoldsNumber
         * \brief Constructor
         * 
         * \param[in] value   The Reynolds number of the value to be stored
        */
        constexpr ReynoldsNumber(long double const value) noexcept
          : value{value} {
          return;
        }
        ReynoldsNumber() = delete;
        ReynoldsNumber(ReynoldsNumber const&) = default;
        ReynoldsNumber& operator= (ReynoldsNumber const&) = default;
        ReynoldsNumber(ReynoldsNumber&&) = default;
        ReynoldsNumber& operator= (ReynoldsNumber&&) = default;

        /**\fn    ReynoldsNumber
         * \brief Constructor
         * 
         * \param[in] velocity              The characteristic velocity of the flow used to calculate the Reynolds number
         * \param[in] length                The characteristic length of the flow used to calculate the Reynolds number
         * \param[in] kinematic_viscosity   The kinematic viscosity of the flowing medium used to calculate the Reynolds number
        */
        constexpr ReynoldsNumber(lbt::unit::Velocity const& velocity, lbt::unit::Length const& length, 
                                 lbt::unit::KinematicViscosity const& kinematic_viscosity) noexcept
          : value{compute(velocity, length, kinematic_viscosity)} {
          return;
        }

        /**\fn    get
         * \brief Function for getting the value of the Reynolds number
         * 
         * \return The dimensionless Reynolds number encapsulated inside the class
        */
        constexpr auto get() const noexcept {
          return value;
        }

        /**\fn    operator long double
         * \brief Custom conversion operator for converting the Reynolds number to a long double
         * 
         * \return The dimensionless Reynolds number as a long double
        */
        constexpr operator long double() const noexcept {
          return value;
        }

      protected:
        long double value;
    };
  }
}

#endif // LBT_UNIT_REYNOLDS_NUMBER
