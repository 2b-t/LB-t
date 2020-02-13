#ifndef CONTINUUM_EXPORT_HPP_INCLUDED
#define CONTINUUM_EXPORT_HPP_INCLUDED


#include <vector>
#include <fstream>
#include <sstream>

#include "paths.hpp"
#include "boundary.hpp"


/**\fn        SetZero
 * \brief     Set not relevant nodes (e.g. boundary nodes) of a the hydrodynamic
 *            field (velocities and density) to zero
 *
 * \param[in] boundary   the indices of all solid cells
*/
template <typename T>
void Continuum<T>::SetZero(std::vector<boundary_element<T>>& boundary)
{
    for(size_t i = 0; i < boundary.size(); ++i)
    {
        for (size_t m = 0; m < NM; ++m)
        {
            M[SpatialToLinear(boundary[i].x, boundary[i].y, boundary[i].z, m)] = static_cast<T>(0.0);
        }
    }
}


/**\fn        Export
 * \brief     Export any scalar quantity at current time step to *.bin file writing to *.bin-files
 *            is significantly faster than using non-binary *.txt-files
 *
 * \param[in] name   the export file name of the scalar
 * \param[in] step   the current time step that will be used for the name
*/
template <typename T>
void Continuum<T>::Export(char const * name, unsigned int const step) const
{
    char filename[256];
    sprintf(filename, "%s/%s_%u.bin", vis_bin, name, step);
    FILE *exportfile = fopen(filename, "wb+");
    fwrite(M, 1, MEM_SIZE, exportfile);
    fclose(exportfile);
}


/**\fn        ExportScalarVtk
 * \brief     Export arbitrary scalar at current time step to a *.vtk-file that can then be read
 *            by visualisation applications like ParaView.
 * \warning   *.vtk export is comparably slow! Better export to .bin-files and then convert them
 *            to *.vtk-files with the external converter app afterwards.
 *
 * \param[in] name   the export file name of the scalar
 * \param[in] step   the current time step that will be used for the name
*/
template <typename T>
void Continuum<T>::ExportScalarVtk(unsigned int const m, char const * name, unsigned int const step) const
{
    char filename[256];
    sprintf(filename, "%s/%s_%u.vtk", vis_vtk, name, step);
    FILE *exportfile = fopen(filename, "w");

    fprintf(exportfile, "# vtk DataFile Version 3.0\n");
    fprintf(exportfile, "LBM CFD simulation scalar\n");
    fprintf(exportfile, "ASCII\n");
    fprintf(exportfile, "DATASET RECTILINEAR_GRID\n");

    fprintf(exportfile, "DIMENSIONS %u %u %u\n", NX, NY, NZ);
    fprintf(exportfile, "X_COORDINATES %u float\n", NX);
    for(unsigned int x = 0; x < NX; ++x)
    {
        fprintf(exportfile,"%u ", x);
    }
    fprintf(exportfile, "\n");
    fprintf(exportfile, "Y_COORDINATES %u float\n", NY);
    for(unsigned int y = 0; y < NY; ++y)
    {
        fprintf(exportfile,"%u ", y);
    }
    fprintf(exportfile, "\n");
    fprintf(exportfile, "Z_COORDINATES %u float\n", NZ);
    for(unsigned int z = 0; z < NZ; ++z)
    {
        fprintf(exportfile, "%u ", z);
    }
    fprintf(exportfile, "\n");

    size_t const no_cells = static_cast<size_t>(NX)*static_cast<size_t>(NY)*static_cast<size_t>(NZ);
    fprintf(exportfile, "POINT_DATA %zu\n", no_cells);

    fprintf(exportfile, "SCALARS transport_scalar float 1\n");
    fprintf(exportfile, "LOOKUP_TABLE default\n");
    for(unsigned int z = 0; z < NZ; ++z)
    {
        for(unsigned int y = 0; y < NY; ++y)
        {
            for(unsigned int x = 0; x < NX; ++x)
            {
                fprintf(exportfile, "%f\n", M[SpatialToLinear(x, y, z, m)]);
            }
        }
    }

    fclose(exportfile);
}

/**\fn        ExportVtk
 * \brief     Export velocity and density at current time step to a *.vtk-file that can then be
 *            read by visualisation applications like ParaView.
 * \warning   *.vtk export is comparably slow! Better export to .bin-files and then convert them
 *            to *.vtk-files with the external converter app afterwards.
 *
 * \param[in] step   the current time step that will be used for the name
*/
template <typename T>
void Continuum<T>::ExportVtk(unsigned int const step) const
{
    char filename[256];
    sprintf(filename, "%s/Export_%u.vtk", vis_vtk, step);
    FILE *exportfile = fopen(filename, "w");

    fprintf(exportfile, "# vtk DataFile Version 3.0\n");
    fprintf(exportfile, "LBM CFD simulation velocity\n");
    fprintf(exportfile, "ASCII\n");
    fprintf(exportfile, "DATASET RECTILINEAR_GRID\n");

    fprintf(exportfile, "DIMENSIONS %u %u %u\n", NX, NY, NZ);
    fprintf(exportfile, "X_COORDINATES %u float\n", NX);
    for(unsigned int x = 0; x < NX; ++x)
    {
        fprintf(exportfile,"%u ", x);
    }
    fprintf(exportfile, "\n");
    fprintf(exportfile, "Y_COORDINATES %u float\n", NY);
    for(unsigned int y = 0; y < NY; ++y)
    {
        fprintf(exportfile,"%u ", y);
    }
    fprintf(exportfile, "\n");
    fprintf(exportfile, "Z_COORDINATES %u float\n", NZ);
    for(unsigned int z = 0; z < NZ; ++z)
    {
        fprintf(exportfile, "%u ", z);
    }
    fprintf(exportfile, "\n");

    size_t const no_cells = static_cast<size_t>(NX)*static_cast<size_t>(NY)*static_cast<size_t>(NZ);
    fprintf(exportfile, "POINT_DATA %zu\n", no_cells);

    fprintf(exportfile, "SCALARS density_variation float 1\n");
    fprintf(exportfile, "LOOKUP_TABLE default\n");
    for(unsigned int z = 0; z < NZ; ++z)
    {
        for(unsigned int y = 0; y < NY; ++y)
        {
            for(unsigned int x = 0; x < NX; ++x)
            {
                fprintf(exportfile, "%f\n", M[SpatialToLinear(x, y, z, 0)]);
            }
        }
    }

    fprintf(exportfile, "VECTORS velocity_vector float\n");
    for(unsigned int z = 0; z < NZ; ++z)
    {
        for(unsigned int y = 0; y < NY; ++y)
        {
            for(unsigned int x = 0; x < NX; ++x)
            {
                fprintf(exportfile, "%f %f %f\n", M[SpatialToLinear(x, y, z, 1)],
                                                  M[SpatialToLinear(x, y, z, 2)],
                                                  M[SpatialToLinear(x, y, z, 3)]);
            }
        }
    }

    fclose(exportfile);
}

#endif //CONTINUUM_EXPORT_HPP_INCLUDED
