#include <array>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "continuum/continuum.hpp"
#include "continuum/initialisation.hpp"
#include "general/disclaimer.hpp"
#include "general/memory_alignment.hpp"
#include "general/parallelism.hpp"
#include "general/parameters_export.hpp"
#include "general/timer.hpp"
#include "geometry/cylinder.hpp"
#include "lattice/D3Q27.hpp"
#include "population/boundary/boundary.hpp"
#include "population/boundary/boundary_bounceback.hpp"
#include "population/collision/collision_bgk.hpp"
#include "population/initialisation.hpp"
#include "population/population.hpp"


int main(int argc, char** argv)
{
    /// set up OpenMP ------------------------------------------------------------------------------
    #ifdef _OPENMP
        Parallelism OpenMP;
        OpenMP.SetThreadsNum(1);
    #endif

    /// print disclaimer ---------------------------------------------------------------------------
    if (argc > 1)
    {
        if ((strcmp(argv[1], "--version") == 0) || (strcmp(argv[1], "--v") == 0))
        {
            print_disclaimer();
        }
        else if (strcmp(argv[1], "--convert") == 0)
        {

        }
        else if ((strcmp(argv[1], "--info") == 0) || (strcmp(argv[1], "--help") == 0))
        {
            std::cerr << "Usage: '--convert'             Convert *.bin files to *.vtk" << std::endl;
            std::cerr << "       '--help'    or '--info' Show help"                    << std::endl;
            std::cerr << "       '--version' or '--v'    Show build version"           << std::endl;
        }
    }

    /// settings -----------------------------------------------------------------------------------
    // floating point accuracy
    typedef double F_TYPE;

    // lattice
    typedef lattice::D3Q27<F_TYPE> DdQq;

    // physics
    constexpr F_TYPE       Re    = 10.0;
    constexpr F_TYPE       U     = 0.05;
    constexpr unsigned int L     = 50;

    // spatial and temporal resolution
    constexpr unsigned int NX    = 64;
    constexpr unsigned int NY    = 64;
    constexpr unsigned int NZ    = 64;
    constexpr unsigned int NT    = 100;

    // initial conditions
    constexpr F_TYPE       RHO_0 = 1.0;
    constexpr F_TYPE       U_0   = U;
    constexpr F_TYPE       V_0   = 0.0;
    constexpr F_TYPE       W_0   = 0.0;

    ExportParameters(NX, NY, NZ, NT, Re, RHO_0, L, U);

    /// set up microscopic and macroscopic arrays --------------------------------------------------
    Continuum<NX,NY,NZ,F_TYPE> Macro;
    Population<NX,NY,NZ,DdQq>  Micro(Re,U,L);

    /// define boundary conditions -----------------------------------------------------------------
    alignas(CACHE_LINE) std::vector<boundaryElement<F_TYPE>> wall;
    alignas(CACHE_LINE) std::vector<boundaryElement<F_TYPE>> inlet;
    alignas(CACHE_LINE) std::vector<boundaryElement<F_TYPE>> outlet;

    constexpr unsigned int radius = L/4;
    constexpr std::array<unsigned int,3> position = {NX/2, NY/2, NZ/2};
    Cylinder3D<NX,NY,NZ>(radius, position, "x", true, wall, inlet, outlet);

    /// define initial conditions ------------------------------------------------------------------
    InitContinuum<NX,NY,NZ>(Macro, RHO_0, U_0, V_0, W_0);
    InitLattice<NX,NY,NZ,DdQq>(Macro, Micro);

    /// main loop ----------------------------------------------------------------------------------
    Timer Stopwatch;
    Stopwatch.Start();

    for (size_t i = 0; i < NT; i+=2)
    {
        collision_bgk<false,NX,NY,NZ,DdQq>(Macro, Micro, true, 0);
        boundary_bounceback_halfway<false,NX,NY,NZ>(wall, Micro, 0);

        collision_bgk<true,NX,NY,NZ,DdQq>(Macro, Micro, true, 0);
        boundary_bounceback_halfway<true,NX,NY,NZ>(wall, Micro, 0);

        if (i%20 == 0)
        {
            //Macro.SetZero(wall);
            //Macro.Export("step",i);
        }
    }

    Stopwatch.Stop();

    std::cout << "Simulation runtime: " << Stopwatch.GetRuntime() << std::endl;

    /// export -------------------------------------------------------------------------------------
    /*Macro.SetZero(wall);
    Macro.Export("step",NT);
    Macro.ExportVtk(NT);
    Macro.ExportScalarVtk(0,"rho",NT);*/

    return EXIT_SUCCESS;
}
