#ifndef LBT_D3Q15
#define LBT_D3Q15
#pragma once

/**
 * \file     D3Q15.hpp
 * \mainpage Discretisation parameters for D3Q15-lattice
 * \author   Tobit Flatscher (github.com/2b-t)
 *
 * \warning  Static classes with more complex members such as std::vector and std::array require C++17
*/


#include <cstdint>
#include <type_traits>

#include "../general/constexpr_math.hpp"
#include "../general/type_definitions.hpp"


namespace lbt {
  namespace lattice {

    /**\class  lattice::D3Q15P16
     * \brief  Class for D3Q15 lattice with padding to 16
     *
     * \note   "Lattice BGK models for Navier-Stokes equation"
     *         Y.H. Qian, D. D'Humières, P. Lallemand
     *         Europhysics Letters (EPL) Vol. 17 (1992)
     *         DOI: 10.1209/0295-5075/17/6/001
     *
     * \tparam T   Floating data type used for simulation
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    class D3Q15P16 final {
      public:
        using type = T;

        /// lattice discretisation parameters
        static constexpr std::int32_t    DIM =  3;
        static constexpr std::int32_t SPEEDS = 15;
        static constexpr std::int32_t HSPEED = (SPEEDS + 1)/2;

        /// linear memory layout padding
        static constexpr std::int32_t PAD = 1;
        static constexpr std::int32_t  ND = SPEEDS + PAD;
        static constexpr std::int32_t OFF = ND/2;

        /// discrete velocities
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DX =
        { 0,  1,  0,  0,  1,  1,  1,  1,   // positive velocities
          0, -1,  0,  0, -1, -1, -1, -1 }; // negative velocities
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DY =
        { 0,  0,  1,  0,  1, -1,  1, -1,
          0,  0, -1,  0, -1,  1, -1,  1 };
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DZ =
        { 0,  0,  0,  1,  1,  1, -1, -1,
          0,  0,  0, -1, -1, -1,  1,  1 };

        /// corresponding weights
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> W =
        { 2.0/9.0,                                  // positive velocities
          1.0/9.0, 1.0/9.0, 1.0/9.0,
          1.0/72.0, 1.0/72.0, 1.0/72.0, 1.0/72.0,
          2.0/9.0,                                  // negative velocities
          1.0/9.0, 1.0/9.0, 1.0/9.0,
          1.0/72.0, 1.0/72.0, 1.0/72.0, 1.0/72.0 };

        /// logical mask for relevant populations
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> MASK =
        { 1, 1, 1, 1, 1, 1, 1, 1, 
          0, 1, 1, 1, 1, 1, 1, 1 };

        /// lattice speed of sound
        static constexpr T CS = 1.0/cem::sqrt(3.0);
    };

    /// Alias default lattice
    template<typename T> using D3Q15 = D3Q15P16<T>;

  }
}

#endif // LBT_D3Q15
