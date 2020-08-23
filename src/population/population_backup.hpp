#ifndef POPULATION_BACKUP_HPP_INCLUDED
#define POPULATION_BACKUP_HPP_INCLUDED

/**
 * \file     population_backup.hpp
 * \mainpage Class members for backing-up (export and import) microscopic populations
*/

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string.h>

#include "../general/paths.hpp"


template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP>
void Population<NX,NY,NZ,LT,NPOP>::importBin(std::string const& name)
{
    std::string const fileName = BACKUP_IMPORT_PATH + std::string("/") + name + std::string(".bin");

    if(auto const importFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "rb+"), &fclose ) != nullptr)
    {
        fread(F_, 1, MEM_SIZE_, importFile);
    }
    else
    {
        std::cerr << "Error: Could not import population from disk." << std::endl;
        exit(EXIT_FAILURE);
    }

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP>
void Population<NX,NY,NZ,LT,NPOP>::exportBin(std::string const& name) const
{
    struct stat info;

    if (stat(BACKUP_EXPORT_PATH.c_str(), &info) == 0 && S_ISDIR(info.st_mode))
    {
        std::string const fileName = BACKUP_EXPORT_PATH + std::string("/") + name + std::string(".bin");
        auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "wb+"), &fclose );

        fwrite(F_, 1, MEM_SIZE_, exportFile);
    }
    else
    {
        std::cerr << "Fatal error: Directory '" << BACKUP_EXPORT_PATH << "' not found." << std::endl;
        exit(EXIT_FAILURE);
    }

    return;
}

#endif //LATTICE_BACKUP_HPP_INCLUDED
