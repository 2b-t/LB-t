#ifndef LBT_BOUNDARY_ORIENTATION
#define LBT_BOUNDARY_ORIENTATION

/**
 * \file     boundary_orientation.hpp
 * \mainpage Holds orientation parameters for boundary conditions
 * \author   Tobit Flatscher (github.com/2b-t)
*/


namespace orientation
{
    class Left final
    {
        public:
            static constexpr int x =  1; ///< normal direction pointing into the fluid volume
            static constexpr int y =  0;
            static constexpr int z =  0;
    };

    class Right final
    {
        public:
            static constexpr int x = -1;
            static constexpr int y =  0;
            static constexpr int z =  0;
    };

    class Front final
    {
        public:
            static constexpr int x =  0;
            static constexpr int y =  1;
            static constexpr int z =  0;
    };

    class Back final
    {
        public:
            static constexpr int x =  0;
            static constexpr int y = -1;
            static constexpr int z =  0;
    };


    class Bottom final
    {
        public:
            static constexpr int x =  0;
            static constexpr int y =  0;
            static constexpr int z =  1;
    };

    class Top final
    {
        public:
            static constexpr int x =  0;
            static constexpr int y =  0;
            static constexpr int z = -1;
    };
}

#endif // LBT_BOUNDARY_ORIENTATION
