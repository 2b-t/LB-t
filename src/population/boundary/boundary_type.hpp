#ifndef LBT_BOUNDARY_TYPE
#define LBT_BOUNDARY_TYPE

/**
 * \file     boundary_type.hpp
 * \mainpage Holds classes for different types boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <array>


/**\def       T_N (orient,t,n)
 * \brief     Macro for determining if the velocity is tangential of normal
 *
 * \param[in] orient   orientation: 0 for tangential and +-1 for normal
 * \param[in] t        value that is returned if the velocity is tangental
 * \param[in] n        value that is returned if the velocity is normal
*/
#define T_N(orient,t,n)    (orient == 0) ? t : n


namespace type
{
    /**\class  Velocity
     * \brief  Class for velocity boundaries: enforce normal and tangential velocities
     * 
     * \tparam Orientation   orientation of the current boundary given by an orientation class with members x, y, z
    */
    template <class Orientation>
    class Velocity
    {
        public:
            /**\fn        getMacroscopicValues
             * \brief     Determine if the values of the boundary or the interpolated values from the simulation should be used
             *
             * \tparam    T          floating data type used for simulation
             * \param[in] boundary   values at the boundary
             * \param[in] interp     interpolated values
             * \return    an array that contains a mixture of enforced or interpolated values depending on the precise boundary
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
    class Pressure
    {
        public:
            template <typename T>
            static inline std::array<T,4> getMacroscopicValues(std::array<T,4> const& boundary, std::array<T,4> const& interp)
            {
                return { boundary[0],
                         T_N(Orientation::x, boundary[1], interp[1]),
                         T_N(Orientation::y, boundary[2], interp[2]),
                         T_N(Orientation::z, boundary[3], interp[3]) };
            }
    };
}

#endif // LBT_BOUNDARY_TYPE
