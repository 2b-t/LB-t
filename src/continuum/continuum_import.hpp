#ifndef LBT_CONTINUUM_IMPORT
#define LBT_CONTINUUM_IMPORT

/**
 * \file     continuum_import.hpp
 * \mainpage Class members for importing macroscopic values
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "../general/paths.hpp"


template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::importBin(std::string const& name, unsigned int const step) noexcept
{
    std::filesystem::create_directories(path::outputBin);

    std::string const fileName = path::outputBin + std::string("/") + name + std::string("_") + std::to_string(step) + std::string(".bin");
    auto const importFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "rb+"), &fclose );

    if(importFile != nullptr)
    {
        std::fread(M, 1, memorySize, importFile.get());
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

#endif // LBT_CONTINUUM_IMPORT
