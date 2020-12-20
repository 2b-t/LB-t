#ifndef LBT_CONTINUUM_INDEXING
#define LBT_CONTINUUM_INDEXING

/**
 * \file     continuum_indexing.hpp
 * \mainpage Class members for indexing macroscopic values
 * \author   Tobit Flatscher (github.com/2b-t)
*/


template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
inline size_t __attribute__((always_inline)) Continuum<NX,NY,NZ,T>::spatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                                                                    unsigned int const m) const noexcept
{
    return ((z*NY + y)*NX + x)*numberOfMacroscopicValues + m;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
std::tuple<unsigned int, unsigned int, unsigned int, unsigned int> Continuum<NX,NY,NZ,T>::linearToSpatial(size_t const index) const noexcept
{
    size_t factor = numberOfMacroscopicValues*NX*NY;
    size_t rest   = index%factor;

    unsigned int const z = index/factor;

    factor = numberOfMacroscopicValues*NX;
    unsigned int const y = rest/factor;
    rest   = rest%factor;

    factor = numberOfMacroscopicValues;
    unsigned int const x = rest/factor;
    unsigned int const m = rest%factor;

    return std::make_tuple(x,y,z,m);
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
inline T& __attribute__((always_inline)) Continuum<NX,NY,NZ,T>::operator() (unsigned int const x, unsigned int const y, unsigned int const z,
                                                                            unsigned int const m) noexcept
{
    return M[spatialToLinear(x,y,z,m)];
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
inline T const& __attribute__((always_inline)) Continuum<NX,NY,NZ,T>::operator() (unsigned int const x, unsigned int const y, unsigned int const z,
                                                                                  unsigned int const m) const noexcept
{
    return M[spatialToLinear(x,y,z,m)];
}

#endif // LBT_CONTINUUM_INDEXING
