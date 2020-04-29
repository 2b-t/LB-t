#include <string.h>
#include <iostream>

#include "memory_alignment.hpp"
#include "disclaimer.hpp"
#include "parallelism.hpp"
#include "lattice.hpp"
#include "continuum.hpp"
#include "geometry.hpp"
#include "initialisation.hpp"
#include "collision_bgk.hpp"
#include "collision_bgk_avx.hpp"
#include "boundary.hpp"
#include "boundary_bounceback.hpp"
#include "timer.hpp"


int main(int argc, char** argv)
{
    /// set up OpenMP
    #ifdef _OPENMP
        Parallelism OpenMP;
        OpenMP.SetThreadsNum(1);
    #endif

    /// print disclaimer
    if ((argc > 1) && (strcmp(argv[1], "--version") == 0))
    {
        print_disclaimer();
    }

    /// settings
    constexpr F_TYPE       Re    = 10.0;
    constexpr F_TYPE       U     = 0.05;
    constexpr unsigned int L     = 50;
    constexpr unsigned int NX    = 64;
    constexpr unsigned int NY    = 64;
    constexpr unsigned int NZ    = 64;
    constexpr unsigned int NT    = 100;
    constexpr F_TYPE       RHO_0 = 1.0;
    constexpr F_TYPE       U_0   = U;
    constexpr F_TYPE       V_0   = 0.0;
    constexpr F_TYPE       W_0   = 0.0;

    /// set up lattice, microscopic value and collision operator
    Lattice<F_TYPE>   D3Q27(Re,RHO_0,U,L,NX,NY,NZ);
    Continuum<F_TYPE> Macro(NX,NY,NZ);

    /// define boundary conditions and import scenario
    std::vector<boundary_element<F_TYPE>> wall;
    std::vector<boundary_element<F_TYPE>> inlet;
    std::vector<boundary_element<F_TYPE>> outlet;

    constexpr unsigned int radius = L/4;
    constexpr unsigned int position[3] = {NX/2, NY/2, NZ/2};
    Cylinder_3D(D3Q27, radius, position, "x", true, wall, inlet, outlet);

    /// define initial conditions
    init_continuum(Macro, D3Q27, RHO_0, U_0, V_0, W_0);
    init_lattice(Macro,   D3Q27);

    /// main loop
    Timer Stopwatch;
    Stopwatch.Start();

    for (size_t i = 0; i < NT; i+=2)
    {
        collision_bgk<false>(Macro, D3Q27, true, 0);
        boundary_bounceback_halfway<false>(wall, D3Q27, 0);

        collision_bgk<true>(Macro,  D3Q27, true, 0);
        boundary_bounceback_halfway<true>(wall, D3Q27, 0);

        if (i%20 == 0)
        {
            Macro.SetZero(wall);
            Macro.Export("step",i);
        }
    }

    Stopwatch.Stop();

    std::cout << Stopwatch.GetRuntime() << std::endl;

    /// export
    //Macro.SetZero(wall);
    //Macro.ExportVtk(NT);
    //Macro.ExportScalarVtk(0,"rho",NT);
    //Macro.Export("step",NT);

    return EXIT_SUCCESS;
}
