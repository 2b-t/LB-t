#ifndef LBT_OUTPUT
#define LBT_OUTPUT

/**
 * \file     output.hpp
 * \brief    Functions for console output
 *
 * \mainpage Functions that output certain information into the console.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdio>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif
#include <sstream>
#include <string>

#include "../continuum/continuum.hpp"
#include "../general/paths.hpp"
#include "../population/population.hpp"


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


/**\class  Output
 * \brief  General class for output to console or export of settings to files
 *
 * \tparam NX             Simulation domain resolution in x-direction
 * \tparam NY             Simulation domain resolution in y-direction
 * \tparam NZ             Simulation domain resolution in z-direction
 * \tparam LT             Static lattice::DdQq class containing discretisation parameters
 * \tparam T              Floating data type used for simulation
 * \tparam NPOP           Number of populations stored side by side in a single merged grid
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP>
class Output
{
    public:
        /**\brief     Class constructor
         * 
         * \param[in] population   Population object holding microscopic distributions
         * \param[in] continuum    Continuum object holding macroscopic variables
         * \param[in] Re           Reynolds number of the simulation
         * \param[in] RHO          Simulation density
         * \param[in] U            Characteristic velocity (measurement for temporal resolution)
         * \param[in] L            Characteristic length scale of the problem
         * \param[in] NT           Number of simulation time steps
         * \param[in] NT_PLOT      Number of time steps after which a plot step occurs
        */
        Output(std::shared_ptr<Population<NX,NY,NZ,LT,T,NPOP>> population, std::shared_ptr<Continuum<NX,NY,NZ,T>> continuum,
               T const Re, T const RHO, T const U, unsigned int const L, unsigned int const NT, unsigned int const NT_PLOT):
          population_(population), continuum_(continuum), Re_(Re), RHO_(RHO), U_(U), L_(L), NT_(NT), NT_PLOT_(NT_PLOT)
        {
           return; 
        }

        /**\fn        initialOutput
         * \brief     Simulation parameters and OpenMP settings output to console.
        */
        void initialOutput() const;

        /**\fn        outputPerformance
         * \brief     Output performance benchmark (simulation time, Mlups) at end of
         *            the simulation.
         *
         * \param[in] runtime   Simulation runtime in seconds
        */
        void outputPerformance(double const runtime) const;
        
        /**\fn        exportSettings
         * \brief     Export parameters to disk
        */
        void exportSettings() const;

    private:
        std::shared_ptr<Population<NX,NY,NZ,LT,T,NPOP>> population_;
        std::shared_ptr<Continuum<NX,NY,NZ,T>>          continuum_;
        T Re_;
        T RHO_;
        T U_;
        unsigned int L_;
        unsigned int NT_; 
        unsigned int NT_PLOT_;
};


template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP>
void Output<NX,NY,NZ,LT,T,NPOP>::initialOutput() const
{
    std::printf("LBM simulation\n\n");
    std::printf("     domain size: %ux%ux%u\n", NX, NY, NZ);
    std::printf("         lattice: D%uQ%u\n", LT<T>::DIM, LT<T>::SPEEDS);

    std::printf(" Reynolds number: %.2f\n", Re_);
    std::printf(" initial density: %.2g\n", RHO_);
    std::printf("    char. length: %.2u\n", L_);
    std::printf("  char. velocity: %.2g\n", U_);
    std::printf("\n");
    std::printf("      #timesteps: %u\n", NT_);
    std::printf("\n");
    #ifdef _OPENMP
        std::printf("OpenMP\n");
        std::printf("   #max. threads: %i\n", omp_get_num_procs());
        std::printf("        #threads: %i\n", omp_get_max_threads());
        std::printf("\n");
    #endif

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP>
void Output<NX,NY,NZ,LT,T,NPOP>::outputPerformance(double const runtime) const
{
    constexpr double bytesPerMiB = 1024.0 * 1024.0;
    constexpr double bytesPerGiB = bytesPerMiB * 1024.0;

    size_t const memory = continuum_->MEM_SIZE_ + population_->MEM_SIZE_;

    unsigned int const  valuesRead = LT<T>::SPEEDS;
    unsigned int const valuesWrite = LT<T>::SPEEDS;
    unsigned int const valuesSaved = continuum_->NM_;

    size_t const nodesUpdated = static_cast<size_t>(NT_)*NX*NY*static_cast<size_t>(NZ);
    size_t const   nodesSaved = nodesUpdated/NT_PLOT_;
    double const        speed = 1e-6*nodesUpdated/runtime;
    double const    bandwidth = (nodesUpdated*(valuesRead + valuesWrite) + nodesSaved*valuesSaved)*sizeof(T) / (runtime*bytesPerGiB);

    std::printf("\nPerformance\n");
    std::printf("   memory allocated: %.1f (MiB)\n", memory/bytesPerMiB);
    std::printf("         #timesteps: %u\n", NT_);
    std::printf(" simulation runtime: %.2f (s)\n", runtime);
    std::printf("              speed: %.2f (Mlups)\n", speed);
    std::printf("    ideal bandwidth: %.1f (GiB/s)\n", bandwidth);

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP>
void Output<NX,NY,NZ,LT,T,NPOP>::exportSettings() const
{
    std::filesystem::create_directories(OUTPUT_BIN_PATH);

    std::string const fileName = OUTPUT_BIN_PATH + std::string("/parameters.txt");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "w"), &fclose );

    if(exportFile != nullptr)
    {
        std::fprintf(exportFile.get(), "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        std::fprintf(exportFile.get(), "~                           ~\n");
        std::fprintf(exportFile.get(), "~      LB-t                 ~\n");
        std::fprintf(exportFile.get(), "~      2019-2020            ~\n");
        std::fprintf(exportFile.get(), "~      Tobit Flatscher      ~\n");
        std::fprintf(exportFile.get(), "~      github.com/2b-t      ~\n");
        std::fprintf(exportFile.get(), "~      Parameter file       ~\n");
        std::fprintf(exportFile.get(), "~                           ~\n");
        std::fprintf(exportFile.get(), "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        std::fprintf(exportFile.get(), "\n");
        std::fprintf(exportFile.get(), "~~~~~Spatial resolution~~~~~~\n");
        std::fprintf(exportFile.get(), "NX               %u\n", NX);
        std::fprintf(exportFile.get(), "NY               %u\n", NY);
        std::fprintf(exportFile.get(), "NZ               %u\n", NZ);
        std::fprintf(exportFile.get(), "\n");
        std::fprintf(exportFile.get(), "~~~~~Temporal resolution~~~~~\n");
        std::fprintf(exportFile.get(), "NT               %u\n", NT_);
        std::fprintf(exportFile.get(), "\n");
        std::fprintf(exportFile.get(), "~~~~~Physical parameters~~~~~\n");
        std::fprintf(exportFile.get(), "RE               %.2f\n", static_cast<double>(Re_));
        std::fprintf(exportFile.get(), "RHO_0            %.2f\n", static_cast<double>(RHO_));
        std::fprintf(exportFile.get(), "L                %u\n",   L_);
        std::fprintf(exportFile.get(), "U                %.4f\n", static_cast<double>(U_));
        std::fprintf(exportFile.get(), "\n");
        std::fprintf(exportFile.get(), "~~~~~~~~~~~Lattice~~~~~~~~~~~\n");
        std::fprintf(exportFile.get(), "lattice          D%uQ%u\n", LT<T>::DIM, LT<T>::SPEEDS);
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

#endif // LBT_OUTPUT
