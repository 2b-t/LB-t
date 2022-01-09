#ifndef LBT_INDEXING
#define LBT_INDEXING
#pragma once

/**
 * \file     indexing.hpp
 * \brief    Base class members for indexing of populations with different access patterns
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <tuple>

#include "../../general/type_definitions.hpp"


namespace lbt {

  /**\enum  Timestep
   * \brief Strongly typed enum for even and odd time steps required for AA access pattern
   */
  enum class Timestep: bool { Even = false, Odd = true };

  /**\fn        Negation timestep operator
   * \brief     Negation operator for the timestep
   *
   * \param[in] ts   Timestep to be negated
   * \return    Negated timestep
   */
  constexpr Timestep operator! (Timestep const& ts) noexcept {
    return (ts == Timestep::Even) ? Timestep::Odd : Timestep::Even;
  }

  /**\fn            Timestep output stream operator
   * \brief         Output stream operator for the timestep
   *
   * \param[in,out] os   Output stream
   * \param[in]     ts   Timestep to be printed to output stream
   * \return        Output stream including the type of timestep
   */
  std::ostream& operator << (std::ostream& os, Timestep const& ts) noexcept {
    os << ((ts == Timestep::Even) ? "even time step" : "odd time step");
    return os;
  }

  /**\class  Indexing
   * \brief  Class for indexing of a population
   *
   * \tparam LT   Static lattice::DdQq class containing discretisation parameters and floating data-type
   * \tparam NP   Number of populations stored side by side in a single merged grid (default = 1)
  */
  template <typename LT, std::int32_t NP>
  class Indexing {
    public:
      /**\fn        Indexing
       * \brief     Constructor
       *
       * \param[in] NX   Simulation domain resolution in x-direction
       * \param[in] NY   Simulation domain resolution in y-direction
       * \param[in] NZ   Simulation domain resolution in z-direction
      */
      constexpr Indexing(std::int32_t const NX, std::int32_t const NY, std::int32_t const NZ) noexcept
        : NX{NX}, NY{NY}, NZ{NZ} {
        return;
      }
      Indexing() = delete;
      Indexing(Indexing const&) = default;
      Indexing(Indexing&&) = default;
      Indexing& operator= (Indexing const&) = default;
      Indexing& operator= (Indexing&&) = default;

      /**\fn        spatialToLinear
       * \brief     Inline function for converting 3D population coordinates to scalar index
       *
       * \param[in] x   x coordinate of cell
       * \param[in] y   y coordinate of cell
       * \param[in] z   z coordinate of cell
       * \param[in] n   Positive (0) or negative (1) index/lattice velocity
       * \param[in] d   Relevant population index
       * \param[in] p   Relevant population (default = 0)
       * \return    Requested linear population index
      */
      LBT_FORCE_INLINE constexpr std::int64_t spatialToLinear(std::int32_t const x, std::int32_t const y, std::int32_t const z,
                                                              std::int32_t const n, std::int32_t const d, std::int32_t const p) const noexcept {
        return (((static_cast<std::int64_t>(z)*NY + y)*NX + x)*NP + p)*LT::ND + n*LT::OFF + d;
      }

      /**\fn        linearToSpatial
       * \brief     Generate 3D population coordinates from scalar index
       *
       * \param[in] index   Current linear population index
       * \return    Return the x, y, z coordinates as well as the positive (0) or negative (1) index n and the number of relevant 
       *            population index d belonging to the scalar index and the index of the population p
      */
      constexpr std::tuple<std::int32_t,std::int32_t,std::int32_t,std::int32_t,std::int32_t,std::int32_t>
      linearToSpatial(std::int64_t const index) const noexcept;

    protected:
      std::int32_t NX;
      std::int32_t NY;
      std::int32_t NZ;
  };


  template <typename LT, std::int32_t NP>
  constexpr std::tuple<std::int32_t,std::int32_t,std::int32_t,std::int32_t,std::int32_t,std::int32_t>
  Indexing<LT,NP>::linearToSpatial(std::int64_t const index) const noexcept {
    std::int64_t factor {LT::ND*NP*NX*NY};
    std::int64_t rest {index%factor};

    std::int32_t const z {static_cast<std::int32_t>(index/factor)};

    factor = LT::ND*NP*NX;
    std::int32_t const y {static_cast<std::int32_t>(rest/factor)};
    rest   = rest%factor;

    factor = LT::ND*NP;
    std::int32_t const x {static_cast<std::int32_t>(rest/factor)};
    rest   = rest%factor;

    factor = LT::ND;
    std::int32_t const p {static_cast<std::int32_t>(rest/factor)};
    rest   = rest%factor;

    factor = LT::OFF;
    std::int32_t const n {static_cast<std::int32_t>(rest/factor)};
    rest   = rest%factor;

    factor = LT::SPEEDS;
    std::int32_t const d {static_cast<std::int32_t>(rest%factor)};

    return std::make_tuple(x,y,z,n,d,p);
  }

}

#endif // LBT_INDEXING
