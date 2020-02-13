#ifndef COLLISION_BGK_HPP_INCLUDED
#define COLLISION_BGK_HPP_INCLUDED


#if __has_include (<omp.h>)
    #include <omp.h>
#endif
#include <algorithm>
#include <cmath>

#include "memory_alignment.hpp"

#include "continuum.hpp"
#include "lattice.hpp"


template <bool odd>
void collision_bgk(Continuum<F_TYPE>& macro, Lattice<F_TYPE>& lattice, bool const save = false, unsigned int const p = 0)
{
    //#pragma omp parallel for default(none) shared(macro, lattice) firstprivate(save, p) schedule(static,1)
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


                    /// load distributions
                    F_TYPE f[lattice.ND] = {0.0};

                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        for(unsigned int d = 0; d < lattice.HSPEED; ++d)
                        {
                            f[n*lattice.OFF + d] = lattice.AA_Read<odd>(x_n, y_n, z_n, n, d, p);
                        }
                    }
                    f[lattice.OFF] = 0.0;

                    /// macroscopic values
                    F_TYPE rho = 0.0;
                    F_TYPE u   = 0.0;
                    F_TYPE v   = 0.0;
                    F_TYPE w   = 0.0;
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        for(unsigned int d = 0; d < lattice.HSPEED; ++d)
                        {
                            unsigned int const curr = n*lattice.OFF + d;
                            rho += f[curr];
                            u   += f[curr]*lattice.DX[curr];
                            v   += f[curr]*lattice.DY[curr];
                            w   += f[curr]*lattice.DZ[curr];
                        }
                    }

                    if (save == true)
                    {
                        macro(x, y, z, 0) = rho;
                        macro(x, y, z, 1) = u;
                        macro(x, y, z, 2) = v;
                        macro(x, y, z, 3) = w;
                    }

                    F_TYPE feq[lattice.ND] = {0.0};

                    F_TYPE const uu = - 1.0/(2.0*lattice.CS*lattice.CS)*(u*u + v*v + w*w);

                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        for(unsigned int d = 0; d < lattice.HSPEED; ++d)
                        {
                            size_t const curr = n*lattice.OFF + d;
                            F_TYPE const cu = 1.0/(lattice.CS*lattice.CS)*(u*lattice.DX[curr] + v*lattice.DY[curr] + w*lattice.DZ[curr]);
                            feq[curr] = lattice.W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
                        }
                    }
                    f[lattice.OFF] = f[0];

                    /// collision and streaming
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        for(unsigned int d = 0; d < lattice.HSPEED; ++d)
                        {
                            size_t const curr = n*lattice.OFF + d;
                            lattice.AA_Write<odd>(x_n,y_n,z_n,n,d,p) = f[curr] + lattice.OMEGA*(feq[curr] - f[curr]);
                        }
                    }
                }
            }
        }
    }
}

#endif //COLLISION_BGK_HPP_INCLUDED
