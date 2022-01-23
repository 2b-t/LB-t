#ifndef LBT_D3Q27
#define LBT_D3Q27
#pragma once

/**
 * \file     D3Q27.hpp
 * \mainpage Discretisation parameters for D3Q27-lattice
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

    /**\class  lattice::D3Q27P28
     * \brief  Class for D3Q27 lattice with padding to 28
     *
     * \note   "Lattice BGK models for Navier-Stokes equation"
     *         Y.H. Qian, D. D'Humières, P. Lallemand
     *         Europhysics Letters (EPL) Vol. 17 (1992)
     *         DOI: 10.1209/0295-5075/17/6/001
     *
     * \tparam T   Floating data type used for simulation
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    class D3Q27P28 final {
      public:
        using type = T;

        /// lattice discretisation parameters
        static constexpr std::int32_t    DIM =  3;
        static constexpr std::int32_t SPEEDS = 27;
        static constexpr std::int32_t HSPEED = (SPEEDS + 1)/2;

        /// linear memory layout padding
        static constexpr std::int32_t PAD = 1;
        static constexpr std::int32_t  ND = SPEEDS + PAD;
        static constexpr std::int32_t OFF = ND/2;

        /// discrete velocities
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DX =
        { 0,  1,  0,  0,  1,  1,  1,   // positive velocities
          1,  0,  0,  1,  1,  1,  1,
          0, -1,  0,  0, -1, -1, -1,   // negative velocities
         -1,  0,  0, -1, -1, -1, -1 };
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DY =
        { 0,  0,  1,  0,  1, -1,  0,
          0,  1,  1,  1, -1,  1, -1,
          0,  0, -1,  0, -1,  1,  0,
          0, -1, -1, -1,  1, -1,  1 };
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DZ =
        { 0,  0,  0,  1,  0,  0,  1,
         -1,  1, -1,  1,  1, -1, -1,
          0,  0,  0, -1,  0,  0, -1,
          1, -1,  1, -1, -1,  1,  1 };

        /// corresponding weights
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> W =
        { 8.0/27.0,                       // positive velocities
          2.0/27.0,  2.0/27.0,  2.0/27.0,
          1.0/54.0,  1.0/54.0,  1.0/54.0,
          1.0/54.0,  1.0/54.0,  1.0/54.0,
          1.0/216.0, 1.0/216.0,
          1.0/216.0, 1.0/216.0,
          8.0/27.0,                       // negative velocities
          2.0/27.0,  2.0/27.0,  2.0/27.0,
          1.0/54.0,  1.0/54.0,  1.0/54.0,
          1.0/54.0,  1.0/54.0,  1.0/54.0,
          1.0/216.0, 1.0/216.0,
          1.0/216.0, 1.0/216.0 };

        /// logical mask for relevant populations
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> MASK =
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
          0, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1 };

        /// lattice speed of sound
        static constexpr T CS = 1.0/cem::sqrt(3.0);
    };

    /// Alias default lattice
    template<typename T> using D3Q27 = D3Q27P28<T>;

    /**\class  lattice::D3Q27PC
     * \brief  Class for D3Q27 lattice with padding to match a full cache line
     *
     * \note   "Lattice BGK models for Navier-Stokes equation"
     *         Y.H. Qian, D. D'Humières, P. Lallemand
     *         Europhysics Letters (EPL) Vol. 17 (1992)
     *         DOI: 10.1209/0295-5075/17/6/001
     *
     * \tparam T   floating data type used for simulation
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    class D3Q27PC final {
      public:
        using type = T;

        /// lattice discretisation parameters
        static constexpr std::int32_t    DIM =  3;
        static constexpr std::int32_t SPEEDS = 27;
        static constexpr std::int32_t HSPEED = (SPEEDS + 1)/2;

        /// linear memory layout padding
        static constexpr std::int32_t PAD = ((lbt::alignment - sizeof(T)*SPEEDS % lbt::alignment) % lbt::alignment) / sizeof(T);
        static constexpr std::int32_t  ND = SPEEDS + PAD;
        static constexpr std::int32_t OFF = ND/2;

        /// discrete velocities
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DX =
        { 0,  1,  0,  0,  1,  1,  1, // positive velocities
          1,  0,  0,  1,  1,  1,  1,
          0,  0,                     // padding
          0, -1,  0,  0, -1, -1, -1, // negative velocities
         -1,  0,  0, -1, -1, -1, -1,
          0,  0 };
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DY =
        { 0,  0,  1,  0,  1, -1,  0,
          0,  1,  1,  1, -1,  1, -1,
          0,  0,
          0,  0, -1,  0, -1,  1,  0,
          0, -1, -1, -1,  1, -1,  1,
          0,  0 };
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DZ =
        { 0,  0,  0,  1,  0,  0,  1,
         -1,  1, -1,  1,  1, -1, -1,
          0,  0,
          0,  0,  0, -1,  0,  0, -1,
          1, -1,  1, -1, -1,  1,  1,
          0,  0 };

        /// corresponding weights
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> W =
        { 8.0/27.0,                       // positive velocities
          2.0/27.0,  2.0/27.0,  2.0/27.0,
          1.0/54.0,  1.0/54.0,  1.0/54.0,
          1.0/54.0,  1.0/54.0,  1.0/54.0,
          1.0/216.0, 1.0/216.0,
          1.0/216.0, 1.0/216.0,
          0.0, 0.0,                       // padding
          8.0/27.0,                       // negative velocities
          2.0/27.0,  2.0/27.0,  2.0/27.0,
          1.0/54.0,  1.0/54.0,  1.0/54.0,
          1.0/54.0,  1.0/54.0,  1.0/54.0,
          1.0/216.0, 1.0/216.0,
          1.0/216.0, 1.0/216.0,
          0.0, 0.0 };

        /// logical mask for relevant populations
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> MASK =
        { 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
          0, 0,
          0, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
          0,  0 };

        /// lattice speed of sound
        static constexpr T CS = 1.0/cem::sqrt(3.0);
    };

  }
}

#endif // LBT_D3Q27
