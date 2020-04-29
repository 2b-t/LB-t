#ifndef BOUNDARY_BOUNCEBACK_HPP_INCLUDED
#define BOUNDARY_BOUNCEBACK_HPP_INCLUDED

#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "lattice.hpp"
#include "boundary.hpp"


template <bool odd>
void boundary_bounceback_halfway(std::vector<boundary_element<F_TYPE>> const& wall, Lattice<F_TYPE>& lattice, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(wall,lattice,p) schedule(static,32)
    for(size_t i = 0; i < wall.size(); ++i)
    {
        unsigned int x_n[3] = {0, 0, 0};
        unsigned int y_n[3] = {0, 0, 0};
        unsigned int z_n[3] = {0, 0, 0};

        x_n[0] = (lattice.NX + wall[i].x - 1) % lattice.NX;
        x_n[1] =               wall[i].x;
        x_n[2] = (             wall[i].x + 1) % lattice.NX;

        y_n[0] = (lattice.NY + wall[i].y - 1) % lattice.NY;
        y_n[1] =               wall[i].y;
        y_n[2] = (             wall[i].y + 1) % lattice.NY;

        z_n[0] = (lattice.NZ + wall[i].z - 1) % lattice.NZ;
        z_n[1] =               wall[i].z;
        z_n[2] = (             wall[i].z + 1) % lattice.NZ;

        for(unsigned int n = 0; n <= 1; ++n)
        {
            for(unsigned int d = 1; d < lattice.HSPEED; ++d)
            {
                lattice.AA_Write<odd>(x_n, y_n, z_n, !n, d, p) = lattice.AA_Read<!odd>(x_n, y_n, z_n, n, d, p);
            }
        }
    }
}

#endif //BOUNDARY_BOUNCEBACK_HPP_INCLUDED
