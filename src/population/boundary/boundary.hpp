#ifndef BOUNDARY_HPP_INCLUDED
#define BOUNDARY_HPP_INCLUDED

/**
 * \file     boundary.hpp
 * \mainpage Holds struct for boundary element information
*/


/**\struct boundaryElement
 * \brief  Struct holding properties of boundary element
 *
 * \tparam T   floating data type used for simulation
*/
template <typename T = double>
struct boundaryElement
{
    /// spatial position of element
    unsigned int const x;
    unsigned int const y;
    unsigned int const z;

    /// macroscopic properties
    T rho; // density
    T ux;  // velocity in x-direction
    T uy;  // velocity in y-direction
    T uz;  // velocity in z-direction
};

#endif // BOUNDARY_HPP_INCLUDED
