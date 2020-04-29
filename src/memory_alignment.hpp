#ifndef ALIGN_HPP_INCLUDED
#define ALIGN_HPP_INCLUDED

/**
 * \file     align.hpp
 * \mainpage File for cache-related settings
*/


/// size of the cache line of the architecture (typically 64 bit)
#define CACHE_LINE    64

/// define type to be used for populations (must match the intrinsic data type as well!)
typedef double        F_TYPE;


#ifdef __AVX2__

/// header for AVX2 intrinsics
#include <immintrin.h>

// size of the intrinsic (AVX2: 256/8=32) and corresponding number of values of type F_TYPE
#define AVX_SIZE          sizeof(__m256d)
#define AVX_REG_SIZE      sizeof(__m256d)/sizeof(F_TYPE)

#endif // __AVX2__

#endif //ALIGN_HPP_INCLUDED
