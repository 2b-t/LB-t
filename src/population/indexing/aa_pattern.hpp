#ifndef LBT_AA_PATTERN
#define LBT_AA_PATTERN
#pragma once

/**
 * \file     aa_pattern.hpp
 * \brief    Class members for indexing of populations with A-A access pattern
 * \author   Tobit Flatscher (github.com/2b-t)
 *
 * \mainpage The A-A access pattern avoids the usage of two distinct populations before and
 *           after streaming by treating even and odd time steps differently: Even time steps
 *           perform only a local collision step with a reverse read of the populations and a
 *           regular write while odd steps perform a combined streaming-collision-streaming
 *           step with a regular read and a reverse write.
 *           This is implemented by different macros that determine the population indices for
 *           even and odd time steps.
*/

#include <array>
#include <cstdint>
#include <iostream>

#include "../../general/type_definitions.hpp"
#include "indexing.hpp"


namespace lbt {

  /**\class  AaPattern
   * \brief  Class that is responsible for indexing according to Bailey's A-A pattern
   *
   * \note   "Accelerating Lattice Boltzmann Fluid Flow Simulations Using Graphics Processors"
   *         P. Bailey, J. Myre, S.D.C. Walsh, D.J. Lilja, M.O. Saar
   *         38th International Conference on Parallel Processing (ICPP), Vienna, Austria (2009)
   *         DOI: 10.1109/ICPP.2009.38
   *
   * \tparam LT   Static lattice::DdQq class containing discretisation parameters
   * \tparam NP   Number of populations stored side by side in a single merged grid (default = 1)
  */
  template <typename LT, std::int32_t NP>
  class AaPattern: public Indexing<LT,NP> {
    public:
      constexpr AaPattern(std::int32_t const NX, std::int32_t const NY, std::int32_t const NZ) noexcept
        : Indexing<LT,NP>{NX,NY,NZ} {
        return;
      }
      AaPattern() = delete;
      AaPattern(AaPattern const&) = default;
      AaPattern& operator= (AaPattern const&) = default;
      AaPattern(AaPattern&&) = default;
      AaPattern& operator= (AaPattern&&) = default;

      /**\fn        oddEven
       * \brief     Function for access of individual indices in odd and even time steps
       * \warning   Reduces the range of unsigned int -> int and might overflow!
       *
       * \param[in] ts           boolean index for odd (1 = true) and even (0 = false) time steps
       * \param[in] odd_index    index to be accessed at an odd time step
       * \param[in] even_index   index to be accessed at an even time step
      */
      template <Timestep TS>
      LBT_FORCE_INLINE static constexpr std::int32_t oddEven(std::int32_t const odd_index, std::int32_t const even_index) noexcept {
        return (TS == Timestep::Odd) ? odd_index : even_index;
      }

      /**\fn        indexRead
       * \brief     Function for determining linear index when reading/writing values before collision depending on even and odd time step.
       * \warning   Inline function! Has to be declared in header!
       *
       * \tparam    TS    Even (0, false) or odd (1, true) time step
       * \param[in] x     x coordinates of current cell and its neighbours [(NX+x-1)%NX,x,(x+1)%NX]
       * \param[in] y     y coordinates of current cell and its neighbours [(NY+y-1)%NY,y,(y+1)%NY]
       * \param[in] z     z coordinates of current cell and its neighbours [(NZ+z-1)%NZ,z,(z+1)%NZ]
       * \param[in] n     Positive (0) or negative (1) index/lattice velocity
       * \param[in] d     Relevant population index
       * \param[in] p     Relevant population (default = 0)
       * \return    Requested linear population index before collision
      */
      template <Timestep TS>
      LBT_FORCE_INLINE constexpr std::int64_t indexRead(lbt::StackArray<std::int32_t,3> const& x,
                                                        lbt::StackArray<std::int32_t,3> const& y,
                                                        lbt::StackArray<std::int32_t,3> const& z,
                                                        std::int32_t                    const n,
                                                        std::int32_t                    const d,
                                                        std::int32_t                    const p) const noexcept {
        return Indexing<LT,NP>::spatialToLinear(x[1 + oddEven<TS>(static_cast<std::int32_t>(LT::DX[(!n)*LT::OFF+d]), 0)],
                                                y[1 + oddEven<TS>(static_cast<std::int32_t>(LT::DY[(!n)*LT::OFF+d]), 0)],
                                                z[1 + oddEven<TS>(static_cast<std::int32_t>(LT::DZ[(!n)*LT::OFF+d]), 0)],
                                                oddEven<TS>(n, !n),
                                                d, p);
      }
      template <Timestep TS>
      LBT_FORCE_INLINE constexpr std::int64_t indexRead(std::int32_t const x,
                                                        std::int32_t const y,
                                                        std::int32_t const z,
                                                        std::int32_t const n,
                                                        std::int32_t const d,
                                                        std::int32_t const p) const noexcept {
        if constexpr (TS == Timestep::Odd) {
          std::int32_t const xn = (Indexing<LT,NP>::NX + x + static_cast<std::int32_t>(LT::DX[(!n)*LT::OFF+d])) % Indexing<LT,NP>::NX;
          std::int32_t const yn = (Indexing<LT,NP>::NY + y + static_cast<std::int32_t>(LT::DY[(!n)*LT::OFF+d])) % Indexing<LT,NP>::NY;
          std::int32_t const zn = (Indexing<LT,NP>::NZ + z + static_cast<std::int32_t>(LT::DZ[(!n)*LT::OFF+d])) % Indexing<LT,NP>::NZ;
          return Indexing<LT,NP>::spatialToLinear(xn,yn,zn,n,d,p);
        } else {
          return Indexing<LT,NP>::spatialToLinear(x,y,z,!n,d,p);
        }
      }

      /**\fn        indexWrite
       * \brief     Function for determining linear index when reading/writing values after collision depending on even and odd time step.
       * \warning   Inline function! Has to be declared in header!
       *
       * \tparam    TS    Even (0, false) or odd (1, true) time step
       * \param[in] x     x coordinates of current cell and its neighbours [(NX+x-1)%NX,x,(x+1)%NX]
       * \param[in] y     y coordinates of current cell and its neighbours [(NY+y-1)%NY,y,(y+1)%NY]
       * \param[in] z     z coordinates of current cell and its neighbours [(NZ+z-1)%NZ,z,(z+1)%NZ]
       * \param[in] n     Positive (0) or negative (1) index/lattice velocity
       * \param[in] d     Relevant population index
       * \param[in] p     Relevant population (default = 0)
       * \return    Requested linear population index after collision
      */
      template <Timestep TS>
      LBT_FORCE_INLINE constexpr std::int64_t indexWrite(lbt::StackArray<std::int32_t,3> const& x,
                                                         lbt::StackArray<std::int32_t,3> const& y,
                                                         lbt::StackArray<std::int32_t,3> const& z,
                                                         std::int32_t                    const n,
                                                         std::int32_t                    const d,
                                                         std::int32_t                    const p) const noexcept {
        return Indexing<LT,NP>::spatialToLinear(x[1 - oddEven<TS>(0, static_cast<std::int32_t>(LT::DX[n*LT::OFF+d]))],
                                                y[1 - oddEven<TS>(0, static_cast<std::int32_t>(LT::DY[n*LT::OFF+d]))],
                                                z[1 - oddEven<TS>(0, static_cast<std::int32_t>(LT::DZ[n*LT::OFF+d]))],
                                                oddEven<TS>(!n, n),
                                                d, p);
      }
      template <Timestep TS>
      LBT_FORCE_INLINE constexpr std::int64_t indexWrite(std::int32_t const x,
                                                         std::int32_t const y,
                                                         std::int32_t const z,
                                                         std::int32_t const n,
                                                         std::int32_t const d,
                                                         std::int32_t const p) const noexcept {
        if constexpr (TS == Timestep::Odd) {
          return Indexing<LT,NP>::spatialToLinear(x,y,z,!n,d,p);
        } else {
          std::int32_t const xn = (Indexing<LT,NP>::NX + x - static_cast<std::int32_t>(LT::DX[n*LT::OFF+d])) % Indexing<LT,NP>::NX;
          std::int32_t const yn = (Indexing<LT,NP>::NY + y - static_cast<std::int32_t>(LT::DY[n*LT::OFF+d])) % Indexing<LT,NP>::NY;
          std::int32_t const zn = (Indexing<LT,NP>::NZ + z - static_cast<std::int32_t>(LT::DZ[n*LT::OFF+d])) % Indexing<LT,NP>::NZ;
          return Indexing<LT,NP>::spatialToLinear(xn,yn,zn,n,d,p);
        }
      }
  };

}

#endif // LBT_AA_PATTERN
