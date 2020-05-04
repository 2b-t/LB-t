#ifndef LATTICE_HPP_INCLUDED
#define LATTICE_HPP_INCLUDED

/**
 * \file     prototype.hpp
 * \mainpage Prototype lattice class holding lattice discretisation parameters.
 *           Copy and modify for new models
*/

#include <memory>
#include <array>

#include "../general/constexpr_func.hpp"
#include "../general/memory_alignment.hpp"

namespace lattice
{
    /**\class    Lattice
     * \brief    Prototype class for regular lattices: is not working as ND = 0, copy and paste for new lattices
     * \warning  Static classes with more complex members such as std::vector and std::array require C++17
    */
    template <typename T = double>
    class Lattice
    {
        public:
            /// lattice discretisation parameters
            static constexpr unsigned int    DIM = 0;
            static constexpr unsigned int SPEEDS = 0;
            static constexpr unsigned int HSPEED = (SPEEDS + 1)/2;

            /// linear memory layout padding
            static constexpr unsigned int PAD = ((CACHE_LINE - sizeof(T)*SPEEDS % CACHE_LINE) % CACHE_LINE) / sizeof(T);
            static constexpr unsigned int  ND = SPEEDS + PAD;
            static constexpr unsigned int OFF = ND/2;

            /// discrete velocities
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DX = {0.0};
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DY = {0.0};
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> DZ = {0.0};

            /// corresponding weights
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> W = {0.0};
            
            /// logical mask
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) static constexpr std::array<T, ND> MASK = {0.0}

            /// lattice speed of sound
            static constexpr T CS = 0.0;
    };
}

#endif //LATTICE_HPP_INCLUDED
