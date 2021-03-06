#ifndef LBT_D3Q15
#define LBT_D3Q15

/**
 * \file     D3Q15.hpp
 * \mainpage Discretisation parameters for D3Q15-lattice
 * \author   Tobit Flatscher (github.com/2b-t)
 *
 * \warning  Static classes with more complex members such as std::vector and std::array require C++17
*/

#include <array>
#include <memory>

#include "../general/constexpr_math.hpp"
#include "../general/memory_alignment.hpp"


namespace lattice
{
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
    template <typename T = double>
    class D3Q15P16 final
    {
        public:
            /// lattice discretisation parameters
            static constexpr unsigned int    DIM =  3;
            static constexpr unsigned int SPEEDS = 15;
            static constexpr unsigned int HSPEED = (SPEEDS + 1)/2;

            /// linear memory layout padding
            static constexpr unsigned int PAD = 1;
            static constexpr unsigned int  ND = SPEEDS + PAD;
            static constexpr unsigned int OFF = ND/2;

            /// discrete velocities
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DX =
            { 0,  1,  0,  0,  1,  1,  1,  1,   // positive velocities
              0, -1,  0,  0, -1, -1, -1, -1 }; // negative velocities
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DY =
            { 0,  0,  1,  0,  1, -1,  1, -1,
              0,  0, -1,  0, -1,  1, -1,  1 };
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DZ =
            { 0,  0,  0,  1,  1,  1, -1, -1,
              0,  0,  0, -1, -1, -1,  1,  1 };

            /// corresponding weights
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> W =
            { 2.0/9.0,                                  // positive velocities
              1.0/9.0, 1.0/9.0, 1.0/9.0,
              1.0/72.0, 1.0/72.0, 1.0/72.0, 1.0/72.0,
              2.0/9.0,                                  // negative velocities
              1.0/9.0, 1.0/9.0, 1.0/9.0,
              1.0/72.0, 1.0/72.0, 1.0/72.0, 1.0/72.0 };

            /// logical mask for relevant populations
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> MASK =
            { 1, 1, 1, 1, 1, 1, 1, 1, 
              0, 1, 1, 1, 1, 1, 1, 1 };

            /// lattice speed of sound
            static constexpr T CS = 1.0/cem::sqrt(3.0);
    };

    /// Alias default lattice
    template<typename T> using D3Q15 = D3Q15P16<T>;
}

#endif // LBT_D3Q15
