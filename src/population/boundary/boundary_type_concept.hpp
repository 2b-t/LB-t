#ifndef LBT_BOUNDARY_TYPE_CONCEPT
#define LBT_BOUNDARY_TYPE_CONCEPT

/**
 * \file     boundary_type_concept.hpp
 * \mainpage C++20 concept of a boundary type
 * \author   Tobit Flatscher (github.com/2b-t)
 *
 * \warning  Requires C++20
*/

#if __cplusplus >= 201709L

#include <array>
#include <concepts>


namespace type
{
    /**\concept Type
     * \brief   Concept of a boundary condition orientation
     *
     * \tparam  T             The boundary type
     * \tparam  Orientation   Orientation of the current boundary given by an orientation class with members x, y, z
     * \tparam  U             Floating data type used for simulation
    */
    template <template <class Orientation> class T, class Orientation, typename U>
    concept Type = requires
    {
        T<Orientation>:: template getMacroscopicValues<U>;
    };
}

#endif

#endif // LBT_BOUNDARY_TYPE_CONCEPT
