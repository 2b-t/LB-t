#ifndef BOUNDARY_ORIENTATION_HPP_INCLUDED
#define BOUNDARY_ORIENTATION_HPP_INCLUDED

/**
 * \file     boundary_orientation.hpp
 * \mainpage Holds orientation parameters for boundary conditions
*/

namespace orientation
{
    class Left
    {
        public:
            static constexpr int x =  1; ///< normal direction pointing into the fluid volume
            static constexpr int y =  0;
            static constexpr int z =  0;
    };

    class Right
    {
        public:
            static constexpr int x = -1;
            static constexpr int y =  0;
            static constexpr int z =  0;
    };

    class Front
    {
        public:
            static constexpr int x =  0;
            static constexpr int y =  1;
            static constexpr int z =  0;
    };

    class Back
    {
        public:
            static constexpr int x =  0;
            static constexpr int y = -1;
            static constexpr int z =  0;
    };


    class Bottom
    {
        public:
            static constexpr int x =  0;
            static constexpr int y =  0;
            static constexpr int z =  1;
    };

    class Top
    {
        public:
            static constexpr int x =  0;
            static constexpr int y =  0;
            static constexpr int z = -1;
    };
}

#endif // BOUNDARY_ORIENTATION_HPP_INCLUDED
