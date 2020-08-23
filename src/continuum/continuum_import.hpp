#ifndef CONTINUUM_IMPORT_HPP_INCLUDED
#define CONTINUUM_IMPORT_HPP_INCLUDED

/**
 * \file     continuum_import.hpp
 * \mainpage Class members for importing macroscopic values
*/

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string.h>

#include "../general/paths.hpp"


template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::importBin(std::string const& name, unsigned int const step)
{
    std::string const fileName = OUTPUT_BIN_PATH + std::string("/") + name + std::string("_") + std::to_string(step) + std::string(".bin");

    if(auto const importFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "rb+"), &fclose ) != nullptr)
    {
        fread(M_, 1, MEM_SIZE_, importFile);
    }
    else
    {
        std::cerr << "Fatal error: Could not import macroscopic values from disk." << std::endl;
        exit(EXIT_FAILURE);
    }

    return;
}

#endif // CONTINUUM_IMPORT_HPP_INCLUDED
