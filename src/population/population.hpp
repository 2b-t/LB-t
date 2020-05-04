#ifndef POPULATION_HPP_INCLUDED
#define POPULATION_HPP_INCLUDED

/**
 * \file     population.hpp
 * \mainpage Class for microscopic populations
*/


#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string.h>

#include "../general/memory_alignment.hpp"
#include "../general/constexpr_func.hpp"


/**\class Population
 * \brief Class that holds macroscopic values
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP = 1>
class Population
{
    public:
        /// current lattice data type
        typedef typename std::remove_const<decltype(LT::CS)>::type T;

        /// lattice characteristics
        static constexpr unsigned int    DIM_ = LT::DIM;
        static constexpr unsigned int SPEEDS_ = LT::SPEEDS;
        static constexpr unsigned int HSPEED_ = LT::HSPEED;

        /// linear memory layout
        static constexpr unsigned int PAD_ = LT::PAD;
        static constexpr unsigned int  ND_ = LT::ND;
        static constexpr unsigned int OFF_ = LT::OFF;
        static constexpr size_t  MEM_SIZE_ = sizeof(T)*NZ*NY*NX*NPOP*static_cast<size_t>(ND_);

        /// parallelism: 3D blocks
        //  each cell gets a block of cells instead of a single cell
        static constexpr unsigned int   BLOCK_SIZE_ = 32;                                               ///< loop block size
        static constexpr unsigned int NUM_BLOCKS_Z_ = cef::ceil(static_cast<double>(NZ) / BLOCK_SIZE_); ///< number of blocks in each dimension
        static constexpr unsigned int NUM_BLOCKS_Y_ = cef::ceil(static_cast<double>(NY) / BLOCK_SIZE_);
        static constexpr unsigned int NUM_BLOCKS_X_ = cef::ceil(static_cast<double>(NX) / BLOCK_SIZE_);
        static constexpr unsigned int   NUM_BLOCKS_ = NUM_BLOCKS_X_*NUM_BLOCKS_Y_*NUM_BLOCKS_Z_;        ///< total number of blocks

        /// pointer to population
        T* const F_ = static_cast<T*>(aligned_alloc(CACHE_LINE, MEM_SIZE_));

        T const NU_;    //kinematic viscosity
        T const TAU_;   //laminar relaxation time
        T const OMEGA_; //collision frequency


        /// constructor
        Population(T const Re, T const U, unsigned int const L):
            NU_(U*static_cast<T>(L) / Re), TAU_(NU_/(LT::CS*LT::CS) + 1.0/ 2.0), OMEGA_(1.0/TAU_)
        {
            if (F_ == nullptr)
            {
                std::cerr << "Fatal error: Population could not be allocated." << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        /// destructor
        ~Population()
        {
            std::cout << "See you, comrade!" << std::endl;
            free(F_);
        }

        /// indexing functions
        inline size_t SpatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                      unsigned int const n, unsigned int const d, unsigned int const p = 0) const;
        void LinearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z,
                             unsigned int& p, unsigned int& n, unsigned int& d,
                             size_t const index) const;

        template <bool odd>
        inline size_t AA_IndexRead(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                   unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const;
        template <bool odd>
        inline size_t AA_IndexWrite(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                    unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const;

        template <bool odd>
        inline auto&       AA_Read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                   unsigned int const n,       unsigned int const d,       unsigned int const p = 0);
        template <bool odd>
        inline auto const& AA_Read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                   unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const;

        template <bool odd>
        inline auto&       AA_Write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                    unsigned int const n,       unsigned int const d,       unsigned int const p = 0);
        template <bool odd>
        inline auto const& AA_Write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                    unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const;

        /// import and export: population back-up
        void Import(std::string const name);
        void Export(std::string const name) const;
};

/// include related header files
#include "population_indexing.hpp"
#include "population_backup.hpp"

#endif // POPULATION_HPP_INCLUDED
