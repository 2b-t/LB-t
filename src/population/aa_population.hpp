#ifndef LBT_AA_POPULATION
#define LBT_AA_POPULATION
#pragma once

/**
 * \file     aa_population.hpp
 * \mainpage Class for microscopic populations based on a single population with the A-A access pattern
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cassert>
#include <cstdint>
#include <iostream>

#include "../general/type_definitions.hpp"
#include "indexing/aa_pattern.hpp"
#include "indexing/indexing.hpp"


namespace lbt {

  /**\class  AaPopulation
   * \brief  Class that holds macroscopic values with indexing based on the A-A access pattern
   *
   * \tparam LT   Static lattice::DdQq class containing discretisation parameters
   * \tparam NP   Number of populations stored side by side in a single merged grid (default = 1)
  */
  template <class LT, std::int32_t NP = 1>
  class AaPopulation: public AaPattern<LT,NP> {
    public:
      AaPopulation() = delete;

      /**\fn    AaPopulation
       * \brief Class constructor
      */
      AaPopulation(std::int32_t const NX, std::int32_t const NY, std::int32_t const NZ) noexcept
        : AaPattern<LT,NP>{NX, NY, NZ}, A(static_cast<std::size_t>(NZ)*NY*NX*NP*LT::ND) {
        static_assert((LT::DIM == 2) ? (NZ == 1) : true, "Two-dimensional lattice with NZ != 1.");
        return;
      }
      AaPopulation(AaPopulation&) = default;
      AaPopulation& operator= (AaPopulation&) = default;
      AaPopulation(AaPopulation&&) = default;
      AaPopulation& operator= (AaPopulation&&) = default;

      /**\fn        read
       * \brief     Function for accessing values before collision depending on even and odd time step.
       * \warning   Sloppy syntax: avoid usage, use the full expression below for indexing
       *
       * \tparam    TS    Even (0, false) or odd (1, true) time step
       * \param[in] x     x coordinates of current cell and its neighbours [x-1,x,x+1]
       * \param[in] y     y coordinates of current cell and its neighbours [y-1,y,y+1]
       * \param[in] z     z coordinates of current cell and its neighbours [z-1,z,z+1]
       * \param[in] n     Positive (0) or negative (1) index/lattice velocity
       * \param[in] d     Relevant population index
       * \param[in] p     Relevant population (default = 0)
       * \return    Requested linear population index before collision (reading)
      */
      template <Timestep TS>
      LBT_FORCE_INLINE auto& read(std::array<std::int32_t,3> const &x,
                                  std::array<std::int32_t,3> const &y,
                                  std::array<std::int32_t,3> const &z,
                                  std::int32_t               const n,
                                  std::int32_t               const d,
                                  std::int32_t               const p = 0) noexcept {
        return A[AaPattern<LT,NP>::indexRead<TS>(x,y,z,n,d,p)];
      }

      template <Timestep TS>
      LBT_FORCE_INLINE auto const& read(std::array<std::int32_t,3> const &x,
                                        std::array<std::int32_t,3> const &y,
                                        std::array<std::int32_t,3> const &z,
                                        std::int32_t               const n,
                                        std::int32_t               const d,
                                        std::int32_t               const p = 0) const noexcept {
        return A[AaPattern<LT,NP>::indexRead<TS>(x,y,z,n,d,p)];
      }

      /**\fn        write
       * \brief     Function for accessing values after collision depending on even and odd time step.
       * \warning   Sloppy syntax: avoid usage, use the full expression below for indexing
       *
       * \tparam    TS    Even (0, false) or odd (1, true) time step
       * \param[in] x     x coordinates of current cell and its neighbours [x-1,x,x+1]
       * \param[in] y     y coordinates of current cell and its neighbours [y-1,y,y+1]
       * \param[in] z     z coordinates of current cell and its neighbours [z-1,z,z+1]
       * \param[in] n     Positive (0) or negative (1) index/lattice velocity
       * \param[in] d     Relevant population index
       * \param[in] p     Relevant population (default = 0)
       * \return    Requested linear population index after collision (writing)
      */
      template <Timestep TS>
      LBT_FORCE_INLINE auto& write(std::array<std::int32_t,3> const &x, 
                                   std::array<std::int32_t,3> const &y,
                                   std::array<std::int32_t,3> const &z,
                                   std::int32_t               const n,
                                   std::int32_t               const d,
                                   std::int32_t               const p = 0) noexcept {
        return A[AaPattern<LT,NP>::indexWrite<TS>(x,y,z,n,d,p)];
      }

      template <Timestep TS>
      LBT_FORCE_INLINE auto const& write(std::array<std::int32_t,3> const &x,
                                         std::array<std::int32_t,3> const &y,
                                         std::array<std::int32_t,3> const &z,
                                         std::int32_t               const n,
                                         std::int32_t               const d,
                                         std::int32_t               const p) const noexcept {
        return A[AaPattern<LT,NP>::indexWrite<TS>(x,y,z,n,d,p)];
      }

      // LBT_FORCE_INLINE lbt::array<LT::type,LT::ND> read(std::int32_t const x, std::int32_t const y, std::int32_t const z)
      // Vectorised and non-vectorised store and load

    protected:
      using T = typename LT::type;
      LBT_ALIGN lbt::HeapArray<T> A;
  };

}

#endif // LBT_AA_POPULATION
