#ifndef OUTPUT_HPP_INCLUDED
#define OUTPUT_HPP_INCLUDED

/**
 * \file     output.hpp
 * \brief    Functions for console output
 *
 * \mainpage Functions that output certain information into the console.
*/

#include <memory>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif
#include <cstdio>

#include "../continuum/continuum.hpp"
#include "../population/population.hpp"


/**\fn        initialOutput
 * \brief     Simulation parameters and OpenMP settings output to console.
 *
 * \tparam    NX    Spatial resolution of the simulation domain in x-direction
 * \tparam    NY    Spatial resolution of the simulation domain in y-direction
 * \tparam    NZ    Spatial resolution of the simulation domain in z-direction
 * \tparam    LT    Static lattice::DdQq class containing discretisation parameters
 * \tparam    T     Floating data type used for simulation
 * \param[in] NT    Number of simulation time steps
 * \param[in] Re    Reynolds number of the simulation
 * \param[in] RHO   Simulation density
 * \param[in] U     Characteristic velocity (measurement for temporal resolution)
 * \param[in] L     Characteristic length scale of the problem
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
void initialOutput(unsigned int const NT, T const Re, T const RHO, T const U, unsigned int const L)
{
    std::printf("LBM simulation\n\n");
    std::printf("     domain size: %ux%ux%u\n", NX, NY, NZ);
    std::printf("         lattice: D%uQ%u\n", LT::DIM, LT::SPEEDS);

    std::printf(" Reynolds number: %.2f\n", Re);
    std::printf(" initial density: %.2g\n", RHO);
    std::printf("    char. length: %.2u\n", L);
    std::printf("  char. velocity: %.2g\n", U);
    std::printf("\n");
    std::printf("      #timesteps: %u\n", NT);
    std::printf("\n");
    #ifdef _OPENMP
        std::printf("OpenMP\n");
        std::printf("   #max. threads: %i\n", omp_get_num_procs());
        std::printf("        #threads: %i\n", omp_get_max_threads());
        std::printf("\n");
    #endif

    return;
}

/**\fn    statusOutput
 * \brief Output simulation status and progress in main loop.
 *
 * \param[in] step      The current time step
 * \param[in] NT        Number of time steps
 * \param[in] runtime   Runtime so far in seconds
*/
void statusOutput(unsigned int const step, unsigned int const NT, double const runtime)
{
    std::printf("Time step %u (runtime %.2f s, %.2f%%)\n", step, runtime, 100.0*static_cast<double>(step)/static_cast<double>(NT));

    return;
}

/**\fn        performanceOutput
 * \brief     Output performance benchmark (simulation time, Mlups) at end of
 *            the simulation.
 *
 * \tparam    NX        Spatial resolution of the simulation domain in x-direction
 * \tparam    NY        Spatial resolution of the simulation domain in y-direction
 * \tparam    NZ        Spatial resolution of the simulation domain in z-direction
 * \tparam    LT        Static lattice::DdQq class containing discretisation parameters
 * \tparam    T         Floating data type used for simulation
 * \param[in] con       Continuum object holding macroscopic variables
 * \param[in] pop       Population object holding microscopic variables
 * \param[in] NT        Number of time steps
 * \param[in] NT_PLOT   Time between two plot time steps
 * \param[in] runtime   Simulation runtime in seconds
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
void performanceOutput(std::shared_ptr<Continuum<NX,NY,NZ,T>> const& continuum, std::shared_ptr<Population<NX,NY,NZ,LT>>& population,
                       unsigned int const NT, double NT_PLOT, double const runtime)
{
    constexpr double bytesPerMiB = 1024.0 * 1024.0;
    constexpr double bytesPerGiB = bytesPerMiB * 1024.0;

    size_t const memory = continuum->MEM_SIZE_ + population->MEM_SIZE_;

    unsigned int const  valuesRead = population->SPEEDS_;
    unsigned int const valuesWrite = population->SPEEDS_;
    unsigned int const valuesSaved = continuum->NM_;

    size_t const nodesUpdated = static_cast<size_t>(NT)*NX*NY*static_cast<size_t>(NZ);
    size_t const   nodesSaved = nodesUpdated/NT_PLOT;
    double const        speed = 1e-6*nodesUpdated/runtime;
    double const    bandwidth = (nodesUpdated*(valuesRead + valuesWrite) + nodesSaved*valuesSaved)*sizeof(T) / (runtime*bytesPerGiB);

    std::printf("\nPerformance\n");
    std::printf("   memory allocated: %.1f (MiB)\n", memory/bytesPerMiB);
    std::printf("         #timesteps: %u\n", NT);
    std::printf(" simulation runtime: %.2f (s)\n", runtime);
    std::printf("              speed: %.2f (Mlups)\n", speed);
    std::printf("    ideal bandwidth: %.1f (GiB/s)\n", bandwidth);

    return;
}

#endif // OUTPUT_HPP_INCLUDED
