#ifndef LBT_LATTICE
#define LBT_LATTICE

/**
 * \file     lattice.hpp
 * \mainpage Prototype lattice class holding lattice discretisation parameters.
 *           Copy and modify for new models
 * \author   Tobit Flatscher (github.com/2b-t)
 *
 * \warning  Static classes with more complex members such as std::vector and std::array require C++17
*/

#include <array>
#include <cassert>
#include <memory>

#include "../general/constexpr_math.hpp"
#include "../general/memory_alignment.hpp"


namespace lattice
{
    template <typename T, size_t DIM, size_t SPEEDS, size_t PAD>
    class Lattice final
    {
        public:
            /// lattice discretisation parameters
            static constexpr unsigned int HSPEED = (SPEEDS + 1)/2;

            /// linear memory layout padding
            static constexpr unsigned int  ND = SPEEDS + PAD*2;
            static constexpr unsigned int OFF = ND/2;

            /// discrete velocities
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, ND> DX = cem::mergeArrays(DX_P_, PAD_, DX_M_, PAD_);
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, ND> DY = cem::mergeArrays(DY_P_, PAD_, DY_M_, PAD_);
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, ND> DZ = cem::mergeArrays(DZ_P_, PAD_, DZ_M_, PAD_);

            /// corresponding weights
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, ND> W = cem::mergeArrays(W_P_, PAD_, W_M_, PAD_);

            /// logical mask for relevant populations
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, ND> MASK = cem::mergeArrays(MASK_P_, PAD_, MASK_M_, PAD_);

            /// lattice speed of sound
            static constexpr T CS = 0.0;
            
        private:
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE)
            static constexpr std::array<T, PAD> PAD_ = {0.0};
        
            /// discretisation parameters for half the velocities without padding
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE)
            static constexpr std::array<T, HSPEED> DX_P_ = {0.0};
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE)
            static constexpr std::array<T, HSPEED> DX_N_ = {0.0};
            
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, HSPEED> DY_P_ = {0.0};
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, HSPEED> DY_N_ = {0.0};
            
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, HSPEED> DZ_P_ = {0.0};
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, HSPEED> DZ_N_ = {0.0};
            
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, HSPEED> W_P_ = {0.0};
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, HSPEED> W_N_ = {0.0};
            
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, HSPEED> MASK_P_ = {0.0};
            __attribute__((aligned(CACHE_LINE))) alignas(CACHE_LINE) 
            static constexpr std::array<T, HSPEED> MASK_N_ = {0.0};
            
    };
}

#endif // LBT_LATTICE
