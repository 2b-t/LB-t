#ifndef BOUNDARY_BOUNCEBACK_HPP_INCLUDED
#define BOUNDARY_BOUNCEBACK_HPP_INCLUDED

/**
 * \file     boundary_bounceback.hpp
 * \mainpage Solid walls with simple bounce-back boundaries
 * 
 * \note     "Analytic solutions of simple flows and analysis of non-slip boundary conditions for
 *           the lattice Boltzmann BGK model"
 *           X. He, Q. Zou, L.S. Luo, M. Dembo
 *           Journal of Statistical Physics 87 (1997)
 *           DOI: 10.1007/BF02181482
*/

#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "boundary.hpp"


/**\fn         BounceBackHalfway
 * \brief      Solid wall boundary treatment with simple halfway bounce-back
 *
 * \param[in]  wall    vector holding all corresponding boundary condition elements
 * \param[out] micro   population object holding microscopic variables
 * \param[in]  p       relevant population (default 0)
*/
template <bool odd, unsigned int NX, unsigned int NY, unsigned int NZ, class POP, typename T>
void BounceBackHalfway(std::vector<boundaryElement<T>> const& wall, POP& micro, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(wall,micro,p) schedule(static,32)
    for(size_t i = 0; i < wall.size(); ++i)
    {
        unsigned int x_n[3] = {0, 0, 0};
        unsigned int y_n[3] = {0, 0, 0};
        unsigned int z_n[3] = {0, 0, 0};

        x_n[0] = (NX + wall[i].x - 1) % NX;
        x_n[1] =       wall[i].x;
        x_n[2] = (     wall[i].x + 1) % NX;

        y_n[0] = (NY + wall[i].y - 1) % NY;
        y_n[1] =       wall[i].y;
        y_n[2] = (     wall[i].y + 1) % NY;

        z_n[0] = (NZ + wall[i].z - 1) % NZ;
        z_n[1] =       wall[i].z;
        z_n[2] = (     wall[i].z + 1) % NZ;

        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #pragma GCC unroll (16)
            for(unsigned int d = 1; d < micro.HSPEED_; ++d)
            {
                micro. template AA_Write<odd>(x_n, y_n, z_n, !n, d, p) = micro. template AA_Read<!odd>(x_n, y_n, z_n, n, d, p);
            }
        }
    }
}

#endif // BOUNDARY_BOUNCEBACK_HPP_INCLUDED
