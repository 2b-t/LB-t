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


/**\fn        InitialOutput
 * \brief     Simulation parameters and OpenMP settings output to console.
 *
 * \param[in] SPEEDS: number of discrete lattice velocities
 * \param[in] tau: hydrodynamic relaxation time
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, unsigned int NT, class LT, class POP>
void InitialOutput(POP const& micro,
                   double const RE, double const RHO_0, double const U, unsigned int const L)
{
    printf("LBM simulation\n\n");
    printf("     domain size: %ux%ux%u\n", NX, NY, NZ);
    printf("         lattice: D%uQ%u\n", LT::DIM, LT::SPEEDS);

    printf(" Reynolds number: %.2f\n", RE);
    printf(" initial density: %.2g\n", RHO_0);
    printf("    char. length: %.2u\n", L);
    printf("  char. velocity: %.2g\n", U);
    printf("\n");
    printf("  kin. viscosity: %.2g\n", micro.NU_);
    printf(" relaxation time: %.2g\n", micro.TAU_);
    printf("\n");
    printf("      #timesteps: %u\n", NT);
    printf("\n");
    printf("OpenMP\n");
    printf("   #max. threads: %i\n", omp_get_num_procs());
    printf("        #threads: %i\n", omp_get_max_threads());
    printf("\n");
}

/**\fn        StatusOutput
 * \brief     Output simulation status and progress in main loop.
 *
 * \param[in] step: the current time step
*/
template <unsigned int NT>
void StatusOutput(unsigned int const step)
{
    printf("Time step %u (%.2f%%)\n", step, 100.0*step/NT);
}

/**\fn        PerformanceOutput
 * \brief     Output performance benchmark (simulation time, Mlups) at end of
 *            the simulation.
 *
 * \param[in] MEMORY: allocated memory in bytes
 * \param[in] runtime: simulation runtime (main loop only!)
 * \param[in] SPEEDS: number of discrete lattice velocities
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, unsigned int NT, class CON, class POP>
void PerformanceOutput(CON const& macro, POP const& micro, double NT_PLOT, double const runtime)
{
    typedef typename std::remove_const<decltype(micro.NU_)>::type T;

    constexpr double bytesPerMiB = 1024.0 * 1024.0;
    constexpr double bytesPerGiB = bytesPerMiB * 1024.0;

    size_t const memory = macro.MEM_SIZE_ + micro.MEM_SIZE_;

    unsigned int const  valuesRead = micro.SPEEDS_;
    unsigned int const valuesWrite = micro.SPEEDS_;
    unsigned int const valuesSaved = macro.NM_;

    size_t const nodesUpdated = static_cast<size_t>(NT)*static_cast<size_t>(NX)*static_cast<size_t>(NY)*static_cast<size_t>(NZ);
    size_t const   nodesSaved = nodesUpdated/NT_PLOT;
    double const        speed = 1e-6*nodesUpdated/runtime;

    double const bandwidth = (nodesUpdated*(valuesRead + valuesWrite) + nodesSaved*valuesSaved)*sizeof(T) / (runtime*bytesPerGiB);

    printf("\nPerformance\n");
    printf("   memory allocated: %.1f (MiB)\n", memory/bytesPerMiB);
    printf("         #timesteps: %u\n", NT);
    printf(" simulation runtime: %.2f (s)\n", runtime);
    printf("              speed: %.2f (Mlups)\n", speed);
    printf("    ideal bandwidth: %.1f (GiB/s)\n", bandwidth);
}

#endif // OUTPUT_HPP_INCLUDED
