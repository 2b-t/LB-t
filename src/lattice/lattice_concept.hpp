#ifndef LBT_LATTICE_CONCEPT
#define LBT_LATTICE_CONCEPT

/**
 * \file     lattice_concept.hpp
 * \mainpage C++20 concept of a lattice
 * \author   Tobit Flatscher (github.com/2b-t)
 *
 * \warning  Requires C++20
*/

#if __cplusplus > 201703L

#include <concepts>
#include <numeric>


namespace lattice
{
    /**\concept Lattice
     * \brief   Concept of a lattice
     *
     * \tparam  LT   A lattice class
    */
    template<typename LT>
    concept Lattice = requires
    {
        LT::DIM;
        LT::SPEEDS;
        LT::HSPEED;
        LT::PAD;
        LT::ND;
        LT::OFF;
        std::accumulate(LT::DX.begin(),   LT::DX.end(),   0.0);
        std::accumulate(LT::DY.begin(),   LT::DY.end(),   0.0);
        std::accumulate(LT::DZ.begin(),   LT::DZ.end(),   0.0);
        std::accumulate(LT::W.begin(),    LT::W.end(),    0.0);
        std::accumulate(LT::MASK.begin(), LT::MASK.end(), 0.0);
    };
}

#endif

#endif // LBT_LATTICE_CONCEPT
