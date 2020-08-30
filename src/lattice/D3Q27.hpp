#ifndef LBT_D3Q27
#define LBT_D3Q27

/**
 * \file     D3Q27.hpp
 * \mainpage Discretisation parameters for D3Q27-lattice
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
    /**\class    lattice::D3Q27P28
     * \brief    Class for D3Q27 lattice with padding to 28
     * \note     "Lattice BGK models for Navier-Stokes equation"
     *           Y.H. Qian, D. D'Humières, P. Lallemand
     *           Europhysics Letters (EPL) Vol. 17 (1992)
     *           DOI: 10.1209/0295-5075/17/6/001
     *
     * \tparam   T   Floating data type used for simulation
    */
    template <typename T = double>
    class D3Q27P28 final
    {
        public:
            /// lattice discretisation parameters
            static constexpr unsigned int    DIM =  3;
            static constexpr unsigned int SPEEDS = 27;
            static constexpr unsigned int HSPEED = (SPEEDS + 1)/2;

            /// linear memory layout padding
            static constexpr unsigned int PAD = 1;
            static constexpr unsigned int  ND = SPEEDS + PAD;
            static constexpr unsigned int OFF = ND/2;

            /// discrete velocities
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DX =
            { 0,  1,  0,  0,  1,  1,  1,        //positive velocities
              1,  0,  0,  1,  1,  1,  1,
              0, -1,  0,  0, -1, -1, -1,        //negative velocities
             -1,  0,  0, -1, -1, -1, -1 };
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DY =
            { 0,  0,  1,  0,  1, -1,  0,
              0,  1,  1,  1, -1,  1, -1,
              0,  0, -1,  0, -1,  1,  0,
              0, -1, -1, -1,  1, -1,  1 };
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DZ =
            { 0,  0,  0,  1,  0,  0,  1,
             -1,  1, -1,  1,  1, -1, -1,
              0,  0,  0, -1,  0,  0, -1,
              1, -1,  1, -1, -1,  1,  1 };

            /// corresponding weights
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> W =
            { 8.0/27.0,                         //positive velocities
              2.0/27.0,  2.0/27.0,  2.0/27.0,
              1.0/54.0,  1.0/54.0,  1.0/54.0,
              1.0/54.0,  1.0/54.0,  1.0/54.0,
              1.0/216.0, 1.0/216.0,
              1.0/216.0, 1.0/216.0,
              8.0/27.0,                         //negative velocities
              2.0/27.0,  2.0/27.0,  2.0/27.0,
              1.0/54.0,  1.0/54.0,  1.0/54.0,
              1.0/54.0,  1.0/54.0,  1.0/54.0,
              1.0/216.0, 1.0/216.0,
              1.0/216.0, 1.0/216.0 };

            /// logical mask for relevant populations
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> MASK =
            { 1, 1, 1, 1, 1, 1, 1,
              1, 1, 1, 1, 1, 1, 1,
              0, 1, 1, 1, 1, 1, 1,
              1, 1, 1, 1, 1, 1, 1 };

            /// lattice speed of sound
            static constexpr T CS = 1.0/cef::sqrt(3.0);
    };
    
    /// Alias default lattice
    template<typename T> using D3Q27 = D3Q27P28<T>;
    
    /**\class    lattice::D3Q27PC
     * \brief    Class for D3Q27 lattice with padding to match a full cache line
     * \note     "Lattice BGK models for Navier-Stokes equation"
     *           Y.H. Qian, D. D'Humières, P. Lallemand
     *           Europhysics Letters (EPL) Vol. 17 (1992)
     *           DOI: 10.1209/0295-5075/17/6/001
     *
     * \tparam   T   floating data type used for simulation
    */
    template <typename T = double>
    class D3Q27PC final
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
            { 0,  1,  0,  0,  1,  1,  1,        //positive velocities
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

#endif // LBT_D3Q27
