#ifndef LBT_BOUNDARY_ORIENTATION
#define LBT_BOUNDARY_ORIENTATION

/**
 * \file     boundary_orientation.hpp
 * \mainpage Holds orientation parameters for boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
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

#endif // LBT_BOUNDARY_ORIENTATION
