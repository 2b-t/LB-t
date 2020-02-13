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
        //OpenMP.SetThreadsNum(1);
    #endif

    /// print disclaimer
    if ((argc > 1) && (strcmp(argv[1], "--version") == 0))
    {
        print_disclaimer();
    }

    /// settings
    F_TYPE       constexpr Re    = 10.0;
    F_TYPE       constexpr U     = 0.05;
    unsigned int constexpr L     = 50;
    unsigned int constexpr NX    = 320;
    unsigned int constexpr NY    = 128;
    unsigned int constexpr NZ    = 128;
    unsigned int constexpr NT    = 400;
    F_TYPE       constexpr RHO_0 = 1.0;
    F_TYPE       constexpr U_0   = U;
    F_TYPE       constexpr V_0   = 0.0;
    F_TYPE       constexpr W_0   = 0.0;

    /// set up lattice, microscopic value and collision operator
    Lattice<F_TYPE>   D3Q27(Re,RHO_0,U,L,NX,NY,NZ);
    Continuum<F_TYPE> Macro(NX,NY,NZ);

    /// define boundary conditions and import scenario
    std::vector<boundary_element<double>> wall;
    std::vector<boundary_element<double>> inlet;
    std::vector<boundary_element<double>> outlet;

    unsigned int const radius = L/4;
    unsigned int const position[3] = {NX/2, NY/2, NZ/2};
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
