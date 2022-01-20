#ifndef LBT_BOUNDARY_ORIENTATION
#define LBT_BOUNDARY_ORIENTATION
#pragma once

/**
 * \file     boundary_orientation.hpp
 * \mainpage Holds orientation parameters for boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <ostream>


namespace lbt {
  namespace boundary{

    /**\enum  boundary::Orientation
     * \brief Strongly typed for all possible boundary orientations
    */
    enum class Orientation { Left, Right, Front, Back, Bottom, Top };

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
          os << "Left";
          break;
        case Orientation::Right:
          os << "Right";
          break;
        case Orientation::Front:
          os << "Front";
          break;
        case Orientation::Back:
          os << "Back";
          break;
        case Orientation::Bottom:
          os << "Bottom";
          break;
        case Orientation::Top:
          os << "Top";
          break;
        default:
          os << "None";
          break;
        }
        return os;
    }

    /**\class  Normal
     * \brief  Class for the normals for each individual direction
     *
     * \tparam O   Orientation of the normal
    */
    template <Orientation O>
    class Normal {
      public:
        static constexpr std::int32_t x =  0;
        static constexpr std::int32_t y =  0;
        static constexpr std::int32_t z =  0;
    };

    template <>
    class Normal<Orientation::Left> {
      public:
        static constexpr std::int32_t x =  1; ///< normal direction pointing into the fluid volume
        static constexpr std::int32_t y =  0;
        static constexpr std::int32_t z =  0;
    };

    template <>
    class Normal<Orientation::Right> {
      public:
        static constexpr std::int32_t x = -1;
        static constexpr std::int32_t y =  0;
        static constexpr std::int32_t z =  0;
    };

    template <>
    class Normal<Orientation::Front> {
      public:
        static constexpr std::int32_t x =  0;
        static constexpr std::int32_t y =  1;
        static constexpr std::int32_t z =  0;
    };

    template <>
    class Normal<Orientation::Back> {
      public:
        static constexpr std::int32_t x =  0;
        static constexpr std::int32_t y = -1;
        static constexpr std::int32_t z =  0;
    };

    template <>
    class Normal<Orientation::Bottom> {
      public:
        static constexpr std::int32_t x =  0;
        static constexpr std::int32_t y =  0;
        static constexpr std::int32_t z =  1;
    };

    template <>
    class Normal<Orientation::Top> {
      public:
        static constexpr std::int32_t x =  0;
        static constexpr std::int32_t y =  0;
        static constexpr std::int32_t z = -1;
    };

  }
}

#endif // LBT_BOUNDARY_ORIENTATION
