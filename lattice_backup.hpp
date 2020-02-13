#ifndef LATTICE_BACKUP_HPP_INCLUDED
#define LATTICE_BACKUP_HPP_INCLUDED


#include <fstream>
#include <sstream>

#include "paths.hpp"


/**\fn          Import
 * \brief       Import populations from a *.bin file (initialisation)
 * \warning     Please make sure the correct resolution is selected and run the
 *              initialisation of populations afterwards.
 *
 * \param[in]   name   the import file name of the scalar
 */
template <typename T>
void Lattice<T>::Import(char const * name)
{
    char filename[256];
    FILE *importfile;

    sprintf(filename, "%s/%s.bin", pop_import, name);
    if(fopen(filename, "rb+") != nullptr)
    {
        importfile = fopen(filename, "rb+");
        fread(F, 1, MEM_SIZE, importfile);
        fclose(importfile);

    }
    else
    {
        //Error
    }
}

/**\fn          Export
 * \brief       Export populations at current time step to *.bin file
 *
 * \param[in]   name   the export file name of the scalar
 */
template <typename T>
void Lattice<T>::Export(char const * name) const
{
    char filename[256];
    FILE *exportfile;

    sprintf(filename, "%s/%s.bin", pop_export, name);
    exportfile = fopen(filename, "wb+");
    fwrite(F, 1, MEM_SIZE, exportfile);
    fclose(exportfile);
}

template <typename T>
void Lattice<T>::ExportParameters() const
{
    char filename[256];
    sprintf(filename, "%s/parameters.txt", vis_bin);
    FILE *exportfile = fopen(filename, "w");

    fprintf(exportfile, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(exportfile, "~                           ~\n");
    fprintf(exportfile, "~      LBM-t                ~\n");
    fprintf(exportfile, "~      December 2019        ~\n");
    fprintf(exportfile, "~      github.com/2b-t      ~\n");
    fprintf(exportfile, "~      Parameter file       ~\n");
    fprintf(exportfile, "~                           ~\n");
    fprintf(exportfile, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    fprintf(exportfile, "\n");
    fprintf(exportfile, "~~~~~Spatial resolution~~~~~~\n");
    fprintf(exportfile, "NX     %u\n", NX);
    fprintf(exportfile, "NY     %u\n", NY);
    fprintf(exportfile, "NZ     %u\n", NZ);
    fprintf(exportfile, "\n");
    fprintf(exportfile, "~~~~~Temporal resolution~~~~~\n");
    fprintf(exportfile, "NT     %u\n", NT);
    fprintf(exportfile, "\n");
    fprintf(exportfile, "~~~~~Physical parameters~~~~~\n");
    fprintf(exportfile, "RE     %.2f\n", RE);
    fprintf(exportfile, "RHO_0  %.2f\n", RHO_0);
    fprintf(exportfile, "L      %u\n",   L);
    fprintf(exportfile, "U      %.4f\n", U);
    fprintf(exportfile, "\n");
    fprintf(exportfile, "~~~~~~~Solver settings~~~~~~~\n");
    fprintf(exportfile, "coming soon...");

    fclose(exportfile);
}

#endif //LATTICE_BACKUP_HPP_INCLUDED
