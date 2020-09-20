#ifndef LBT_BOUNDARY_ORIENTATION_CONCEPT
#define LBT_BOUNDARY_ORIENTATION_CONCEPT

/**
 * \file     boundary_orientation_concept.hpp
 * \mainpage C++20 concept of a boundary orientation
 * \author   Tobit Flatscher (github.com/2b-t)
 *
 * \warning  Requires C++20
*/

#if __cplusplus >= 201709L

#include <concepts>


namespace orientation
{
    /**\concept Orientation
     * \brief   Concept of a boundary condition orientation
     *
     * \tparam  O   The orientation
    */
    template <class O>
    concept Orientation = requires
    {
        { O::x } -> std::convertible_to<int>;
        { O::y } -> std::convertible_to<int>;
        { O::z } -> std::convertible_to<int>;
    };
}

#endif

#endif // LBT_BOUNDARY_ORIENTATION_CONCEPT
