#ifndef CONTINUUM_IMPORT_HPP_INCLUDED
#define CONTINUUM_IMPORT_HPP_INCLUDED

/**
 * \file     continuum_import.hpp
 * \mainpage Class members for importing macroscopic values
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

#include "../general/paths.hpp"


/**\fn         Import
 * \brief      Import macroscopic values from *.bin-file
 *
 * \param[in]  name   the import file name holding the macroscopic quantities
 * \param[in]  step   the current time step that will be used for the name
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::Import(std::string const name, unsigned int const step)
{
    char fileName[256];
    sprintf(fileName, "%s/%s_%u.bin", OUTPUT_BIN_PATH, name, step);

    if(fopen(fileName, "rb+") != nullptr)
    {
        FILE * const importFile = fopen(fileName, "rb+");
        fread(M_, 1, MEM_SIZE_, importFile);
        fclose(importFile);
    }
    else
    {
        std::cerr << "Fatal error: Could not import macroscopic values from disk." << std::endl;
        exit(EXIT_FAILURE);
    }
}

#endif // CONTINUUM_IMPORT_HPP_INCLUDED
