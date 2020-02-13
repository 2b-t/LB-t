#ifndef GEOMETRY_HPP_INCLUDED
#define GEOMETRY_HPP_INCLUDED

/**
 * \file     geometry.hpp
 * \mainpage Sample geometry import
*/


#include <vector>
#include <cstring>

#include "lattice.hpp"
#include "boundary.hpp"


/**\fn         Cylinder_3D
 * \brief      Load pre-defined scenario of a three-dimensional flow around cylinder
 *
 * \param[in]  radius           unsigned integer that holds the radius of the cylinder
 * \param[in]  position         vector of unsigned integers that holds the position of the center of the
 *                              cylinder
 * \param[in]  orientation      pointer to a character array that holds the orientation
 * \param[in]  walls            a boolean variable that indicates if walls on the side should be
 *                              included (true) or not (wrong)
 * \param[out] wall             a pointer to a vector of relevant linear indices that correspond to a wall
 * \param[out] vel_boundary     a pointer to a vector of relevant linear indices that correspond to
 *                              a velocity boundary
 * \param[out] press_boundary   a pointer to a vector of relevant linear indices that correspond to
 *                              a pressure boundary
*/
template <typename T = double>
void Cylinder_3D(Lattice<T> const& lattice,
                 unsigned int const radius, unsigned int const (&position)[3],
                 char const * orientation, bool const walls,
                 std::vector<boundary_element<T>>& wall,
                 std::vector<boundary_element<T>>& inlet, std::vector<boundary_element<T>>& outlet)
{
    if (strcmp(orientation, "x")==0)
    {
        for(unsigned int z = 0; z < lattice.NZ; ++z)
        {
            for(unsigned int y = 0; y < lattice.NY; ++y)
            {
                for(unsigned int x = 0; x < lattice.NX; ++x)
                {
                    //create element
                    boundary_element<T> const element = {x, y, z, 0.0, 0.0, 0.0, 0.0};

                    if ((x-position[0])*(x-position[0]) + (y-position[1])*(y-position[1]) <= radius*radius)
                    {
                        //wall.push_back(element);
                    }
                    else if ((y == 0) || (y == lattice.NY-1) || (z == 0) || (z == lattice.NZ-1))
                    {
                        if (walls == true)
                        {
                            wall.push_back(element);
                        }
                    }
                    else if (x == 0)
                    {
                        inlet.push_back(element);
                    }
                    else if (x == lattice.NX-1)
                    {
                        outlet.push_back(element);
                    }
                }
            }
        }
    }

    else
    {
        //Error
    }
}

#endif //GEOMETRY_HPP_INCLUDED
