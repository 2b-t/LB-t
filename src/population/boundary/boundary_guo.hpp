#ifndef BOUNDARY_GUO_HPP_INCLUDED
#define BOUNDARY_GUO_HPP_INCLUDED

/**
 * \file     boundary_guo.hpp
 * \mainpage Very robust Guo interpolation boundary condition for pressure and velocity
*/

#include <array>

#include "boundary.hpp"
#include "boundary_orientation.hpp"
#include "boundary_type.hpp"
#include "../population.hpp"


/**\fn      Guo
 * \brief   Interpolation velocity and pressure boundary conditions as proposed by Guo.
 *          Most stable boundary conditions in this implementation so far.
 * \note    "Non-equilibrium extrapolation method for velocity and pressure boundary conditions in the
 *          lattice Boltzmann method"
 *          Z.L. Guo, C.G. Zheng, B.C. Shi
 *          Chinese Physics, Volume 11, Number 4 (2002)
 *          DOI: 10.1088/1009-1963/11/4/310
 *
 * \tparam     odd           even (0, false) or odd (1, true) time step
 * \tparam     Type          type of the boundary condition (type::Pressure or type::Velocity)
 * \tparam     Orientation   boundary orientation (orientation::Left, orientation::Right)
 * \tparam     NX            simulation domain resolution in x-direction
 * \tparam     NY            simulation domain resolution in y-direction
 * \tparam     NZ            simulation domain resolution in z-direction
 * \tparam     LT            static lattice::DdQq class containing discretisation parameters
 * \tparam     T             floating data type used for simulation
 * \param[in]  boundary      vector holding all corresponding boundary condition elements
 * \param[out] pop           population object holding microscopic variables
 * \param[in]  p             relevant population (default = 0)
*/
template <bool odd, template <class Orientation> class Type, class Orientation, unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
void Guo(std::vector<boundaryElement<T>> const& boundary, Population<NX,NY,NZ,LT>& pop, unsigned int const p = 0)
{
    #pragma omp parallel for default(none) shared(boundary,pop,p) schedule(static,32)
    for(size_t i = 0; i < boundary.size(); ++i)
    {
        /// for neighbouring cell
        unsigned int const x_n[3] = { (NX + boundary[i].x + Orientation::x - 1) % NX,
                                            boundary[i].x + Orientation::x,
                                           (boundary[i].x + Orientation::x + 1) % NX };
        unsigned int const y_n[3] = { (NY + boundary[i].y + Orientation::y - 1) % NY,
                                            boundary[i].y + Orientation::y,
                                           (boundary[i].y + Orientation::y + 1) % NY };
        unsigned int const z_n[3] = { (NZ + boundary[i].z + Orientation::z - 1) % NZ,
                                            boundary[i].z + Orientation::z,
                                           (boundary[i].z + Orientation::z + 1) % NZ };

        // load distributions
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

        // macroscopic values
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

        // non-equilibrium part of distributions
        alignas(CACHE_LINE) T fneq[LT::ND] = {0.0};

        T uu = - 1.0/(2.0*LT::CS*LT::CS)*(u*u + v*v + w*w);

        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #pragma GCC unroll (16)
            for(unsigned int d = n; d < LT::HSPEED; ++d)
            {
                unsigned int const curr = n*LT::OFF + d;
                T const cu = 1.0/(LT::CS*LT::CS)*(u*LT::DX[curr] + v*LT::DY[curr] + w*LT::DZ[curr]);
                fneq[curr] = f[curr] - LT::W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
            }
        }

        /// write to current node
        // set new macroscopic values
        std::array<double,4> const bound  = {boundary[i].rho,
                                             boundary[i].u,
                                             boundary[i].v,
                                             boundary[i].w};
        std::array<double,4> const interp = {rho, u, v, w};
        std::array<double,4> res = Type<Orientation>::getMacroscopicValues(bound, interp);
        rho = res[0];
        u   = res[1];
        v   = res[2];
        w   = res[3];

        // equilibrium distributions
        alignas(CACHE_LINE) T feq[LT::ND] = {0.0};

        uu = - 1.0/(2.0*LT::CS*LT::CS)*(u*u + v*v + w*w);

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

        // write new population values to cell: feq + fneq
        unsigned int const x_c[3] = { (NX + boundary[i].x - 1) % NX, boundary[i].x, (boundary[i].x + 1) % NX };
        unsigned int const y_c[3] = { (NY + boundary[i].y - 1) % NY, boundary[i].y, (boundary[i].y + 1) % NY };
        unsigned int const z_c[3] = { (NZ + boundary[i].z - 1) % NZ, boundary[i].z, (boundary[i].z + 1) % NZ };

        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #pragma GCC unroll (16)
            for(unsigned int d = n; d < LT::HSPEED; ++d)
            {
                unsigned int const curr = n*LT::OFF + d;
                pop.F_[pop. template AA_IndexRead<odd>(x_c,y_c,z_c,n,d,p)] = feq[curr] + fneq[curr];
            }
        }
    }
}

#endif // BOUNDARY_GUO_HPP_INCLUDED
