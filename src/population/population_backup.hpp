#ifndef LBT_POPULATION_BACKUP
#define LBT_POPULATION_BACKUP

/**
 * \file     population_backup.hpp
 * \mainpage Class members for backing-up (export and import) microscopic populations
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "../general/paths.hpp"


template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP>
void Population<NX,NY,NZ,LT,T,NPOP>::importBin(std::string const& name) noexcept
{
    std::string const fileName = path::backupImport + std::string("/") + name + std::string(".bin");
    auto const importFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "rb+"), &fclose );

    if(importFile != nullptr)
    {
        std::fread(A, 1, memorySize, importFile.get());
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP>
void Population<NX,NY,NZ,LT,T,NPOP>::exportBin(std::string const& name) const noexcept
{
    std::filesystem::create_directories(path::backupExport);

    std::string const fileName = path::backupExport + std::string("/") + name + std::string(".bin");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "wb+"), &fclose );

    if (exportFile != nullptr)
    {
        std::fwrite(A, 1, memorySize, exportFile.get());
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

#endif // LBT_POPULATION_BACKUP
