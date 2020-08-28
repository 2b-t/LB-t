#ifndef LBT_PARAMETERS_EXPORT
#define LBT_PARAMETERS_EXPORT

/**
 * \file     parameters_export.hpp
 * \mainpage Tools for exporting the simulation parameters
*/

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "../general/paths.hpp"
#include "../population/population.hpp"


/**\fn        exportParameters
 * \brief     Export parameters to disk
 *
 * \tparam    NX      Spatial resolution of the simulation domain in x-direction
 * \tparam    NY      Spatial resolution of the simulation domain in y-direction
 * \tparam    NZ      Spatial resolution of the simulation domain in z-direction
 * \tparam    LT      Static lattice::DdQq class containing discretisation parameters
 * \tparam    T       Floating data type used for simulation
 * \param[in] NT      Number of simulation time steps
 * \param[in] Re      Reynolds number of the simulation
 * \param[in] RHO_0   Simulation density
 * \param[in] L       Characteristic length scale of the problem
 * \param[in] U       Characteristic velocity (measurement for temporal resolution)
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, typename T>
void exportParameters(unsigned int const NT, T const Re, T const RHO_0, T const U, unsigned int const L)
{
    std::filesystem::create_directories(OUTPUT_BIN_PATH);

    std::string const fileName = OUTPUT_BIN_PATH + std::string("/parameters.txt");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "w"), &fclose );

    if(exportFile != nullptr) {
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
        std::fprintf(exportFile.get(), "NT               %u\n", NT);
        std::fprintf(exportFile.get(), "\n");
        std::fprintf(exportFile.get(), "~~~~~Physical parameters~~~~~\n");
        std::fprintf(exportFile.get(), "RE               %.2f\n", static_cast<double>(Re));
        std::fprintf(exportFile.get(), "RHO_0            %.2f\n", static_cast<double>(RHO_0));
        std::fprintf(exportFile.get(), "L                %u\n",   L);
        std::fprintf(exportFile.get(), "U                %.4f\n", static_cast<double>(U));
        std::fprintf(exportFile.get(), "\n");
        std::fprintf(exportFile.get(), "~~~~~~~~~~~Lattice~~~~~~~~~~~\n");
        std::fprintf(exportFile.get(), "lattice          D%uQ%u\n", LT::DIM, LT::SPEEDS);
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

#endif // LBT_PARAMETERS_EXPORT
