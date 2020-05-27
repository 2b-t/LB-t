#ifndef D3Q27_HPP_INCLUDED
#define D3Q27_HPP_INCLUDED

/**
 * \file     D3Q27.hpp
 * \mainpage Discretisation parameters for D3Q27-lattice
 * \warning  Static classes with more complex members such as std::vector and std::array require C++17
*/

#include <memory>
#include <array>

#include "../general/constexpr_func.hpp"
#include "../general/memory_alignment.hpp"

namespace lattice
{
    /**\class    lattice::D3Q27
     * \brief    Class for D3Q27 lattice
     * \note     "Lattice BGK models for Navier-Stokes equation"
     *           Y.H. Qian, D. D'Humières, P. Lallemand
     *           Europhysics Letters (EPL) Vol. 17 (1992)
     *           DOI: 10.1209/0295-5075/17/6/001
     *
     * \tparam   T   floating data type used for simulation
    */
    template <typename T = double>
    class D3Q27 final
    {
        public:
            /// lattice discretisation parameters
            static constexpr unsigned int    DIM =  3;
            static constexpr unsigned int SPEEDS = 27;
            static constexpr unsigned int HSPEED = (SPEEDS + 1)/2;

            /// linear memory layout padding
            static constexpr unsigned int PAD = ((CACHE_LINE - sizeof(T)*SPEEDS % CACHE_LINE) % CACHE_LINE) / sizeof(T);
            static constexpr unsigned int  ND = SPEEDS + PAD;
            static constexpr unsigned int OFF = ND/2;

            /// discrete velocities
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DX =
            { 0,  1,  0,  0,  1,  1,  1,        //postive velocities
              1,  0,  0,  1,  1,  1,  1,
              0,  0,                            //padding
              0, -1,  0,  0, -1, -1, -1,        //negative velocities
             -1,  0,  0, -1, -1, -1, -1,
              0,  0 };
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DY =
            { 0,  0,  1,  0,  1, -1,  0,
              0,  1,  1,  1, -1,  1, -1,
              0,  0,
              0,  0, -1,  0, -1,  1,  0,
              0, -1, -1, -1,  1, -1,  1,
              0,  0 };
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DZ =
            { 0,  0,  0,  1,  0,  0,  1,
             -1,  1, -1,  1,  1, -1, -1,
              0,  0,
              0,  0,  0, -1,  0,  0, -1,
              1, -1,  1, -1, -1,  1,  1,
              0,  0 };

            /// corresponding weights
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> W =
            { 8.0/27.0,                         //positive velocities
              2.0/27.0,  2.0/27.0,  2.0/27.0,
              1.0/54.0,  1.0/54.0,  1.0/54.0,
              1.0/54.0,  1.0/54.0,  1.0/54.0,
              1.0/216.0, 1.0/216.0,
              1.0/216.0, 1.0/216.0,
              0.0, 0.0,                         //padding
              8.0/27.0,                         //negative velocities
              2.0/27.0,  2.0/27.0,  2.0/27.0,
              1.0/54.0,  1.0/54.0,  1.0/54.0,
              1.0/54.0,  1.0/54.0,  1.0/54.0,
              1.0/216.0, 1.0/216.0,
              1.0/216.0, 1.0/216.0,
              0.0, 0.0 };

            /// logical mask for relevant populations
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> MASK =
            { 1, 1, 1, 1, 1, 1, 1,
              1, 1, 1, 1, 1, 1, 1,
              0, 0,
              0, 1, 1, 1, 1, 1, 1,
              1, 1, 1, 1, 1, 1, 1,
              0,  0 };

            /// lattice speed of sound
            static constexpr T CS = 1.0/cef::sqrt(3.0);
    };
}

#endif // D3Q27_HPP_INCLUDED
