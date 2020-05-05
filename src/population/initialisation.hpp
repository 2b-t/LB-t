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


/**\fn         InitLattice
 * \brief      Initialise microscopic distributions from continuum values
 *
 * \tparam     odd   even (0, false) or odd (1, true) time step
 * \tparam     NX    simulation domain resolution in x-direction
 * \tparam     NY    simulation domain resolution in y-direction
 * \tparam     NZ    simulation domain resolution in z-direction
 * \tparam     LT    static lattice::DdQq class containing discretisation parameters
 * \tparam     T     floating data type used for simulation
 * \param[in]  con   continuum object holding macroscopic variables
 * \param[out] pop   population object holding microscopic variables
 * \param[in]  p     relevant population (default = 0)
*/
template <bool odd, unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
void InitLattice(Continuum<NX,NY,NZ,T> const& con, Population<NX,NY,NZ,LT>& pop, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(con, pop) firstprivate(p) schedule(static,1)
    for(unsigned int block = 0; block < pop.NUM_BLOCKS_; ++block)
    {
        unsigned int const z_start = pop.BLOCK_SIZE_ * (block / (pop.NUM_BLOCKS_X_*pop.NUM_BLOCKS_Y_));
        unsigned int const   z_end = std::min(z_start + pop.BLOCK_SIZE_, NZ);

        for(unsigned int z = z_start; z < z_end; ++z)
        {
            unsigned int const z_n[3] = { (NZ + z - 1) % NZ, z, (z + 1) % NZ };

            unsigned int const y_start = pop.BLOCK_SIZE_*((block % (pop.NUM_BLOCKS_X_*pop.NUM_BLOCKS_Y_)) / pop.NUM_BLOCKS_X_);
            unsigned int const   y_end = std::min(y_start + pop.BLOCK_SIZE_, NY);

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                unsigned int const y_n[3] = { (NY + y - 1) % NY, y, (y + 1) % NY };

                unsigned int const x_start = pop.BLOCK_SIZE_*(block % pop.NUM_BLOCKS_X_);
                unsigned int const   x_end = std::min(x_start + pop.BLOCK_SIZE_, NX);

                for(unsigned int x = x_start; x < x_end; ++x)
                {
                    unsigned int const x_n[3] = { (NX + x - 1) % NX, x, (x + 1) % NX };

                    T const rho = con(x, y, z, 0);
                    T const u   = con(x, y, z, 1);
                    T const v   = con(x, y, z, 2);
                    T const w   = con(x, y, z, 3);

                    T const uu = - 1.0/(2.0*LT::CS*LT::CS)*(u*u + v*v + w*w);

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = n; d < LT::OFF; ++d)
                        {
                            unsigned int const curr = n*LT::OFF + d;
                            T const cu = 1.0/(LT::CS*LT::CS)*(u*LT::DX[curr] + v*LT::DY[curr] + w*LT::DZ[curr]);
                            pop.F_[pop. template AA_IndexRead<odd>(x_n,y_n,z_n,n,d,p)] = LT::W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
                        }
                    }
                }
            }
        }
    }
}

#endif // POPULATION_INITIALISATION_HPP_INCLUDED
