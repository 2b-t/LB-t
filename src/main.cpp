#include <array>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "continuum/continuum.hpp"
#include "general/disclaimer.hpp"
#include "general/memory_alignment.hpp"
#include "general/output.hpp"
#include "general/parallelism.hpp"
#include "general/parameters_export.hpp"
#include "general/timer.hpp"
#include "geometry/cylinder.hpp"
#include "geometry/sphere.hpp"
#include "lattice/D3Q19.hpp"
#include "lattice/D3Q27.hpp"
#include "population/collision/collision_bgk.hpp"
#include "population/collision/collision_bgk-s.hpp"
#include "population/collision/collision_bgk_avx2.hpp"
#include "population/collision/collision_trt.hpp"
#include "population/population.hpp"


int main(int argc, char** argv)
{
    /// set up OpenMP ------------------------------------------------------------------------------
    #ifdef _OPENMP
        Parallelism openMP;
        openMP.setThreadsNum(2);
    #endif

    /// print disclaimer ---------------------------------------------------------------------------
    if (argc > 1)
    {
        if ((strcmp(argv[1], "--version") == 0) || (strcmp(argv[1], "--v") == 0))
        {
            printDisclaimer();
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(argv[1], "--convert") == 0)
        {
            std::cerr << "Error: Feature not implemented yet." << std::endl;
            exit(EXIT_FAILURE);
        }
        else if ((strcmp(argv[1], "--info") == 0) || (strcmp(argv[1], "--help") == 0))
        {
            std::cerr << "Usage: '--convert'             Convert *.bin files to *.vtk" << std::endl;
            std::cerr << "       '--help'    or '--info' Show help"                    << std::endl;
            std::cerr << "       '--version' or '--v'    Show build version"           << std::endl;
            exit(EXIT_SUCCESS);
        }
    }

    /// solver settings ---------------------------------------------------------------------------
    // floating point accuracy
    typedef double F_TYPE;

    // lattice
    typedef lattice::D3Q27<F_TYPE> DdQq;

    // spatial and temporal resolution
    constexpr unsigned int NX = 192;
    constexpr unsigned int NY = 96;
    constexpr unsigned int NZ = 96;
    constexpr unsigned int NT = 10000;

    // physics
    constexpr F_TYPE      Re = 1000.0;
    constexpr F_TYPE       U = 0.05;
    constexpr unsigned int L = NY/5;

    // initial conditions
    constexpr F_TYPE RHO_0 = 1.0;
    constexpr F_TYPE   U_0 = U;
    constexpr F_TYPE   V_0 = 0.0;
    constexpr F_TYPE   W_0 = 0.0;

    // save values to disk after each time step (disable for benchmark)
    constexpr bool isSave = true;

    /// set up microscopic and macroscopic arrays --------------------------------------------------
    auto macro = std::make_shared<Continuum<NX,NY,NZ,F_TYPE>>();
    auto micro = std::make_shared<Population<NX,NY,NZ,DdQq>>();

    initialOutput<NX,NY,NZ,DdQq,F_TYPE>(NT, Re, RHO_0, U, L);
    exportParameters<NX,NY,NZ,DdQq,F_TYPE>(NT, Re, RHO_0, U, L);

    /// set up geometry and boundary conditions ----------------------------------------------------
    constexpr unsigned int radius = L/2;
    constexpr std::array<unsigned int,3> position = {NX/4, NY/2, NZ/2};
    auto [inlet, outlet, wall] = cylinder3D(micro, radius, position, "x", true, RHO_0, U_0, V_0, W_0);

    /// set collision operator and initialise domain -----------------------------------------------
    BGK_Smagorinsky collisionOperator {micro, macro, Re, U, L};
    collisionOperator.initialise<timestep::even>(RHO_0, U_0, V_0, W_0);

    /// main simulation loop -----------------------------------------------------------------------
    std::cout << "Simulation started..." << std::endl;

    Timer timer;
    timer.start();

    for (size_t i = 0; i < NT; i+=2)
    {
        // export
        if ((i % (NT/10) == 0) && (isSave == true))
        {
            statusOutput(i, NT, timer.getRuntime());
            macro->setBoundary(wall);
            //macro->exportBin("step",i);
            macro->exportVtk(i);
        }

        // even time step
        inlet.beforeCollisionOperator<timestep::even>();
        outlet.beforeCollisionOperator<timestep::even>();
        collisionOperator.collideStream<timestep::even>(isSave);
        wall.afterCollisionOperator<timestep::even>();

        // odd time step
        inlet.beforeCollisionOperator<timestep::odd>();
        outlet.beforeCollisionOperator<timestep::odd>();
        collisionOperator.collideStream<timestep::odd>(isSave);
        wall.afterCollisionOperator<timestep::odd>();
    }

    /// output performance -------------------------------------------------------------------------
    double const runtime = timer.stop();
    std::cout << runtime << std::endl;

    performanceOutput(macro, micro, NT, NT, runtime);

    /// final export -------------------------------------------------------------------------------
    macro->setBoundary(wall);
    //macro->exportBin("step",NT);
    macro->exportVtk(NT);

    return EXIT_SUCCESS;
}
