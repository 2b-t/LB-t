#ifndef POPULATION_BACKUP_HPP_INCLUDED
#define POPULATION_BACKUP_HPP_INCLUDED

/**
 * \file     population_backup.hpp
 * \mainpage Class members for backing-up (export and import) microscopic populations
*/

#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "../general/paths.hpp"


template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP>
void Population<NX,NY,NZ,LT,NPOP>::importBin(std::string const& name)
{
    std::filesystem::create_directories(BACKUP_EXPORT_PATH);

    std::string const fileName = BACKUP_IMPORT_PATH + std::string("/") + name + std::string(".bin");
    auto const importFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "rb+"), &fclose );

    if(importFile != nullptr)
    {
        std::fread(F_, 1, MEM_SIZE_, importFile.get());
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP>
void Population<NX,NY,NZ,LT,NPOP>::exportBin(std::string const& name) const
{
    std::filesystem::create_directories(BACKUP_EXPORT_PATH);

    std::string const fileName = BACKUP_EXPORT_PATH + std::string("/") + name + std::string(".bin");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "wb+"), &fclose );

    if (exportFile != nullptr)
    {
        std::fwrite(F_, 1, MEM_SIZE_, exportFile.get());
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

#endif //LATTICE_BACKUP_HPP_INCLUDED
