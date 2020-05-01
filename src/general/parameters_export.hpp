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

#include "../general/paths.hpp"


/**\fn        ExportParameters
 * \brief     Export parameters to disk
 *
 * \param[in] NX      spatial resolution of the simulation domain in x-direction
 * \param[in] NY      spatial resolution of the simulation domain in y-direction
 * \param[in] NZ      spatial resolution of the simulation domain in z-direction
 * \param[in] NT      number of simulation time steps
 * \param[in] RE      Reynolds number of the simulation
 * \param[in] RHO_0   simulation density
 * \param[in] L       characteristic length scale of the problem
 * \param[in] U       characteristic velocity (measurement for temporal resolution)
*/
template <typename T>
void ExportParameters(unsigned int const NX, unsigned int const NY, unsigned int const NZ,
                      unsigned int const NT, T const RE, T const RHO_0, unsigned int const L, T const U)
{
    std::string const fileName = OUTPUT_BIN_PATH + std::string("/parameters.txt");
    FILE * const exportFile = fopen(fileName.c_str(), "w");

    fprintf(exportFile, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(exportFile, "~                           ~\n");
    fprintf(exportFile, "~      LB-t                 ~\n");
    fprintf(exportFile, "~      December 2019        ~\n");
    fprintf(exportFile, "~      github.com/2b-t      ~\n");
    fprintf(exportFile, "~      Parameter file       ~\n");
    fprintf(exportFile, "~                           ~\n");
    fprintf(exportFile, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(exportFile, "\n");
    fprintf(exportFile, "~~~~~Spatial resolution~~~~~~\n");
    fprintf(exportFile, "NX     %u\n", NX);
    fprintf(exportFile, "NY     %u\n", NY);
    fprintf(exportFile, "NZ     %u\n", NZ);
    fprintf(exportFile, "\n");
    fprintf(exportFile, "~~~~~Temporal resolution~~~~~\n");
    fprintf(exportFile, "NT     %u\n", NT);
    fprintf(exportFile, "\n");
    fprintf(exportFile, "~~~~~Physical parameters~~~~~\n");
    fprintf(exportFile, "RE     %.2f\n", static_cast<double>(RE));
    fprintf(exportFile, "RHO_0  %.2f\n", static_cast<double>(RHO_0));
    fprintf(exportFile, "L      %u\n",   L);
    fprintf(exportFile, "U      %.4f\n", static_cast<double>(U));
    fprintf(exportFile, "\n");
    fprintf(exportFile, "~~~~~~~Solver settings~~~~~~~\n");
    fprintf(exportFile, "coming soon...");

    fclose(exportFile);
}

#endif // PARAMETERS_EXPORT_HPP_INCLUDED
