#ifndef POPULATION_INITIALISATION_HPP_INCLUDED
#define POPULATION_INITIALISATION_HPP_INCLUDED

/**
 * \file     initialisation.hpp
 * \mainpage Initialisation of a population from a continuum
*/

#include <algorithm>
#include <cmath>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "../continuum/continuum.hpp"
#include "population.hpp"


template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, class POP, typename T, bool odd = false>
void InitLattice(Continuum<NX,NY,NZ,T> const& macro, POP& micro, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(macro, micro) firstprivate(p) schedule(static,1)
    for(unsigned int block = 0; block < micro.NUM_BLOCKS_; ++block)
    {
        unsigned int x_n[3] = {0, 0, 0};
        unsigned int y_n[3] = {0, 0, 0};
        unsigned int z_n[3] = {0, 0, 0};

        unsigned int const z_start = micro.BLOCK_SIZE_ * (block / (micro.NUM_BLOCKS_X_*micro.NUM_BLOCKS_Y_));
        unsigned int const   z_end = std::min(z_start + micro.BLOCK_SIZE_, NZ);

        for(unsigned int z = z_start; z < z_end; ++z)
        {
            unsigned int const y_start = micro.BLOCK_SIZE_*((block % (micro.NUM_BLOCKS_X_*micro.NUM_BLOCKS_Y_)) / micro.NUM_BLOCKS_X_);
            unsigned int const   y_end = std::min(y_start + micro.BLOCK_SIZE_, NY);

            z_n[0] = (NZ + z - 1) % NZ;
            z_n[1] =       z;
            z_n[2] = (     z + 1) % NZ;

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                unsigned int const x_start = micro.BLOCK_SIZE_*(block % micro.NUM_BLOCKS_X_);
                unsigned int const   x_end = std::min(x_start + micro.BLOCK_SIZE_, NX);

                y_n[0] = (NY + y - 1) % NY;
                y_n[1] =       y;
                y_n[2] = (     y + 1) % NY;

                for(unsigned int x = x_start; x < x_end; ++x)
                {
                    x_n[0] = (NX + x - 1) % NX;
                    x_n[1] =       x;
                    x_n[2] = (     x + 1) % NX;

                    T const rho = macro(x, y, z, 0);
                    T const u   = macro(x, y, z, 1);
                    T const v   = macro(x, y, z, 2);
                    T const w   = macro(x, y, z, 3);

                    T const uu = - 1.0/(2.0*LT::CS*LT::CS)*(u*u + v*v + w*w);

                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        for(unsigned int d = 0; d < micro.OFF_; ++d)
                        {
                            size_t const curr = n*micro.OFF_ + d;
                            T const cu = 1.0/(LT::CS*LT::CS)*(u*LT::DX[curr] + v*LT::DY[curr] + w*LT::DZ[curr]);
                            micro. template AA_Read<odd>(x_n,y_n,z_n,n,d,p) = LT::W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
                        }
                    }
                }
            }
        }
    }
}

#endif // POPULATION_INITIALISATION_HPP_INCLUDED
