#ifndef PARAMETERS_EXPORT_HPP_INCLUDED
#define PARAMETERS_EXPORT_HPP_INCLUDED

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
    std::filesystem::create_directory(OUTPUT_BIN_PATH);

    std::string const fileName = OUTPUT_BIN_PATH + std::string("/parameters.txt");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "w"), &fclose );

    if(exportFile != nullptr) {
        fprintf(exportFile.get(), "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        fprintf(exportFile.get(), "~                           ~\n");
        fprintf(exportFile.get(), "~      LB-t                 ~\n");
        fprintf(exportFile.get(), "~      2019-2020            ~\n");
        fprintf(exportFile.get(), "~      Tobit Flatscher      ~\n");
        fprintf(exportFile.get(), "~      github.com/2b-t      ~\n");
        fprintf(exportFile.get(), "~      Parameter file       ~\n");
        fprintf(exportFile.get(), "~                           ~\n");
        fprintf(exportFile.get(), "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        fprintf(exportFile.get(), "\n");
        fprintf(exportFile.get(), "~~~~~Spatial resolution~~~~~~\n");
        fprintf(exportFile.get(), "NX               %u\n", NX);
        fprintf(exportFile.get(), "NY               %u\n", NY);
        fprintf(exportFile.get(), "NZ               %u\n", NZ);
        fprintf(exportFile.get(), "\n");
        fprintf(exportFile.get(), "~~~~~Temporal resolution~~~~~\n");
        fprintf(exportFile.get(), "NT               %u\n", NT);
        fprintf(exportFile.get(), "\n");
        fprintf(exportFile.get(), "~~~~~Physical parameters~~~~~\n");
        fprintf(exportFile.get(), "RE               %.2f\n", static_cast<double>(Re));
        fprintf(exportFile.get(), "RHO_0            %.2f\n", static_cast<double>(RHO_0));
        fprintf(exportFile.get(), "L                %u\n",   L);
        fprintf(exportFile.get(), "U                %.4f\n", static_cast<double>(U));
        fprintf(exportFile.get(), "\n");
        fprintf(exportFile.get(), "~~~~~~~~~~~Lattice~~~~~~~~~~~\n");
        fprintf(exportFile.get(), "lattice          D%uQ%u\n", LT::DIM, LT::SPEEDS);
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

#endif // PARAMETERS_EXPORT_HPP_INCLUDED
