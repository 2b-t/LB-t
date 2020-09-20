#ifndef LBT_BOUNDARY_TYPE
#define LBT_BOUNDARY_TYPE

/**
 * \file     boundary_type.hpp
 * \mainpage Holds classes for different types boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <array>


namespace type
{
    /**\def       velocityComponent
     * \brief     Function for determining if the velocity is tangential of normal
     *
     * \tparam    T                     Floating data type used for the simulation
     * \param[in] tangentialOrNormal    Orientation: 0 for tangential and +-1 for normal
     * \param[in] tangentialComponent   Value that is returned if the velocity is tangental
     * \param[in] normalComponent       Value that is returned if the velocity is normal
    */
    template <typename T>
    static inline T velocityComponent(int const tangentialOrNormal, T const tangentialComponent, T const normalComponent)
    {
        return (tangentialOrNormal == 0) ? tangentialComponent : normalComponent;
    }

    /**\class  Velocity
     * \brief  Class for velocity boundaries: enforce normal and tangential velocities
     *
     * \tparam Orientation   Orientation of the current boundary given by an orientation class with members x, y, z
    */
    template <class Orientation>
    class Velocity final
    {
        public:
            /**\fn        getMacroscopicValues
             * \brief     Determine if the values of the boundary or the interpolated values from the simulation should be used
             *
             * \tparam    T          Floating data type used for simulation
             * \param[in] boundary   Values at the boundary
             * \param[in] interp     Interpolated values
             * \return    An array that contains a mixture of enforced or interpolated values depending on the precise boundary
            */
            template <typename T>
            static inline std::array<T,4> getMacroscopicValues(std::array<T,4> const& boundary, std::array<T,4> const& interp)
            {
                return { interp[0], boundary[1], boundary[2], boundary[3] };
            }
    };

    /**\class  Pressure
     * \brief  Class for pressure boundaries: enforce rho and tangential velocities
     *
     * \tparam Orientation   orientation of the current boundary given by an orientation class with members x, y, z
    */
    template <class Orientation>
    class Pressure final
    {
        public:
            template <typename T>
            static inline std::array<T,4> getMacroscopicValues(std::array<T,4> const& boundary, std::array<T,4> const& interp)
            {
                return { boundary[0],
                         velocityComponent(Orientation::x, boundary[1], interp[1]),
                         velocityComponent(Orientation::y, boundary[2], interp[2]),
                         velocityComponent(Orientation::z, boundary[3], interp[3]) };
            }
    };
}

#endif // LBT_BOUNDARY_TYPE
