#ifndef LATTICE_HPP_INCLUDED
#define LATTICE_HPP_INCLUDED


#include <memory>
#include <array>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <stdlib.h>
#include <iostream>

#include "memory_alignment.hpp"


/**\class    Lattice
 * \brief    Parent class for regular lattices
*/
template <typename T = double>
class Lattice
{
    public:

        /// physics
        T            const RE    = 0.0;                       //Reynolds number
        T            const RHO_0 = 1.0;                       //initial density
        T            const U     = 0.0;                       //characteristic velocity
        unsigned int const L     = 0.0;                       //characteristic length
        T            const NU    = U*static_cast<T>(L) / RE;  //kinematic viscosity

        unsigned int const NT    = 0; //temporal resolution
        unsigned int const NX    = 0; //spatial resolution
        unsigned int const NY    = 0;
        unsigned int const NZ    = 0;
        unsigned int const NPOP  = 1; //number of populations

        /// lattice characteristics
        unsigned int        const     DIM    =  3;
        static unsigned int constexpr SPEEDS = 27;
        static unsigned int constexpr HSPEED = 14;

        /// linear memory layout
        static unsigned int constexpr PAD = ((CACHE_LINE - sizeof(T)*SPEEDS % CACHE_LINE) % CACHE_LINE) / sizeof(T); //padding: number of size T
        static unsigned int constexpr ND  = SPEEDS + PAD;
        static unsigned int constexpr OFF = ND/2;
        size_t const MEM_SIZE = sizeof(T)*NZ*NY*NX*static_cast<size_t>(NPOP)*static_cast<size_t>(ND); //size of array in bytes


        /// weights
        alignas(CACHE_LINE) std::array<T, ND> const W = {{8.0/27.0,                         //positive velocities
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
                                                          0.0, 0.0}};

        /// velocities
        alignas(CACHE_LINE) std::array<T, ND> const DX = {{ 0,  1,  0,  0,  1,  1,  1,    //postive velocities
                                                            1,  0,  0,  1,  1,  1,  1,
                                                            0,  0,                        //padding
                                                            0, -1,  0,  0, -1, -1, -1,    //negative velocities
                                                           -1,  0,  0, -1, -1, -1, -1,
                                                            0,  0}};
        alignas(CACHE_LINE) std::array<T, ND> const DY = {{ 0,  0,  1,  0,  1, -1,  0,
                                                            0,  1,  1,  1, -1,  1, -1,
                                                            0,  0,
                                                            0,  0, -1,  0, -1,  1,  0,
                                                            0, -1, -1, -1,  1, -1,  1,
                                                            0,  0}};
        alignas(CACHE_LINE) std::array<T, ND> const DZ = {{ 0,  0,  0,  1,  0,  0,  1,
                                                           -1,  1, -1,  1,  1, -1, -1,
                                                            0,  0,
                                                            0,  0,  0, -1,  0,  0, -1,
                                                            1, -1,  1, -1, -1,  1,  1,
                                                            0,  0}};

        static T constexpr CS = 1.0/std::sqrt(3.0); //lattice speed of sound
        T const TAU   = NU/(CS*CS) + 1.0/ 2.0;       //laminar relaxation time
        T const OMEGA = 1.0/TAU;                    //collision frequency


        /// parallelism: 3D blocks
        //  each cell gets a block of cells instead of a single cell
        unsigned int const BLOCK_SIZE   = 32;                                   ///< loop block size
        unsigned int const NUM_BLOCKS_Z = std::ceil(static_cast<double>(NZ) / BLOCK_SIZE); ///< number of blocks in each dimension
        unsigned int const NUM_BLOCKS_Y = std::ceil(static_cast<double>(NY) / BLOCK_SIZE);
        unsigned int const NUM_BLOCKS_X = std::ceil(static_cast<double>(NX) / BLOCK_SIZE);
        unsigned int const NUM_BLOCKS   = NUM_BLOCKS_X*NUM_BLOCKS_Y*NUM_BLOCKS_Z;          ///< total number of blocks


        /// population
        T* F = nullptr; //pointer to population


        /// constructor
        Lattice(T const Reynolds, T const RHO, T const U_char, unsigned int const L_char,
                unsigned int const X, unsigned int const Y, unsigned int const Z,
                unsigned int const no_populations = 1):
            RE(Reynolds), RHO_0(RHO), U(U_char), L(L_char), NX(X), NY(Y), NZ(Z), NPOP(no_populations)
        {
            F = (T*) aligned_alloc(CACHE_LINE, MEM_SIZE);
            ExportParameters();

            if (F == nullptr)
            {
                std::cout << "ERROR" << std::endl;
            }
        }

        /// destructor
        ~Lattice()
        {
            std::cout << "See you, comrade!" << std::endl;
            free(F);
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
        inline T&       AA_Read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                unsigned int const n,       unsigned int const d,       unsigned int const p = 0);
        template <bool odd>
        inline T const& AA_Read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const;

        template <bool odd>
        inline T&       AA_Write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                 unsigned int const n,       unsigned int const d,       unsigned int const p = 0);
        template <bool odd>
        inline T const& AA_Write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                 unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const;

        /// AVX functions
        template <bool odd>
        inline void Load(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                         T (&f)[ND], unsigned int const p = 0) const;

        template <bool odd>
        inline void Store(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                          T const (&f)[ND], unsigned int const p = 0);

        /// import and export: population back-up
        void Import(char const * name);
        void Export(char const * name) const;
        void ExportParameters() const;
};


/// include related header files
#include "lattice_indexing.hpp"
#include "lattice_avx_indexing.hpp"
#include "lattice_backup.hpp"


#endif //LATTICE_HPP_INCLUDED
