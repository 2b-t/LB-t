/**
 * \file     main.hpp
 * \mainpage This is a generic parallel lattice-Boltzmann Computational Fluids Dynamics (CFD) solver developed and maintained by Tobit Flatscher
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <array>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "continuum/continuum.hpp"
#include "general/disclaimer.hpp"
#include "general/memory_alignment.hpp"
#include "general/openmp_manager.hpp"
#include "general/output.hpp"
#include "general/timer.hpp"
#include "geometry/cylinder.hpp"
#include "geometry/sphere.hpp"
#include "lattice/D2Q9.hpp"
#include "lattice/D3Q15.hpp"
#include "lattice/D3Q19.hpp"
#include "lattice/D3Q27.hpp"
#include "population/collision/collision_bgk.hpp"
#include "population/collision/collision_bgk_avx2.hpp"
#include "population/collision/collision_bgk_avx512.hpp"
#include "population/collision/collision_bgk_smagorinsky.hpp"
#include "population/collision/collision_trt.hpp"
#include "population/population.hpp"
#include "benchmark.hpp"


int main(int argc, char** argv)
{
    /// set up OpenMP ------------------------------------------------------------------------------
    #ifdef _OPENMP
        OpenMpManager& openMpManager = OpenMpManager::getInstance();
        //openMpManager.setThreadsNum(2);
    #endif

    /// print disclaimer ---------------------------------------------------------------------------
    if (argc > 1)
    {
        if ((std::strcmp(argv[1], "--version") == 0) || (std::strcmp(argv[1], "--v") == 0))
        {
            ::printDisclaimer();
            std::exit(EXIT_SUCCESS);
        }
        else if ((std::strcmp(argv[1], "--benchmark") == 0) || (std::strcmp(argv[1], "--b") == 0))
        {
            ::benchmarkAllLatticesAllCollisionOperators();
            std::exit(EXIT_SUCCESS);
        }
        else if ((std::strcmp(argv[1], "--convert") == 0) || (std::strcmp(argv[1], "--c") == 0))
        {
            std::cerr << "Fatal error: Feature not implemented yet." << std::endl;
            std::exit(EXIT_FAILURE);
        }
        else if ((std::strcmp(argv[1], "--help") == 0) || (std::strcmp(argv[1], "--info") == 0))
        {
            std::cout << "Usage:" << std::endl;
            std::cout << "'--benchmark' or '--b'    Benchmark all collision operators" << std::endl;
            std::cout << "'--convert'   or '--c'    Convert *.bin files to *.vtk"      << std::endl;
            std::cout << "'--help'      or '--info' Show help"                         << std::endl;
            std::cout << "'--version'   or '--v'    Show build version"                << std::endl;
            std::exit(EXIT_SUCCESS);
        }
    }

    /// solver settings ---------------------------------------------------------------------------
    // floating point accuracy
    typedef double FType;

    // spatial and temporal resolution
    constexpr unsigned int      NX = 192;
    constexpr unsigned int      NY = 96;
    constexpr unsigned int      NZ = 96;
    constexpr unsigned int      NT = 10000;
    constexpr unsigned int NT_PLOT = 1000;

    // physics
    constexpr FType       Re = 3900.0;
    constexpr FType        U = 0.025;
    constexpr unsigned int L = NY/5;

    // initial conditions
    constexpr FType RHO_0 = 1.0;
    constexpr FType   U_0 = U;
    constexpr FType   V_0 = 0.0;
    constexpr FType   W_0 = 0.0;

    // save values to disk after each time step (disable for benchmark)
    constexpr bool isSave = true;

    /// set up microscopic and macroscopic arrays --------------------------------------------------
    auto macro = std::make_shared<Continuum<NX,NY,NZ,FType>>();
    auto micro = std::make_shared<Population<NX,NY,NZ,lattice::D3Q27,FType>>();

    Output<NX,NY,NZ,lattice::D3Q27,FType> const output(micro, macro, Re, RHO_0, U, L, NT, NT_PLOT);
    output.initialOutput();
    output.exportSettings();

    /// set up geometry and boundary conditions ----------------------------------------------------
    constexpr unsigned int radius = L/2;
    constexpr std::array<unsigned int,3> position = {NX/4, NY/2, NZ/2};
    auto [inlet, outlet, wall] = geometry::cylinder(micro, radius, position, boundary::Orientation::Left, true, RHO_0, U_0, V_0, W_0);
    wall.exportDomainVtk();

    /// set collision operator and initialise domain -----------------------------------------------
    BGK_Smagorinsky collisionOperator {micro, macro, Re, U, L};
    collisionOperator.initialise<timestep::even>(RHO_0, U_0, V_0, W_0);

    /// main simulation loop -----------------------------------------------------------------------
    std::cout << "Simulation started..." << std::endl;

    Timer timer;
    timer.start();

    for (unsigned int i = 0; i < NT; i+=2)
    {
        // status output
        if (i % (NT/10) == 0)
        {
            ::statusOutput(i, NT, timer.getRuntime());
        }

        // even time step
        inlet.beforeCollisionOperator<timestep::even>();
        outlet.beforeCollisionOperator<timestep::even>();
        collisionOperator.collideStream<timestep::even>();
        wall.afterCollisionOperator<timestep::even>();

        bool const isSaveNow = (i % NT_PLOT == 0) && (isSave == true);

        // odd time step
        inlet.beforeCollisionOperator<timestep::odd>();
        outlet.beforeCollisionOperator<timestep::odd>();
        collisionOperator.collideStream<timestep::odd>(isSaveNow);
        wall.afterCollisionOperator<timestep::odd>();

        // export
        if (isSaveNow == true)
        {
            macro->setBoundary(wall);
            //macro->exportBin("step",i);
            macro->exportVtk(i);
        }
    }

    /// output performance -------------------------------------------------------------------------
    double const runtime = timer.stop();
    output.outputPerformance(runtime);

    return EXIT_SUCCESS;
}
