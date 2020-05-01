#ifndef BOUNDARY_HPP_INCLUDED
#define BOUNDARY_HPP_INCLUDED

/**
 * \file     boundary.hpp
 * \mainpage Holds struct for boundary element information
*/


/**\struct boundaryElement
 * \brief  Struct holding properties of boundary element
*/
template <typename T = double>
struct boundaryElement
{
    /// spatial position of element
    unsigned int const x;
    unsigned int const y;
    unsigned int const z;

    /// macroscopic properties
    T rho;
    T ux;
    T uy;
    T uz;
};

#endif // BOUNDARY_HPP_INCLUDED
