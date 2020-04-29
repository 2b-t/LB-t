#ifndef BOUNDARY_HPP_INCLUDED
#define BOUNDARY_HPP_INCLUDED


template <typename T = double>
struct boundary_element
{
    unsigned int const x,  y,  z; //spatial position
    T rho, ux, uy, uz;            //pointwise density and velocity
};

#endif //BOUNDARY_HPP_INCLUDED
