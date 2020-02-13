#ifndef INITIALISATION_HPP_INCLUDED
#define INITIALISATION_HPP_INCLUDED


#if __has_include (<omp.h>)
    #include <omp.h>
#endif
#include <algorithm>
#include <cmath>

#include "continuum.hpp"
#include "lattice.hpp"


template <typename T = double>
void init_continuum(Continuum<T>& macro, Lattice<T> const& lattice,
                    T const RHO_0, T const U_0, T const V_0, T const W_0)
{
    #pragma omp parallel for default(none) shared(macro, lattice) firstprivate(RHO_0, U_0, V_0, W_0) schedule(static,1)
    for(unsigned int block = 0; block < lattice.NUM_BLOCKS; ++block)
    {
        unsigned int const z_start = lattice.BLOCK_SIZE * (block / (lattice.NUM_BLOCKS_X*lattice.NUM_BLOCKS_Y));
        unsigned int const   z_end = std::min(z_start + lattice.BLOCK_SIZE, lattice.NZ);

        for(unsigned int z = z_start; z < z_end; ++z)
        {
            unsigned int const y_start = lattice.BLOCK_SIZE*((block % (lattice.NUM_BLOCKS_X*lattice.NUM_BLOCKS_Y)) / lattice.NUM_BLOCKS_X);
            unsigned int const   y_end = std::min(y_start + lattice.BLOCK_SIZE, lattice.NY);

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                unsigned int const x_start = lattice.BLOCK_SIZE*(block % lattice.NUM_BLOCKS_X);
                unsigned int const   x_end = std::min(x_start + lattice.BLOCK_SIZE, lattice.NX);

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


template <typename T = double, bool odd = false>
void init_lattice(Continuum<T> const& macro, Lattice<F_TYPE>& lattice, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(macro, lattice) firstprivate(p) schedule(static,1)
    for(unsigned int block = 0; block < lattice.NUM_BLOCKS; ++block)
    {
        unsigned int x_n[3] = {0, 0, 0};
        unsigned int y_n[3] = {0, 0, 0};
        unsigned int z_n[3] = {0, 0, 0};

        unsigned int const z_start = lattice.BLOCK_SIZE * (block / (lattice.NUM_BLOCKS_X*lattice.NUM_BLOCKS_Y));
        unsigned int const   z_end = std::min(z_start + lattice.BLOCK_SIZE, lattice.NZ);

        for(unsigned int z = z_start; z < z_end; ++z)
        {
            unsigned int const y_start = lattice.BLOCK_SIZE*((block % (lattice.NUM_BLOCKS_X*lattice.NUM_BLOCKS_Y)) / lattice.NUM_BLOCKS_X);
            unsigned int const   y_end = std::min(y_start + lattice.BLOCK_SIZE, lattice.NY);

            z_n[0] = (lattice.NZ + z - 1) % lattice.NZ;
            z_n[1] =               z;
            z_n[2] = (             z + 1) % lattice.NZ;

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                unsigned int const x_start = lattice.BLOCK_SIZE*(block % lattice.NUM_BLOCKS_X);
                unsigned int const   x_end = std::min(x_start + lattice.BLOCK_SIZE, lattice.NX);

                y_n[0] = (lattice.NY + y - 1) % lattice.NY;
                y_n[1] =               y;
                y_n[2] = (             y + 1) % lattice.NY;

                for(unsigned int x = x_start; x < x_end; ++x)
                {
                    x_n[0] = (lattice.NX + x - 1) % lattice.NX;
                    x_n[1] =               x;
                    x_n[2] = (             x + 1) % lattice.NX;

                    T const rho = macro(x, y, z, 0);
                    T const u   = macro(x, y, z, 1);
                    T const v   = macro(x, y, z, 2);
                    T const w   = macro(x, y, z, 3);

                    F_TYPE const uu = - 1.0/(2.0*lattice.CS*lattice.CS)*(u*u + v*v + w*w);

                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        for(unsigned int d = 0; d < lattice.OFF; ++d)
                        {
                            size_t const curr = n*lattice.OFF + d;
                            F_TYPE const cu = 1.0/(lattice.CS*lattice.CS)*(u*lattice.DX[curr] + v*lattice.DY[curr] + w*lattice.DZ[curr]);
                            lattice.AA_Read<odd>(x_n,y_n,z_n,n,d,p) = lattice.W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
                        }
                    }
                }
            }
        }
    }
}

#endif //INITIALISATION_HPP_INCLUDED
