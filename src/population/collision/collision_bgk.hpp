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


template <bool odd, unsigned int NX, unsigned int NY, unsigned int NZ, class LT, class POP, typename T>
void collision_bgk(Continuum<NX,NY,NZ,T>& macro, POP& micro, bool const save = false, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(macro, micro) firstprivate(save, p) schedule(static,1)
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

                    /// load distributions
                    T f[micro.ND_] = {0.0};

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = 0; d < micro.HSPEED_; ++d)
                        {
                            f[n*micro.OFF_ + d] = micro. template AA_Read<odd>(x_n, y_n, z_n, n, d, p);
                        }
                    }
                    f[micro.OFF_] = 0.0;

                    /// macroscopic values
                    T rho = 0.0;
                    T u   = 0.0;
                    T v   = 0.0;
                    T w   = 0.0;
                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = 0; d < micro.HSPEED_; ++d)
                        {
                            unsigned int const curr = n*micro.OFF_ + d;
                            rho += f[curr];
                            u   += f[curr]*LT::DX[curr];
                            v   += f[curr]*LT::DY[curr];
                            w   += f[curr]*LT::DZ[curr];
                        }
                    }

                    if (save == true)
                    {
                        macro(x, y, z, 0) = rho;
                        macro(x, y, z, 1) = u;
                        macro(x, y, z, 2) = v;
                        macro(x, y, z, 3) = w;
                    }

                    T feq[micro.ND_] = {0.0};

                    T const uu = - 1.0/(2.0*LT::CS*LT::CS)*(u*u + v*v + w*w);

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = 0; d < micro.HSPEED_; ++d)
                        {
                            size_t const curr = n*micro.OFF_ + d;
                            T const cu = 1.0/(LT::CS*LT::CS)*(u*LT::DX[curr] + v*LT::DY[curr] + w*LT::DZ[curr]);
                            feq[curr] = LT::W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
                        }
                    }
                    f[micro.OFF_] = f[0];

                    /// collision and streaming
                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = 0; d < micro.HSPEED_; ++d)
                        {
                            size_t const curr = n*micro.OFF_ + d;
                            micro. template AA_Write<odd>(x_n,y_n,z_n,n,d,p) = f[curr] + micro.OMEGA_*(feq[curr] - f[curr]);
                        }
                    }
                }
            }
        }
    }
}

#endif //COLLISION_BGK_HPP_INCLUDED
