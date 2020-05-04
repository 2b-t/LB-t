#ifndef CONTINUUM_INDEXING_HPP_INCLUDED
#define CONTINUUM_INDEXING_HPP_INCLUDED

/**
 * \file     continuum_indexing.hpp
 * \mainpage Class members for indexing macroscopic values
*/


/**\fn         SpatialToLinear
 * \brief      Inline function for scalar indexing by means of two functions
 * \warning    Inline function! Has to be declared in header!
 *
 * \param[in]  x   x coordinate of cell
 * \param[in]  y   y coordinate of cell
 * \param[in]  z   z coordinate of cell
 * \param[in]  m   macroscopic value (0: density, 1-3: ux, uy,uz)
 * \return      requested linear scalar index
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
inline size_t __attribute__((always_inline)) Continuum<NX,NY,NZ,T>::SpatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                                                                    unsigned int const m) const
{
    return ((z*NY + y)*NX + x)*NM_ + m;
}


/**\fn         LinearToSpatial
 * \brief      Generate scalar coordinates from scalar index
 *
 * \param[out] x       return value x coordinate of cell
 * \param[out] y       return value y coordinate of cell
 * \param[out] z       return value z coordinate of cell
 * \param[in]  index   current linear scalar index
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::LinearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z,
                                            unsigned int& m, size_t const index) const
{
    size_t factor = NM_*NX*NY;
    size_t rest   = index%factor;

    z      = index/factor;

    factor = NM_*NX;
    y      = rest/factor;
    rest   = rest%factor;

    factor = NM_;
    x      = rest/factor;
    m      = rest%factor;
}


template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
inline T& __attribute__((always_inline)) Continuum<NX,NY,NZ,T>::operator() (unsigned int const x, unsigned int const y, unsigned int const z,
                                                                            unsigned int const m)
{
    return M_[SpatialToLinear(x,y,z,m)];
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
inline T const& __attribute__((always_inline)) Continuum<NX,NY,NZ,T>::operator() (unsigned int const x, unsigned int const y, unsigned int const z,
                                                                                  unsigned int const m) const
{
    return M_[SpatialToLinear(x,y,z,m)];
}


#endif //CONTINUUM_INDEXING_HPP_INCLUDED
