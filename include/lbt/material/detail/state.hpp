/**
 * \file     state.hpp
 * \mainpage Contains a struct for a given thermodynamic state of a fluid
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_MATERIAL_STATE
#define LBT_MATERIAL_STATE
#pragma once

#include "lbt/unit/units.hpp"


namespace lbt {
  namespace material {

    /**\class  State
     * \brief  A thermodynamic state of a fluid characterised by its pressure, temperature and density
    */
    class State {
      public:
        /**\fn        State
         * \brief     Constructor
         *
         * \param[in] pressure      The static pressure of the given fluid state
         * \param[in] temperature   The temperature of the given fluid state
         * \param[in] density       The density of the given fluid state
        */
        explicit constexpr State(lbt::unit::Pressure const pressure, lbt::unit::Temperature const temperature, lbt::unit::Density const density) noexcept
        : pressure_{pressure}, temperature_{temperature}, density_{density} {
          return;
        }

        State() = delete;
        State(State const&) = default;
        State& operator= (State const&) = default;
        State(State&&) = default;
        State& operator= (State&&) = default;

        /**\fn     get
         * \brief  Getter for state variables of the given state
         *         Required for extracting the corresponding fields with template meta-programming
         *
         * \tparam T   The type of state variable to be extracte from the state
         * \return The state variable of type T in the current state
        */
        template <typename T>
        constexpr T get() const noexcept = delete;

      protected:
        lbt::unit::Pressure pressure_;
        lbt::unit::Temperature temperature_;
        lbt::unit::Density density_;
    };

    template <>
    constexpr lbt::unit::Pressure State::get() const noexcept {
      return pressure_;
    }

    template <>
    constexpr lbt::unit::Temperature State::get() const noexcept {
      return temperature_;
    }

    template <>
    constexpr lbt::unit::Density State::get() const noexcept {
      return density_;
    }

  }
}

#endif // LBT_MATERIAL_STATE
