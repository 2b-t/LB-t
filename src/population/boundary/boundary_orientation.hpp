#ifndef LBT_BOUNDARY_ORIENTATION
#define LBT_BOUNDARY_ORIENTATION

/**
 * \file     boundary_orientation.hpp
 * \mainpage Holds orientation parameters for boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <iostream>


namespace boundary
{
    /**\enum  boundary::Orientation
     * \brief Strongly typed for all possible boundary orientations
    */
    enum class Orientation { Left, Right, Front, Back, Bottom, Top };

    /**\fn            Orientation output stream operator
     * \brief         Output stream operator for the boundary orientation
     *
     * \param[in,out] os            Output stream
     * \param[in]     orientation   Orientation to be printed to output stream
     * \return        Output stream including the boundary condition orientation
    */
    std::ostream& operator<<(std::ostream& os, Orientation const orientation)
    {
        switch (orientation) {
            case Orientation::Left:
                os << "Left";
                break;
            case Orientation::Right:
                os << "Right";
                break;
            case Orientation::Front:
                os << "Front";
                break;
            case Orientation::Back:
                os << "Back";
                break;
            case Orientation::Bottom:
                os << "Bottom";
                break;
            case Orientation::Top:
                os << "Top";
                break;
            default:
                os << "None";
                break;
        }
        return os;
    }

    /**\class  Normal
     * \brief  Class for the normals for each individual direction
     *
     * \tparam O   Orientation of the normal
    */
    template <Orientation O>
    class Normal final
    {
        public:
            static constexpr int x =  0;
            static constexpr int y =  0;
            static constexpr int z =  0;
    };

    template <>
    class Normal<Orientation::Left> final
    {
        public:
            static constexpr int x =  1; ///< normal direction pointing into the fluid volume
            static constexpr int y =  0;
            static constexpr int z =  0;
    };

    template <>
    class Normal<Orientation::Right> final
    {
        public:
            static constexpr int x = -1;
            static constexpr int y =  0;
            static constexpr int z =  0;
    };

    template <>
    class Normal<Orientation::Front> final
    {
        public:
            static constexpr int x =  0;
            static constexpr int y =  1;
            static constexpr int z =  0;
    };

    template <>
    class Normal<Orientation::Back> final
    {
        public:
            static constexpr int x =  0;
            static constexpr int y = -1;
            static constexpr int z =  0;
    };

    template <>
    class Normal<Orientation::Bottom> final
    {
        public:
            static constexpr int x =  0;
            static constexpr int y =  0;
            static constexpr int z =  1;
    };

    template <>
    class Normal<Orientation::Top> final
    {
        public:
            static constexpr int x =  0;
            static constexpr int y =  0;
            static constexpr int z = -1;
    };
}

#endif // LBT_BOUNDARY_ORIENTATION
