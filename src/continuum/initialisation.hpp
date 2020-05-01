#ifndef CONTINUUM_INITIALISATION_HPP_INCLUDED
#define CONTINUUM_INITIALISATION_HPP_INCLUDED

/**
 * \file     initialisation.hpp
 * \mainpage Initialisation of a continuum
*/

#if __has_include (<omp.h>)
    #include <omp.h>
#endif
#include <algorithm>
#include <cmath>

#include "continuum.hpp"
#include "../general/constexpr_func.hpp"

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void InitContinuum(Continuum<NX,NY,NZ,T>& macro, T const RHO_0, T const U_0, T const V_0, T const W_0)
{
    /// parallelism: 3D blocks
    //  each cell gets a block of cells instead of a single cell
    constexpr unsigned int   BLOCK_SIZE = 32;
    constexpr unsigned int NUM_BLOCKS_Z = cef::ceil(static_cast<double>(NZ) / BLOCK_SIZE);
    constexpr unsigned int NUM_BLOCKS_Y = cef::ceil(static_cast<double>(NY) / BLOCK_SIZE);
    constexpr unsigned int NUM_BLOCKS_X = cef::ceil(static_cast<double>(NX) / BLOCK_SIZE);
    constexpr unsigned int   NUM_BLOCKS = NUM_BLOCKS_X*NUM_BLOCKS_Y*NUM_BLOCKS_Z;

    #pragma omp parallel for default(none) shared(macro) firstprivate(RHO_0, U_0, V_0, W_0) schedule(static,1)
    for(unsigned int block = 0; block < NUM_BLOCKS; ++block)
    {
        unsigned int const z_start = BLOCK_SIZE * (block / (NUM_BLOCKS_X*NUM_BLOCKS_Y));
        unsigned int const   z_end = std::min(z_start + BLOCK_SIZE, NZ);

        for(unsigned int z = z_start; z < z_end; ++z)
        {
            unsigned int const y_start = BLOCK_SIZE*((block % (NUM_BLOCKS_X*NUM_BLOCKS_Y)) / NUM_BLOCKS_X);
            unsigned int const   y_end = std::min(y_start + BLOCK_SIZE, NY);

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                unsigned int const x_start = BLOCK_SIZE*(block % NUM_BLOCKS_X);
                unsigned int const   x_end = std::min(x_start + BLOCK_SIZE, NX);

                for(unsigned int x = x_start; x < x_end; ++x)
                {
                    macro(x, y, z, 0) = RHO_0;
                    macro(x, y, z, 1) = U_0;
                    macro(x, y, z, 2) = V_0;
                    macro(x, y, z, 3) = W_0;
                }
            }
        }
    }
}

#endif // CONTINUUM_INITIALISATION_HPP_INCLUDED
