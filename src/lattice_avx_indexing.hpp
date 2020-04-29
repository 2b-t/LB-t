#ifndef LATTICE_AVX_INDEXING_HPP_INCLUDED
#define LATTICE_AVX_INDEXING_HPP_INCLUDED

#include "memory_alignment.hpp"

template <typename T> template <bool odd>
inline void __attribute__((always_inline)) Lattice<T>::Load(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                            T (&f)[ND], unsigned int const p) const
{
    /*if(odd == false)
    {
        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #pragma GCC unroll (4)
            for (size_t d = 0; d < OFF; d += AVX_REG_SIZE)
            {
                _mm256_store_pd(&f[n*OFF + d], _mm256_load_pd(&F[AA_IndexRead<odd>(x,y,z,n,d,p)]));
            }
        }
        f[OFF] = 0.0;
    }
    else*/
    {
        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #pragma GCC unroll (16)
            for(unsigned int d = 0; d < OFF; ++d)
            {
                f[n*OFF + d] = F[AA_IndexRead<odd>(x,y,z,n,d,p)];
            }
        }
        f[OFF] = 0.0;
    }
}

template <typename T> template <bool odd>
inline void __attribute__((always_inline)) Lattice<T>::Store(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                             T const (&f)[ND], unsigned int const p)
{
    /*if(odd == false)
    {
        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #pragma GCC unroll (4)
            for (size_t d = 0; d < OFF; d += AVX_REG_SIZE)
            {
                _mm256_store_pd(&F[AA_IndexWrite<odd>(x,y,z,n,d,p)], _mm256_load_pd(&f[n*OFF + d]));
            }
        }
    }
    else*/
    {
        #pragma GCC unroll (2)
        for(unsigned int n = 0; n <= 1; ++n)
        {
            #pragma GCC unroll (16)
            for(unsigned int d = 0; d < OFF; ++d)
            {
                F[AA_IndexWrite<odd>(x,y,z,n,d,p)] = f[n*OFF + d];
            }
        }
    }
}


#endif //LATTICE_AVX_INDEXING_HPP_INCLUDED
