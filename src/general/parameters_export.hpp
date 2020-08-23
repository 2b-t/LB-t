#ifndef PARAMETERS_EXPORT_HPP_INCLUDED
#define PARAMETERS_EXPORT_HPP_INCLUDED

/**
 * \file     parameters_export.hpp
 * \mainpage Tools for exporting the simulation parameters
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <sys/stat.h>

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
    struct stat info;

    if (stat(OUTPUT_BIN_PATH.c_str(), &info) == 0 && S_ISDIR(info.st_mode))
    {
        std::string const fileName = OUTPUT_BIN_PATH + std::string("/parameters.txt");
        FILE * const exportFile = fopen(fileName.c_str(), "w");

        fprintf(exportFile, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        fprintf(exportFile, "~                           ~\n");
        fprintf(exportFile, "~      LB-t                 ~\n");
        fprintf(exportFile, "~      2019-2020            ~\n");
        fprintf(exportFile, "~      Tobit Flatscher      ~\n");
        fprintf(exportFile, "~      github.com/2b-t      ~\n");
        fprintf(exportFile, "~      Parameter file       ~\n");
        fprintf(exportFile, "~                           ~\n");
        fprintf(exportFile, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        fprintf(exportFile, "\n");
        fprintf(exportFile, "~~~~~Spatial resolution~~~~~~\n");
        fprintf(exportFile, "NX               %u\n", NX);
        fprintf(exportFile, "NY               %u\n", NY);
        fprintf(exportFile, "NZ               %u\n", NZ);
        fprintf(exportFile, "\n");
        fprintf(exportFile, "~~~~~Temporal resolution~~~~~\n");
        fprintf(exportFile, "NT               %u\n", NT);
        fprintf(exportFile, "\n");
        fprintf(exportFile, "~~~~~Physical parameters~~~~~\n");
        fprintf(exportFile, "RE               %.2f\n", static_cast<double>(Re));
        fprintf(exportFile, "RHO_0            %.2f\n", static_cast<double>(RHO_0));
        fprintf(exportFile, "L                %u\n",   L);
        fprintf(exportFile, "U                %.4f\n", static_cast<double>(U));
        fprintf(exportFile, "\n");
        fprintf(exportFile, "~~~~~~~~~~~Lattice~~~~~~~~~~~\n");
        fprintf(exportFile, "lattice          D%uQ%u\n", LT::DIM, LT::SPEEDS);

        fclose(exportFile);
    }
    else
    {
        std::cerr << "Fatal error: Directory '" << OUTPUT_BIN_PATH << "' not found." << std::endl;
        exit(EXIT_FAILURE);
    }

    return;
}

#endif // PARAMETERS_EXPORT_HPP_INCLUDED
