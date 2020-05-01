#ifndef ALIGN_HPP_INCLUDED
#define ALIGN_HPP_INCLUDED

/**
 * \file     align.hpp
 * \mainpage File for cache-related settings and macros
*/


/// size of the cache line of the architecture (typically 64 bytes)
#define CACHE_LINE    64

#ifdef __AVX2__

	/// header for AVX2 intrinsics
	#include <immintrin.h>

	/// size of the intrinsics (AVX2: 256/8=32) and corresponding number of doubles in an intrinsic
	#define AVX_SIZE          sizeof(__m256d)
	#define AVX_REG_SIZE      sizeof(__m256d)/sizeof(double)

#endif // __AVX2__

#endif // ALIGN_HPP_INCLUDED
