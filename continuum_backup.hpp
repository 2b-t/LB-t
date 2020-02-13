#ifndef CONTINUUM_BACKUP_HPP_INCLUDED
#define CONTINUUM_BACKUP_HPP_INCLUDED


#include <fstream>
#include <sstream>

#include "paths.hpp"


/**\fn         Import
 * \brief      Import macroscopic values from *.bin-file
 *
 * \param[in]  name   the export file name of the scalar
 * \param[in]  step   the current time step that will be used for the name
*/
template <typename T>
void Continuum<T>::Import(char const * name, unsigned int const step)
{
    char filename[256];
    sprintf(filename, "%s/%s_%u.bin", vis_bin, name, step);
    if(fopen(filename, "rb+") != NULL)
    {
        FILE *importfile = fopen(filename, "rb+");
        fread(M, 1, MEM_SIZE, importfile);
        fclose(importfile);
    }
    else
    {
        //Error
    }
}

#endif //CONTINUUM_BACKUP_HPP_INCLUDED
