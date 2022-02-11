/**
 * \file     D2Q9.hpp
 * \mainpage Discretisation parameters for D2Q9-lattice
 * \author   Tobit Flatscher (github.com/2b-t)
 *
 * \warning  Static classes with more complex members such as std::vector and std::array require C++17
*/

#ifndef LBT_D2Q9
#define LBT_D2Q9
#pragma once

#include <cstdint>
#include <type_traits>

#include "../constexpr_math/constexpr_math.hpp"
#include "../general/type_definitions.hpp"


namespace lbt {
  namespace lattice {

    /**\class  lattice::D2Q9P10
     * \brief  Class for D2Q9 lattice with padding to 10
     *
     * \note   "Lattice BGK models for Navier-Stokes equation"
     *         Y.H. Qian, D. D'Humières, P. Lallemand
     *         Europhysics Letters (EPL) Vol. 17 (1992)
     *         DOI: 10.1209/0295-5075/17/6/001
     *
     * \tparam T   Floating data type used for simulation
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    class D2Q9P10 final {
      public:
        using type = T;

        /// lattice discretisation parameters
        static constexpr std::int32_t    DIM =  2;
        static constexpr std::int32_t SPEEDS =  9;
        static constexpr std::int32_t HSPEED = (SPEEDS + 1)/2;

        /// linear memory layout padding
        static constexpr std::int32_t PAD = 1;
        static constexpr std::int32_t  ND = SPEEDS + PAD;
        static constexpr std::int32_t OFF = ND/2;

        /// discrete velocities
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DX =
        { 0,  1,  0,  1, -1,   // positive velocities
          0, -1,  0, -1,  1 }; // negative velocities
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DY =
        { 0,  0,  1,  1,  1,
          0,  0, -1, -1, -1 };
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DZ =
        { 0,  0,  0,  0,  0,
          0,  0,  0,  0,  0 };

        /// corresponding weights
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> W =
        { 4.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/36.0, 1.0/36.0,
          4.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/36.0, 1.0/36.0 };

        /// logical mask for relevant populations
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> MASK =
        { 1, 1, 1, 1, 1,
          0, 1, 1, 1, 1 };

        /// lattice speed of sound
        static constexpr T CS = 1.0/cem::sqrt(3.0);
    };

    /// Alias default lattice
    template<typename T> using D2Q9 = D2Q9P10<T>;


    /**\class  lattice::D2Q9P12
     * \brief  Class for D2Q9 lattice with padding to 12
     *
     * \note   "Lattice BGK models for Navier-Stokes equation"
     *         Y.H. Qian, D. D'Humières, P. Lallemand
     *         Europhysics Letters (EPL) Vol. 17 (1992)
     *         DOI: 10.1209/0295-5075/17/6/001
     *
     * \tparam T   Floating data type used for simulation
    */
    template <typename T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    class D2Q9P12 final {
      public:
        using type = T;

        /// lattice discretisation parameters
        static constexpr std::int32_t    DIM =  2;
        static constexpr std::int32_t SPEEDS =  9;
        static constexpr std::int32_t HSPEED = (SPEEDS + 1)/2;

        /// linear memory layout padding
        static constexpr std::int32_t PAD = 3;
        static constexpr std::int32_t  ND = SPEEDS + PAD;
        static constexpr std::int32_t OFF = ND/2;

        /// discrete velocities
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DX =
        { 0,  1,  0,  1, -1,  0,   // positive velocities
          0, -1,  0, -1,  1,  0 }; // negative velocities
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DY =
        { 0,  0,  1,  1,  1,  0, 
          0,  0, -1, -1, -1,  0 };
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> DZ =
        { 0,  0,  0,  0,  0,  0,
          0,  0,  0,  0,  0,  0 };

        /// corresponding weights
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> W =
        { 4.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/36.0, 1.0/36.0,  0.0, 
          4.0/9.0, 1.0/9.0, 1.0/9.0, 1.0/36.0, 1.0/36.0,  0.0 };

        /// logical mask for relevant populations
        LBT_ALIGN static constexpr lbt::StackArray<T, ND> MASK =
        { 1, 1, 1, 1, 1, 0,
          0, 1, 1, 1, 1, 0 };

        /// lattice speed of sound
        static constexpr T CS = 1.0/cem::sqrt(3.0);
    };

  }
}

#endif // LBT_D2Q9
