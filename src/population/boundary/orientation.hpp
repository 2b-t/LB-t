#ifndef LBT_BOUNDARY_ORIENTATION
#define LBT_BOUNDARY_ORIENTATION
#pragma once

/**
 * \file     orientation.hpp
 * \mainpage Holds orientation parameters for boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <cstdint>
#include <ostream>


namespace lbt {
  namespace boundary{

    /**\enum  boundary::Orientation
     * \brief Strongly typed enum for all possible boundary orientations
    */
    enum class Orientation { Left, Right, Front, Back, Bottom, Top };

    /**\fn        operator!
     * \brief     Negation operator for boundary orientation: Reverses the cartesian direction
     *
     * \param[in] orientation   Orientation to be reversed
     * \return    Reverse orientation of the given orientation if any, else the value itself is returned
    */
    inline constexpr Orientation operator! (Orientation const& orientation) noexcept {
      switch (orientation) {
        case Orientation::Left:
          return Orientation::Right;
        case Orientation::Right:
          return Orientation::Left;
        case Orientation::Front:
          return Orientation::Back;
        case Orientation::Back:
          return Orientation::Front;
        case Orientation::Bottom:
          return Orientation::Top;
        case Orientation::Top:
          return Orientation::Bottom;
        default:
          return orientation;
        }
    }

    /**\fn            Orientation output stream operator
     * \brief         Output stream operator for the boundary orientation
     *
     * \param[in,out] os            Output stream
     * \param[in]     orientation   Orientation to be printed to output stream
     * \return        Output stream including the boundary condition orientation
    */
    inline std::ostream& operator<< (std::ostream& os, Orientation const orientation) noexcept {
      switch (orientation) {
        case Orientation::Left:
          os << "left";
          break;
        case Orientation::Right:
          os << "right";
          break;
        case Orientation::Front:
          os << "front";
          break;
        case Orientation::Back:
          os << "back";
          break;
        case Orientation::Bottom:
          os << "bottom";
          break;
        case Orientation::Top:
          os << "top";
          break;
        default:
          os << "none";
          break;
        }
        return os;
    }

  }
}

#endif // LBT_BOUNDARY_ORIENTATION
