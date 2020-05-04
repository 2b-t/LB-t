#ifndef POPULATION_INDEXING_HPP_INCLUDED
#define POPULATION_INDEXING_HPP_INCLUDED

/**
 * \file     population_indexing.hpp
 * \mainpage Class members for indexing population
*/


/**\def       O_E (odd,a,b)
 * \brief     Macro for access in odd and even time steps
 *
 * \param[in] odd   boolean index for odd (1 = true) and even (0 = false) time steps
 * \param[in] a     index to be accessed at an odd time step
 * \param[in] b     index to be accessed at an even time step
*/
#define O_E(odd,a,b)    a*static_cast<decltype(a)>(odd) + b*static_cast<decltype(b)>(!odd)


/**\fn         SpatialToLinear
 * \brief      Inline function for field indexing for positive and negative indices seperately
 * \warning    Inline function! Has to be declared in header!
 *
 * \param[in]  x   x coordinate of cell
 * \param[in]  y   y coordinate of cell
 * \param[in]  z   z coordinate of cell
 * \param[in]  n   positive (0) or negative (1) index
 * \param[in]  d   relevant population index
 * \param[in]  p   relevant population (default 0)
 * \return     requested linear population index
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP>
inline size_t __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::SpatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                                                                           unsigned int const n, unsigned int const d, unsigned int const p) const
{
    return (((z*NY + y)*NX + x)*NPOP + p)*ND_ + n*OFF_ + d;
}


/**\fn         LinearToSpatial
 * \brief      Generate population coordinates from scalar index
 *
 * \param[out] x       return value x coordinate of cell
 * \param[out] y       return value y coordinate of cell
 * \param[out] z       return value z coordinate of cell
 * \param[out] p       return value number of population
 * \param[out] n       return positive (0) or negative (1) index
 * \param[out] d       return value number of relevant population index
 * \param[in]  index   current linear population index
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP>
void Population<NX,NY,NZ,LT,NPOP>::LinearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z,
                                                   unsigned int& p, unsigned int& n, unsigned int& d,
                                                   size_t const index) const
{
    size_t factor = ND_*NPOP*NX*NY;
    size_t rest   = index%factor;

    z      = index/factor;

    factor = ND_*NPOP*NX;
    y      = rest/factor;
    rest   = rest%factor;

    factor = ND_*NPOP;
    x      = rest/factor;
    rest   = rest%factor;

    factor = ND_;
    p      = rest/factor;
    rest   = rest%factor;

    factor = OFF_;
    n      = rest/factor;
    rest   = rest%factor;

    factor = SPEEDS_;
    d      = rest%factor;
}


/**
 * \fn       Read and Write
 * \brief    Addressing macros for A-A access pattern
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
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <bool odd>
inline size_t __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_IndexRead(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                        unsigned int const n,       unsigned int const d,       unsigned int const p) const
{
    return SpatialToLinear(x[1 + O_E(odd, static_cast<int>(LT::DX[!n*OFF_+d]), 0)],
                           y[1 + O_E(odd, static_cast<int>(LT::DY[!n*OFF_+d]), 0)],
                           z[1 + O_E(odd, static_cast<int>(LT::DZ[!n*OFF_+d]), 0)],
                           O_E(odd, n, !n),
                           d, p);
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <bool odd>
inline size_t __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_IndexWrite(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                         unsigned int const n,       unsigned int const d,       unsigned int const p) const
{
    return SpatialToLinear(x[1 + O_E(odd, static_cast<int>(LT::DX[n*OFF_+d]), 0)],
                           y[1 + O_E(odd, static_cast<int>(LT::DY[n*OFF_+d]), 0)],
                           z[1 + O_E(odd, static_cast<int>(LT::DZ[n*OFF_+d]), 0)],
                           O_E(odd, !n, n),
                           d, p);
}


template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <bool odd>
inline auto& __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_Read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                  unsigned int const n,       unsigned int const d,       unsigned int const p)
{
    return F_[AA_IndexRead<odd>(x,y,z,n,d,p)];
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <bool odd>
inline auto const& __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_Read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                        unsigned int const n,       unsigned int const d,       unsigned int const p) const
{
    return F_[AA_IndexRead<odd>(x,y,z,n,d,p)];
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <bool odd>
inline auto& __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_Write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                   unsigned int const n,       unsigned int const d,       unsigned int const p)
{
    return F_[AA_IndexWrite<odd>(x,y,z,n,d,p)];
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP> template <bool odd>
inline auto const& __attribute__((always_inline)) Population<NX,NY,NZ,LT,NPOP>::AA_Write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                                         unsigned int const n,       unsigned int const d,       unsigned int const p) const
{
    return F_[AA_IndexWrite<odd>(x,y,z,n,d,p)];
}

#endif // POPULATION_INDEXING_HPP_INCLUDED
