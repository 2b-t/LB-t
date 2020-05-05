#ifndef POPULATION_BACKUP_HPP_INCLUDED
#define POPULATION_BACKUP_HPP_INCLUDED

/**
 * \file     population_backup.hpp
 * \mainpage Class members for backing-up (export and import) microscopic populations
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

#include "../general/paths.hpp"


/**\fn          Import
 * \brief       Import populations from a *.bin file (initialisation)
 * \warning     Please make sure the correct resolution is selected and run the
 *              initialisation of populations afterwards.
 *
 * \param[in]   name   the import file name of the scalar
 */
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP>
void Population<NX,NY,NZ,LT,NPOP>::Import(std::string const name)
{
    std::string const fileName = BACKUP_IMPORT_PATH + std::string("/") + name + std::string(".bin");
    FILE * const importFile;

    if(fopen(fileName.c_str(), "rb+") != nullptr)
    {
        importFile = fopen(fileName.c_str(), "rb+");
        fread(F_, 1, MEM_SIZE_, importFile);
        fclose(importFile);

    }
    else
    {
        std::cerr << "Error: Could not import population from disk." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**\fn          Export
 * \brief       Export populations at current time step to *.bin file
 *
 * \param[in]   name   the export file name of the scalar
 */
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP>
void Population<NX,NY,NZ,LT,NPOP>::Export(std::string const name) const
{
    std::string const fileName = BACKUP_EXPORT_PATH + std::string("/") + name + std::string(".bin");
    FILE * const exportFile;

    exportFile = fopen(fileName.c_str(), "wb+");
    fwrite(F_, 1, MEM_SIZE_, exportFile);
    fclose(exportFile);
}

#endif //LATTICE_BACKUP_HPP_INCLUDED
