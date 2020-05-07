#ifndef BOUNDARY_ORIENTATION_HPP_INCLUDED
#define BOUNDARY_ORIENTATION_HPP_INCLUDED

/**
 * \file     boundary_orientation.hpp
 * \mainpage Holds orientation parameters for boundary conditions
 * \warning  Static classes with more complex members such as std::vector and std::array require C++17
*/

namespace orientation
{

    class Left
    {
        public:
            /// normal direction
            static constexpr std::array<int, 3> N = {  1,  0,  0 };
    };

    class Right
    {
        public:
            static constexpr std::array<int, 3> N = { -1,  0,  0 };
    };

    class Front
    {
        public:
            static constexpr std::array<int, 3> N = {  0,  1,  0 };
    };

    class Back
    {
        public:
            static constexpr std::array<int, 3> N = {  0, -1,  0 };
    };


    class Bottom
    {
        public:
            static constexpr std::array<int, 3> N = {  0,  0,  1 };
    };

    class Top
    {
        public:
            static constexpr std::array<int, 3> N = {  0,  0, -1 };
    };
}

#endif // BOUNDARY_ORIENTATION_HPP_INCLUDED
