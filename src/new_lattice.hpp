#ifndef LATTICE_HPP_INCLUDED
#define LATTICE_HPP_INCLUDED


#include <memory>
#include <array>

#include "constexpr_func.hpp"
#include "memory_alignment.hpp"

/// static classes require C++17

namespace lattice
{

    /**\class    Lattice
     * \brief    Prototype class for regular lattices: is not working as ND = 0, copy and paste for new lattices
    */
    template <typename T = double>
    class Lattice
    {
        public:
            /// lattice
            static constexpr unsigned int    DIM = 0;
            static constexpr unsigned int SPEEDS = 0;

            /// linear memory layout
            static constexpr unsigned int PAD = ((CACHE_LINE - sizeof(T)*SPEEDS % CACHE_LINE) % CACHE_LINE) / sizeof(T);
            static constexpr unsigned int  ND = SPEEDS + PAD;

            /// weights
            alignas(CACHE_LINE) static constexpr std::array<T, ND> W = {0.0};

            /// velocities
            alignas(CACHE_LINE) static constexpr std::array<T, ND> DX = {0.0};
            alignas(CACHE_LINE) static constexpr std::array<T, ND> DY = {0.0};
            alignas(CACHE_LINE) static constexpr std::array<T, ND> DZ = {0.0};

            /// lattice speed of sound
            static constexpr T CS = 0.0;
    };

    /**\class    defaultLattice
     * \brief    Dummy lattice: default parameter for merged grid
    */
    template <typename T = double>
    class defaultLattice
    {
        public:
            static constexpr unsigned int    DIM = 0;
            static constexpr unsigned int SPEEDS = 0;
            static constexpr unsigned int    PAD = 0;
            static constexpr unsigned int     ND = 0;
            alignas(CACHE_LINE) static constexpr std::array<T, 1>  W = {0.0};
            alignas(CACHE_LINE) static constexpr std::array<T, 1> DX = {0.0};
            alignas(CACHE_LINE) static constexpr std::array<T, 1> DY = {0.0};
            alignas(CACHE_LINE) static constexpr std::array<T, 1> DZ = {0.0};
            static constexpr T CS = 0.0;
    };

    /**\class    D3Q27
     * \brief    Class for D3Q27 lattice
    */
    template <typename T = double>
    class D3Q27
    {
        public:
            /// lattice
            static constexpr unsigned int DIM    =  3;
            static constexpr unsigned int SPEEDS = 27;

            /// linear memory layout
            static constexpr unsigned int PAD = ((CACHE_LINE - sizeof(T)*SPEEDS % CACHE_LINE) % CACHE_LINE) / sizeof(T); //padding: number of size T
            static constexpr unsigned int  ND = SPEEDS + PAD;


            /// weights
            alignas(CACHE_LINE) static constexpr std::array<T, ND> W =
            {{ 8.0/27.0,                         //positive velocities
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
               0.0, 0.0 }};

            /// velocities
            alignas(CACHE_LINE) static constexpr std::array<T, ND> DX =
            {{ 0,  1,  0,  0,  1,  1,  1,        //postive velocities
               1,  0,  0,  1,  1,  1,  1,
               0,  0,                            //padding
               0, -1,  0,  0, -1, -1, -1,        //negative velocities
              -1,  0,  0, -1, -1, -1, -1,
               0,  0 }};
            alignas(CACHE_LINE) static constexpr std::array<T, ND> DY =
            {{ 0,  0,  1,  0,  1, -1,  0,
               0,  1,  1,  1, -1,  1, -1,
               0,  0,
               0,  0, -1,  0, -1,  1,  0,
               0, -1, -1, -1,  1, -1,  1,
               0,  0 }};
            alignas(CACHE_LINE) static constexpr std::array<T, ND> DZ =
            {{ 0,  0,  0,  1,  0,  0,  1,
              -1,  1, -1,  1,  1, -1, -1,
               0,  0,
               0,  0,  0, -1,  0,  0, -1,
               1, -1,  1, -1, -1,  1,  1,
               0,  0 }};

            /// lattice speed of sound
            static constexpr T CS = 1.0/cef::sqrt(3.0);
    };

}

#endif //LATTICE_HPP_INCLUDED
