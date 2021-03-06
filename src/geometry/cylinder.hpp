#ifndef LBT_CYLINDER
#define LBT_CYLINDER

/**
 * \file     cylinder.hpp
 * \mainpage 2D and 3D cylinder sample geometry import
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "../population/boundary/boundary.hpp"
#include "../population/boundary/boundary_bounceback.hpp"
#include "../population/boundary/boundary_guo.hpp"
#include "../population/boundary/boundary_orientation.hpp"
#include "../population/boundary/boundary_type.hpp"


namespace geometry
{
    /**\fn        cylinder
     * \brief     Load pre-defined scenario of a three-dimensional flow around cylinder
     *
     * \tparam    NX                 Simulation domain resolution in x-direction
     * \tparam    NY                 Simulation domain resolution in y-direction
     * \tparam    NZ                 Simulation domain resolution in z-direction
     * \tparam    T                  Floating data type used for simulation
     * \param[in] radius             Unsigned integer that holds the radius of the cylinder
     * \param[in] position           Array of unsigned integers that holds the position of the center of the cylinder
     * \param[in] inletOrientation   Orientation of scenario
     * \param[in] isWalls            A boolean variable that indicates if walls on the side should be included (true) or not (wrong)
     * \param[in] RHO                Simulation density
     * \param[in] U                  Characteristic velocity (measurement for temporal resolution)
     * \param[in] L                  Characteristic length scale of the problem
     * \param[in] p                  The index of the relevant population
     * \return    A tuple containing pointers to all boundary conditions
    */
    template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP>
    std::tuple< Guo<boundary::Type::Velocity,boundary::Orientation::Left,NX,NY,NZ,LT,T,NPOP>, 
                Guo<boundary::Type::Pressure,boundary::Orientation::Right,NX,NY,NZ,LT,T,NPOP>,
                HalfwayBounceBack<NX,NY,NZ,LT,T,NPOP> >
    cylinder(std::shared_ptr<Population<NX,NY,NZ,LT,T,NPOP>> population, unsigned int const radius, std::array<unsigned int,3> const& position,
             boundary::Orientation const inletOrientation, bool const isWalls, T const RHO, T const U, T const V, T const W, unsigned int p = 0) noexcept
    {
        alignas(CACHE_LINE) std::vector<boundary::Element<T>> wall;
        alignas(CACHE_LINE) std::vector<boundary::Element<T>> inlet;
        alignas(CACHE_LINE) std::vector<boundary::Element<T>> outlet;

        if (inletOrientation == boundary::Orientation::Left)
        {
            for(unsigned int z = 0; z < NZ; ++z)
            {
                for(unsigned int y = 0; y < NY; ++y)
                {
                    for(unsigned int x = 0; x < NX; ++x)
                    {
                        /// add to corresponding boundary
                        if ((x-position[0])*(x-position[0]) + (y-position[1])*(y-position[1]) <= radius*radius)
                        {
                            wall.push_back(boundary::Element<T>{x, y, z, RHO, 0, 0, 0});
                        }
                        else if ((y == 0) || (y == NY-1))
                        {
                            if (isWalls == true)
                            {
                                wall.push_back(boundary::Element<T>{x, y, z, RHO, 0, 0, 0});
                            }
                        }
                        else if (((z == 0) || (z == NZ-1)) && (LT<T>::DIM == 3))
                        {
                            if (isWalls == true)
                            {
                                wall.push_back(boundary::Element<T>{x, y, z, RHO, 0, 0, 0});
                            }
                        }
                        else if (x == 0)
                        {
                            inlet.push_back(boundary::Element<T>{x, y, z, RHO, U, V, W});
                        }
                        else if (x == NX-1)
                        {
                            outlet.push_back(boundary::Element<T>{x, y, z, RHO, U, V, W});
                        }
                    }
                }
            }
        }
        else
        {
            std::cerr << "Warning: Geometry orientation " << inletOrientation << " not found." << std::endl;
            std::exit(EXIT_FAILURE);
        }

        return { Guo<boundary::Type::Velocity,boundary::Orientation::Left,NX,NY,NZ,LT,T,NPOP>(population, inlet, p),
                 Guo<boundary::Type::Pressure,boundary::Orientation::Right,NX,NY,NZ,LT,T,NPOP>(population, outlet, p),
                 HalfwayBounceBack<NX,NY,NZ,LT,T,NPOP>(population, wall, p) };
    }

}

#endif // LBT_CYLINDER
