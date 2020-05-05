#ifndef BOUNDARY_BOUNCEBACK_HPP_INCLUDED
#define BOUNDARY_BOUNCEBACK_HPP_INCLUDED

/**
 * \file     boundary_bounceback.hpp
 * \mainpage Solid walls with simple bounce-back boundaries
*/

#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "boundary.hpp"
#include "../population.hpp"


/**\fn         BounceBackHalfway
 * \brief      Solid wall boundary treatment with simple halfway bounce-back
 * \note       "Analytic solutions of simple flows and analysis of non-slip boundary conditions for
 *             the lattice Boltzmann BGK model"
 *             X. He, Q. Zou, L.S. Luo, M. Dembo
 *             Journal of Statistical Physics 87 (1997)
 *             DOI: 10.1007/BF02181482
 *
 * \tparam     odd    even (0, false) or odd (1, true) time step
 * \tparam     NX     simulation domain resolution in x-direction
 * \tparam     NY     simulation domain resolution in y-direction
 * \tparam     NZ     simulation domain resolution in z-direction
 * \tparam     LT     static lattice::DdQq class containing discretisation parameters
 * \tparam     T      floating data type used for simulation
 * \param[in]  wall   vector holding all corresponding boundary condition elements
 * \param[out] pop    population object holding microscopic variables
 * \param[in]  p      relevant population (default = 0)
*/
template <bool odd, unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
void BounceBackHalfway(std::vector<boundaryElement<T>> const& wall, Population<NX,NY,NZ,LT>& pop, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(wall,pop,p) schedule(static,32)
    for(size_t i = 0; i < wall.size(); ++i)
    {
        unsigned int const x_n[3] = { (NX + wall[i].x - 1) % NX, wall[i].x, (wall[i].x + 1) % NX };
        unsigned int const y_n[3] = { (NY + wall[i].y - 1) % NY, wall[i].y, (wall[i].y + 1) % NY };
        unsigned int const z_n[3] = { (NZ + wall[i].z - 1) % NZ, wall[i].z, (wall[i].z + 1) % NZ };

        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #pragma GCC unroll (15)
            for(unsigned int d = 1; d < LT::HSPEED; ++d)
            {
                pop.F_[pop. template AA_IndexWrite<odd>(x_n, y_n, z_n, !n, d, p)] = pop.F_[pop. template AA_IndexRead<!odd>(x_n, y_n, z_n, n, d, p)];
            }
        }
    }
}

#endif // BOUNDARY_BOUNCEBACK_HPP_INCLUDED
