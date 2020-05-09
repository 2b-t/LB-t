#ifndef OUTPUT_HPP_INCLUDED
#define OUTPUT_HPP_INCLUDED

/**
 * \file     output.hpp
 * \brief    Functions for console output
 *
 * \mainpage Functions that output certain information into the console.
*/

#if __has_include (<omp.h>)
    #include <omp.h>
#endif
#include <stdio.h>

#include "../continuum/continuum.hpp"
#include "../population/population.hpp"


/**\fn        InitialOutput
 * \brief     Simulation parameters and OpenMP settings output to console.
 *
 * \tparam    NX    spatial resolution of the simulation domain in x-direction
 * \tparam    NY    spatial resolution of the simulation domain in y-direction
 * \tparam    NZ    spatial resolution of the simulation domain in z-direction
 * \tparam    LT    static lattice::DdQq class containing discretisation parameters
 * \tparam    T     floating data type used for simulation
 * \param[in] pop   population object holding microscopic variables
 * \param[in] NT    number of simulation time steps
 * \param[in] Re    Reynolds number of the simulation
 * \param[in] RHO   simulation density
 * \param[in] U     characteristic velocity (measurement for temporal resolution)
 * \param[in] L     characteristic length scale of the problem
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
void InitialOutput(Population<NX,NY,NZ,LT> const& pop, unsigned int const NT,
                   T const Re, T const RHO, T const U, unsigned int const L)
{
    printf("LBM simulation\n\n");
    printf("     domain size: %ux%ux%u\n", NX, NY, NZ);
    printf("         lattice: D%uQ%u\n", LT::DIM, LT::SPEEDS);

    printf(" Reynolds number: %.2f\n", Re);
    printf(" initial density: %.2g\n", RHO);
    printf("    char. length: %.2u\n", L);
    printf("  char. velocity: %.2g\n", U);
    printf("\n");
    printf("  kin. viscosity: %.2g\n", static_cast<double>(pop.NU_));
    printf(" relaxation time: %.2g\n", static_cast<double>(pop.TAU_));
    printf("\n");
    printf("      #timesteps: %u\n", NT);
    printf("\n");
    #ifdef _OPENMP
        printf("OpenMP\n");
        printf("   #max. threads: %i\n", omp_get_num_procs());
        printf("        #threads: %i\n", omp_get_max_threads());
        printf("\n");
    #endif
}

/**\fn        StatusOutput
 * \brief     Output simulation status and progress in main loop.
 *
 * \param[in] step: the current time step
*/
void StatusOutput(unsigned int const step, unsigned int NT)
{
    printf("Time step %u (%.2f%%)\n", step, 100.0*step/NT);
}

/**\fn        PerformanceOutput
 * \brief     Output performance benchmark (simulation time, Mlups) at end of
 *            the simulation.
 *
 * \tparam    NX        spatial resolution of the simulation domain in x-direction
 * \tparam    NY        spatial resolution of the simulation domain in y-direction
 * \tparam    NZ        spatial resolution of the simulation domain in z-direction
 * \tparam    LT        static lattice::DdQq class containing discretisation parameters
 * \tparam    T         floating data type used for simulation
 * \param[in] con       continuum object holding macroscopic variables
 * \param[in] pop       population object holding microscopic variables
 * \param[in] NT        number of time steps
 * \param[in] NT_PLOT   time between two plot time steps
 * \param[in] runtime   simulation runtime in seconds
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
void PerformanceOutput(Continuum<NX,NY,NZ,T> const& con, Population<NX,NY,NZ,LT>& pop, unsigned int const NT, double NT_PLOT, double const runtime)
{
    constexpr double bytesPerMiB = 1024.0 * 1024.0;
    constexpr double bytesPerGiB = bytesPerMiB * 1024.0;

    size_t const memory = con.MEM_SIZE_ + pop.MEM_SIZE_;

    unsigned int const  valuesRead = pop.SPEEDS_;
    unsigned int const valuesWrite = pop.SPEEDS_;
    unsigned int const valuesSaved = con.NM_;

    size_t const nodesUpdated = static_cast<size_t>(NT)*NX*NY*static_cast<size_t>(NZ);
    size_t const   nodesSaved = nodesUpdated/NT_PLOT;
    double const        speed = 1e-6*nodesUpdated/runtime;
    double const    bandwidth = (nodesUpdated*(valuesRead + valuesWrite) + nodesSaved*valuesSaved)*sizeof(T) / (runtime*bytesPerGiB);

    printf("\nPerformance\n");
    printf("   memory allocated: %.1f (MiB)\n", memory/bytesPerMiB);
    printf("         #timesteps: %u\n", NT);
    printf(" simulation runtime: %.2f (s)\n", runtime);
    printf("              speed: %.2f (Mlups)\n", speed);
    printf("    ideal bandwidth: %.1f (GiB/s)\n", bandwidth);
}

#endif // OUTPUT_HPP_INCLUDED
