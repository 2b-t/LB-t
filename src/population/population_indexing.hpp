#ifndef POPULATION_INDEXING_HPP_INCLUDED
#define POPULATION_INDEXING_HPP_INCLUDED

/**
 * \file     population_indexing.hpp
 * \brief    Class members for indexing of populations with A-A access pattern
 *
 * \mainpage The A-A access pattern avoids the usage of two distinct populations before and
 *           after streaming by treating even and odd time steps differently: Even time steps
 *           perform only a local collision step with a reverse read of the populations and a
 *           regular write while odd steps perform a combined streaming-collision-streaming
 *           step with a regular read and a reverse write.
 *           This is implemented by different macros that determine the population indices for
 *           even and odd time steps.
 *
 * \note     "Accelerating Lattice Boltzmann Fluid Flow Simulations Using Graphics Processors"
 *           P. Bailey, J. Myre, S.D.C. Walsh, D.J. Lilja, M.O. Saar
 *           38th International Conference on Parallel Processing (ICPP), Vienna, Austria (2009)
 *           DOI: 10.1109/ICPP.2009.38
*/


/**\fn        O_E (odd,a,b)
 * \brief     Macro for access in odd and even time steps
 *
 * \param[in] ts          boolean index for odd (1 = true) and even (0 = false) time steps
 * \param[in] oddIndex    index to be accessed at an odd time step
 * \param[in] evenIndex   index to be accessed at an even time step
*/
inline unsigned int __attribute__((always_inline)) O_E(timestep const ts, unsigned int const oddIndex, unsigned int const evenIndex)
{
    return ts == timestep::odd ? oddIndex : evenIndex;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP>
inline size_t __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::spatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                                                                           unsigned int const n, unsigned int const d, unsigned int const p) const
{
    return (((z*NY + y)*NX + x)*NPOP + p)*LT::ND + n*LT::OFF + d;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP>
void Population<NX,NY,NZ,LT,NPOP>::linearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z,
                                                   unsigned int& p, unsigned int& n, unsigned int& d,
                                                   size_t const index) const
{
    size_t factor = LT::ND*NPOP*NX*NY;
    size_t rest   = index%factor;

    z      = index/factor;

    factor = LT::ND*NPOP*NX;
    y      = rest/factor;
    rest   = rest%factor;

    factor = LT::ND*NPOP;
    x      = rest/factor;
    rest   = rest%factor;

    factor = LT::ND;
    p      = rest/factor;
    rest   = rest%factor;

    factor = LT::OFF;
    n      = rest/factor;
    rest   = rest%factor;

    factor = LT::SPEEDS;
    d      = rest%factor;

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <timestep AA>
inline size_t __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_IndexRead(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                        unsigned int const n,       unsigned int const d,       unsigned int const p) const
{
    return spatialToLinear(x[1 + O_E(AA, static_cast<int>(LT::DX[!n*OFF_+d]), 0)],
                           y[1 + O_E(AA, static_cast<int>(LT::DY[!n*OFF_+d]), 0)],
                           z[1 + O_E(AA, static_cast<int>(LT::DZ[!n*OFF_+d]), 0)],
                           O_E(AA, n, !n),
                           d, p);
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <timestep AA>
inline size_t __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_IndexWrite(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                         unsigned int const n,       unsigned int const d,       unsigned int const p) const
{
    return spatialToLinear(x[1 + O_E(AA, static_cast<int>(LT::DX[n*OFF_+d]), 0)],
                           y[1 + O_E(AA, static_cast<int>(LT::DY[n*OFF_+d]), 0)],
                           z[1 + O_E(AA, static_cast<int>(LT::DZ[n*OFF_+d]), 0)],
                           O_E(AA, !n, n),
                           d, p);
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <timestep AA>
inline auto& __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_Read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                  unsigned int const n,       unsigned int const d,       unsigned int const p)
{
    return F_[AA_IndexRead<AA>(x,y,z,n,d,p)];
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <timestep AA>
inline auto const& __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_Read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                        unsigned int const n,       unsigned int const d,       unsigned int const p) const
{
    return F_[AA_IndexRead<AA>(x,y,z,n,d,p)];
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <timestep AA>
inline auto& __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_Write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                   unsigned int const n,       unsigned int const d,       unsigned int const p)
{
    return F_[AA_IndexWrite<AA>(x,y,z,n,d,p)];
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <timestep AA>
inline auto const& __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_Write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                         unsigned int const n,       unsigned int const d,       unsigned int const p) const
{
    return F_[AA_IndexWrite<AA>(x,y,z,n,d,p)];
}

#endif // POPULATION_INDEXING_HPP_INCLUDED
