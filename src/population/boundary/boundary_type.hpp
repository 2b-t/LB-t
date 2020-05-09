#ifndef BOUNDARY_TYPE_HPP_INCLUDED
#define BOUNDARY_TYPE_HPP_INCLUDED

/**
 * \file     boundary_type.hpp
 * \mainpage Holds classes for different types boundary conditions
*/

#include <array>

/**\def       T_N (orient,t,n)
 * \brief     Macro for determining if the velocity is tangential of normal
 *
 * \param[in] orient   orientation: 0 for tangential and +-1 for normal
 * \param[in] t        value that is returned if the velocity is tangental
 * \param[in] n        value that is returned if the velocity is normal
*/
#define T_N(orient,t,n)    orient == 0 ? t : n


namespace type
{
    /**\class  Boundary
     * \brief  Base class for types of boundaries
     * \tparam Type          type of boundary (velocity, pressure)
     * \tparam Orientation   orientation of the current boundary given by an orientation class with members x, y, z
    */
    template <class Type, class Orientation>
    class Boundary
    {
        public:
            template <typename T>
            inline std::array<T,4> getMacroscopicValues(std::array<T,4> const& boundary, std::array<T,4> const& interp)
            {
                return static_cast<Type*>(this)->implementation(boundary, interp);
            }
    };

    /**\class  Velocity
     * \brief  Derived class for velocity boundaries: enforce normal and tangential velocities
     * \tparam Orientation   orientation of the current boundary given by an orientation class with members x, y, z
    */
    template <class Orientation>
    class Velocity: public Boundary<Velocity<Orientation>, Orientation>
    {
        public:
            template <typename T>
            inline std::array<T,4> implementation(std::array<T,4> const& boundary, std::array<T,4> const& interp)
            {
                return { interp[0], boundary[1], boundary[2], boundary[3] };
            }
    };

    /**\class  Pressure
     * \brief  Derived class for pressure boundaries: enforce rho and tangential velocities
     * \tparam Orientation   orientation of the current boundary given by an orientation class with members x, y, z
    */
    template <class Orientation>
    class Pressure: public Boundary<Pressure<Orientation>, Orientation>
    {
        public:
            template <typename T>
            inline std::array<T,4> implementation(std::array<T,4> const& boundary, std::array<T,4> const& interp)
            {
                return { boundary[0],
                         T_N(Orientation::x, boundary[1], interp[1]),
                         T_N(Orientation::y, boundary[2], interp[2]),
                         T_N(Orientation::z, boundary[3], interp[3]) };
            }
    };
}

#endif // BOUNDARY_TYPE_HPP_INCLUDED
