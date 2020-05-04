#ifndef COLLISION_TRT_HPP_INCLUDED
#define COLLISION_TRT_HPP_INCLUDED

/**
 * \file     collision_trt.hpp
 * \mainpage TRT collision operator
 *
 * \note     "Two-relaxation-time Lattice Boltzmann scheme: about parametrization, velocity,
 *           pressure and mixed boundary conditions"
 *           I. Ginzburg, F. Verhaeghe, D. Humiéres
 *           Communications in Computational Physics Vol. 3 (2008)
 *           Online: http://global-sci.org/intro/article_detail/cicp/7862.html
*/

#include <algorithm>
#include <cmath>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include "../../general/memory_alignment.hpp"
#include "../../continuum/continuum.hpp"
#include "../population.hpp"

/**\fn            CollideStreamTRT
 * \brief         TRT collision operator for arbitrary lattice
 *
 * \param[out]    macro   continuum object holding macroscopic variables
 * \param[in,out] micro   population object holding microscopic variables
 * \param[in]     save    save current macroscopic values to disk (Boolean true/false)
 * \param[in]     p       relevant population (default 0)
*/
template <bool odd, unsigned int NX, unsigned int NY, unsigned int NZ, class LT, class POP, typename T>
void CollideStreamTRT(Continuum<NX,NY,NZ,T>& macro, POP& micro, bool const save = false, unsigned int const p = 0)
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
            z_n[0] = (NZ + z - 1) % NZ;
            z_n[1] =       z;
            z_n[2] = (     z + 1) % NZ;

            unsigned int const y_start = micro.BLOCK_SIZE_*((block % (micro.NUM_BLOCKS_X_*micro.NUM_BLOCKS_Y_)) / micro.NUM_BLOCKS_X_);
            unsigned int const   y_end = std::min(y_start + micro.BLOCK_SIZE_, NY);

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                y_n[0] = (NY + y - 1) % NY;
                y_n[1] =       y;
                y_n[2] = (     y + 1) % NY;

                unsigned int const x_start = micro.BLOCK_SIZE_*(block % micro.NUM_BLOCKS_X_);
                unsigned int const   x_end = std::min(x_start + micro.BLOCK_SIZE_, NX);

                for(unsigned int x = x_start; x < x_end; ++x)
                {
                    x_n[0] = (NX + x - 1) % NX;
                    x_n[1] =       x;
                    x_n[2] = (     x + 1) % NX;

                    /// load distributions
                    alignas(CACHE_LINE) T f[LT::ND] = {0.0};

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = n; d < LT::HSPEED; ++d)
                        {
                            f[n*LT::OFF + d] = micro.F_[micro. template AA_IndexRead<odd>(x_n,y_n,z_n,n,d,p)];
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
                        macro(x, y, z, 0) = rho;
                        macro(x, y, z, 1) = u;
                        macro(x, y, z, 2) = v;
                        macro(x, y, z, 3) = w;
                    }

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

                    alignas(CACHE_LINE) T fp[LT::OFF] = {0.0};
                    alignas(CACHE_LINE) T fm[LT::OFF] = {0.0};

                    #pragma GCC unroll (15)
                    for(unsigned int d = 1; d < LT::HSPEED; ++d)
                    {
                        fp[d] = 0.5*(f[d] + f[LT::OFF + d] - (feq[d] + feq[LT::OFF + d]));
                        fm[d] = 0.5*(f[d] - f[LT::OFF + d] - (feq[d] - feq[LT::OFF + d]));
                    }

                    /// collision and streaming
                    micro.F_[micro. template AA_IndexWrite<odd>(x_n,y_n,z_n,0,0,p)] = f[0] + micro.OMEGA_*(feq[0] - f[0]);
                    #pragma GCC unroll (15)
                    for(unsigned int d = 1; d < LT::HSPEED; ++d)
                    {
                        micro.F_[micro. template AA_IndexWrite<odd>(x_n,y_n,z_n,0,d,p)] = f[d] - micro.OMEGA_*fp[d] - micro.OMEGA_M_*fm[d];
                    }
                    #pragma GCC unroll (15)
                    for(unsigned int d = 1; d < LT::HSPEED; ++d)
                    {
                        micro.F_[micro. template AA_IndexWrite<odd>(x_n,y_n,z_n,1,d,p)] = f[LT::OFF + d] - micro.OMEGA_*fp[d] + micro.OMEGA_M_*fm[d];
                    }
                }
            }
        }
    }
}

#endif //COLLISION_TRT_HPP_INCLUDED
