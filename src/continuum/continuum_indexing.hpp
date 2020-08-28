#ifndef LBT_CONTINUUM_INDEXING
#define LBT_CONTINUUM_INDEXING

/**
 * \file     continuum_indexing.hpp
 * \mainpage Class members for indexing macroscopic values
*/


template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
inline size_t __attribute__((always_inline)) Continuum<NX,NY,NZ,T>::spatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                                                                    unsigned int const m) const
{
    return ((z*NY + y)*NX + x)*NM_ + m;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::linearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z,
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

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
inline T& __attribute__((always_inline)) Continuum<NX,NY,NZ,T>::operator() (unsigned int const x, unsigned int const y, unsigned int const z,
                                                                            unsigned int const m)
{
    return M_[spatialToLinear(x,y,z,m)];
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
inline T const& __attribute__((always_inline)) Continuum<NX,NY,NZ,T>::operator() (unsigned int const x, unsigned int const y, unsigned int const z,
                                                                                  unsigned int const m) const
{
    return M_[spatialToLinear(x,y,z,m)];
}

#endif // LBT_CONTINUUM_INDEXING
