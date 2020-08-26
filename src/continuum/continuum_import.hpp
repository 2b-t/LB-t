#ifndef CONTINUUM_IMPORT_HPP_INCLUDED
#define CONTINUUM_IMPORT_HPP_INCLUDED

/**
 * \file     continuum_import.hpp
 * \mainpage Class members for importing macroscopic values
*/

#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "../general/paths.hpp"


template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::importBin(std::string const& name, unsigned int const step)
{
    std::filesystem::create_directories(OUTPUT_BIN_PATH);

    std::string const fileName = OUTPUT_BIN_PATH + std::string("/") + name + std::string("_") + std::to_string(step) + std::string(".bin");
    auto const importFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "rb+"), &fclose );

    if(importFile != nullptr)
    {
        fread(M_, 1, MEM_SIZE_, importFile.get());
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

#endif // CONTINUUM_IMPORT_HPP_INCLUDED
