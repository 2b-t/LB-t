#ifndef LATTICE_HPP_INCLUDED
#define LATTICE_HPP_INCLUDED

/**
 * \file     lattice.hpp
 * \mainpage Default lattice class holding lattice discretisation parameters.
 *           Is used as default parameter for merged grids.
*/

#include <memory>
#include <array>

#include "../general/constexpr_func.hpp"
#include "../general/memory_alignment.hpp"

namespace lattice
{
    /**\class    lattice
     * \brief    Dummy lattice: default parameter for merged grid
     * \warning  Static classes with more complex members such as std::vector and std::array require C++17
    */
    template <typename T = double>
    class D0Q0
    {
        public:
            /// lattice discretisation parameters
            static constexpr unsigned int    DIM = 0;
            static constexpr unsigned int SPEEDS = 0;

            /// linear memory layout padding
            static constexpr unsigned int PAD = 0;
            static constexpr unsigned int  ND = 0;

            /// discrete velocities
            alignas(CACHE_LINE) static constexpr std::array<T, 1> DX = {0.0};
            alignas(CACHE_LINE) static constexpr std::array<T, 1> DY = {0.0};
            alignas(CACHE_LINE) static constexpr std::array<T, 1> DZ = {0.0};

            /// corresponding weights
            alignas(CACHE_LINE) static constexpr std::array<T, 1> W = {0.0};

            /// lattice speed of sound
            static constexpr T CS = 0.0;
    };
}

#endif //LATTICE_HPP_INCLUDED
