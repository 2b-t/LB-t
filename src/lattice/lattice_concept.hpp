#ifndef LBT_LATTICE_CONCEPT
#define LBT_LATTICE_CONCEPT

/**
 * \file     lattice_concept.hpp
 * \mainpage C++20 concept of a lattice
 * \author   Tobit Flatscher (github.com/2b-t)
 *
 * \warning  Requires C++20
*/

#if __cplusplus >= 201709L

#include <concepts>


namespace lattice
{
    /**\concept Lattice
     * \brief   Concept of a lattice
     *
     * \tparam  LT   A lattice class
     * \tparam  T    Floating data type used for simulation
    */
    template <template <typename T> class LT, typename T>
    concept Lattice = requires
    {
        { LT<T>::DIM }        -> std::convertible_to<unsigned int>;
        { LT<T>::SPEEDS }     -> std::convertible_to<unsigned int>;
        { LT<T>::HSPEED }     -> std::convertible_to<unsigned int>;
        { LT<T>::PAD }        -> std::convertible_to<unsigned int>;
        { LT<T>::ND }         -> std::convertible_to<unsigned int>;
        { LT<T>::OFF }        -> std::convertible_to<unsigned int>;
        { LT<T>::DX.at(0) }   -> std::convertible_to<T>;
        { LT<T>::DY.at(0) }   -> std::convertible_to<T>;
        { LT<T>::DZ.at(0) }   -> std::convertible_to<T>;
        { LT<T>::W.at(0) }    -> std::convertible_to<T>;
        { LT<T>::MASK.at(0) } -> std::convertible_to<T>;
        { LT<T>::CS }         -> std::convertible_to<T>;
    };
}

#endif

#endif // LBT_LATTICE_CONCEPT
