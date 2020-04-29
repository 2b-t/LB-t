#ifndef CONTINUUM_INDEXING_HPP_INCLUDED
#define CONTINUUM_INDEXING_HPP_INCLUDED


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
template <typename T>
inline size_t __attribute__((always_inline)) Continuum<T>::SpatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                                                           unsigned int const m) const
{
    return ((z*NY + y)*NX + x)*NM + m;
}


/**\fn         LinearToSpatial
 * \brief      Generate scalar coordinates from scalar index
 *
 * \param[out] x       return value x coordinate of cell
 * \param[out] y       return value y coordinate of cell
 * \param[out] z       return value z coordinate of cell
 * \param[in]  index   current linear scalar index
*/
template <typename T>
void Continuum<T>::LinearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z,
                                   unsigned int& m, size_t const index) const
{
    size_t factor = NM*NX*NY;
    size_t rest   = index%factor;

    z      = index/factor;

    factor = NM*NX;
    y      = rest/factor;
    rest   = rest%factor;

    factor = NM;
    x      = rest/factor;
    m      = rest%factor;
}


template <typename T>
inline T& __attribute__((always_inline)) Continuum<T>::operator() (unsigned int const x, unsigned int const y, unsigned int const z,
                                                                   unsigned int const m)
{
    return M[SpatialToLinear(x,y,z,m)];
}

template <typename T>
inline T const& __attribute__((always_inline)) Continuum<T>::operator() (unsigned int const x, unsigned int const y, unsigned int const z,
                                                                         unsigned int const m) const
{
    return M[SpatialToLinear(x,y,z,m)];
}


#endif //CONTINUUM_INDEXING_HPP_INCLUDED
