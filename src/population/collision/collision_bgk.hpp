#ifndef COLLISION_BGK_HPP_INCLUDED
#define COLLISION_BGK_HPP_INCLUDED

/**
 * \file     collision_bgk.hpp
 * \mainpage BGK collision operator
*/

#include <algorithm>
#include <cmath>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "../../general/memory_alignment.hpp"
#include "../../continuum/continuum.hpp"
#include "../population.hpp"

/**\fn            CollideStreamBGK
 * \brief         BGK collision operator for arbitrary lattice
 * \note          "A Model for Collision Processes in Gases. I. Small Amplitude Processes in Charged
 *                and Neutral One-Component Systems"
 *                P.L. Bhatnagar, E.P. Gross, M. Krook
 *                Physical Review 94 (1954)
 *                DOI: 10.1103/PhysRev.94.511
 *
 * \tparam        odd    even (0, false) or odd (1, true) time step
 * \tparam        NX     simulation domain resolution in x-direction
 * \tparam        NY     simulation domain resolution in y-direction
 * \tparam        NZ     simulation domain resolution in z-direction
 * \tparam        LT     static lattice::DdQq class containing discretisation parameters
 * \tparam        T      floating data type used for simulation
 * \param[out]    con    continuum object holding macroscopic variables
 * \param[in,out] pop    population object holding microscopic variables
 * \param[in]     save   save current macroscopic values to disk (Boolean true/false)
 * \param[in]     p      relevant population (default = 0)
*/
template <bool odd, unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
void CollideStreamBGK(Continuum<NX,NY,NZ,T>& con, Population<NX,NY,NZ,LT>& pop, bool const save = false, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(con, pop) firstprivate(save,p) schedule(static,1)
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

                    /// load distributions
                    alignas(CACHE_LINE) T f[LT::ND] = {0.0};

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = n; d < LT::HSPEED; ++d)
                        {
                            f[n*LT::OFF + d] = pop.F_[pop. template AA_IndexRead<odd>(x_n,y_n,z_n,n,d,p)];
                        }
                    }

                    /// macroscopic values
                    T rho = 0.0;
                    T u   = 0.0;
                    T v   = 0.0;
                    T w   = 0.0;
                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = n; d < LT::HSPEED; ++d)
                        {
                            unsigned int const curr = n*LT::OFF + d;
                            rho += f[curr];
                            u   += f[curr]*LT::DX[curr];
                            v   += f[curr]*LT::DY[curr];
                            w   += f[curr]*LT::DZ[curr];
                        }
                    }
                    u /= rho;
                    v /= rho;
                    w /= rho;

                    if (save == true)
                    {
                        con(x, y, z, 0) = rho;
                        con(x, y, z, 1) = u;
                        con(x, y, z, 2) = v;
                        con(x, y, z, 3) = w;
                    }

                    /// equilibrium distributions
                    alignas(CACHE_LINE) T feq[LT::ND] = {0.0};

                    T const uu = - 1.0/(2.0*LT::CS*LT::CS)*(u*u + v*v + w*w);

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = n; d < LT::HSPEED; ++d)
                        {
                            unsigned int const curr = n*LT::OFF + d;
                            T const cu = 1.0/(LT::CS*LT::CS)*(u*LT::DX[curr] + v*LT::DY[curr] + w*LT::DZ[curr]);
                            feq[curr] = LT::W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
                        }
                    }

                    /// collision and streaming
                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = n; d < LT::HSPEED; ++d)
                        {
                            unsigned int const curr = n*LT::OFF + d;
                            pop.F_[pop. template AA_IndexWrite<odd>(x_n,y_n,z_n,n,d,p)] = f[curr] + pop.OMEGA_*(feq[curr] - f[curr]);
                        }
                    }
                }
            }
        }
    }
}

#endif //COLLISION_BGK_HPP_INCLUDED
