#ifndef CONTINUUM_EXPORT_HPP_INCLUDED
#define CONTINUUM_EXPORT_HPP_INCLUDED

/**
 * \file     continuum_export.hpp
 * \mainpage Class members for exporting macroscopic values
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "../population/boundary/boundary.hpp"
#include "../general/paths.hpp"


/**\fn        SetZero
 * \brief     Set not relevant nodes (e.g. boundary nodes) of a the hydrodynamic
 *            field (velocities and density) to zero
 *
 * \param[in] boundary   the indices of all solid cells
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::SetZero(std::vector<boundaryElement<T>> const& boundary)
{
    for(size_t i = 0; i < boundary.size(); ++i)
    {
        for (size_t m = 0; m < NM_; ++m)
        {
            M_[SpatialToLinear(boundary[i].x, boundary[i].y, boundary[i].z, m)] = static_cast<T>(0.0);
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
template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::Export(std::string const name, unsigned int const step) const
{
    struct stat info;

    if (stat(OUTPUT_BIN_PATH.c_str(), &info) == 0 && S_ISDIR(info.st_mode))
    {
        std::string const fileName = OUTPUT_BIN_PATH + std::string("/") + name + std::string("_") + std::to_string(step) + std::string(".bin");
        FILE * const exportFile = fopen(fileName.c_str(), "wb+");
        fwrite(M_, 1, MEM_SIZE_, exportFile);
        fclose(exportFile);
    }
    else
    {
        std::cerr << "Fatal error: Directory '" << OUTPUT_BIN_PATH << "' not found." << std::endl;
        exit(EXIT_FAILURE);
    }
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
template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::ExportScalarVtk(unsigned int const m, std::string const name, unsigned int const step) const
{
    struct stat info;

    if (stat(OUTPUT_VTK_PATH.c_str(), &info) == 0 && S_ISDIR(info.st_mode))
    {
        std::string const fileName = OUTPUT_VTK_PATH + std::string("/") + name + std::string("_") + std::to_string(step) + std::string(".vtk");
        FILE * const exportFile = fopen(fileName.c_str(), "w");

        fprintf(exportFile, "# vtk DataFile Version 3.0\n");
        fprintf(exportFile, "LBM CFD simulation scalar %s\n", name.c_str());
        fprintf(exportFile, "ASCII\n");
        fprintf(exportFile, "DATASET RECTILINEAR_GRID\n");

        fprintf(exportFile, "DIMENSIONS %u %u %u\n", NX, NY, NZ);
        fprintf(exportFile, "X_COORDINATES %u float\n", NX);
        for(unsigned int x = 0; x < NX; ++x)
        {
            fprintf(exportFile,"%u ", x);
        }
        fprintf(exportFile, "\n");
        fprintf(exportFile, "Y_COORDINATES %u float\n", NY);
        for(unsigned int y = 0; y < NY; ++y)
        {
            fprintf(exportFile,"%u ", y);
        }
        fprintf(exportFile, "\n");
        fprintf(exportFile, "Z_COORDINATES %u float\n", NZ);
        for(unsigned int z = 0; z < NZ; ++z)
        {
            fprintf(exportFile, "%u ", z);
        }
        fprintf(exportFile, "\n");

        size_t const numberOfCells = static_cast<size_t>(NX)*static_cast<size_t>(NY)*static_cast<size_t>(NZ);
        fprintf(exportFile, "POINT_DATA %zu\n", numberOfCells);

        fprintf(exportFile, "SCALARS transport_scalar float 1\n");
        fprintf(exportFile, "LOOKUP_TABLE default\n");
        for(unsigned int z = 0; z < NZ; ++z)
        {
            for(unsigned int y = 0; y < NY; ++y)
            {
                for(unsigned int x = 0; x < NX; ++x)
                {
                    fprintf(exportFile, "%f\n", M_[SpatialToLinear(x, y, z, m)]);
                }
            }
        }

        fclose(exportFile);
    }
    else
    {
        std::cerr << "Fatal error: Directory '" << OUTPUT_VTK_PATH << "' not found." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**\fn        ExportVtk
 * \brief     Export velocity and density at current time step to a *.vtk-file that can then be
 *            read by visualisation applications like ParaView.
 * \warning   *.vtk export is comparably slow! Better export to .bin-files and then convert them
 *            to *.vtk-files with the external converter app afterwards.
 *
 * \param[in] step   the current time step that will be used for the name
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::ExportVtk(unsigned int const step) const
{
    struct stat info;

    if (stat(OUTPUT_VTK_PATH.c_str(), &info) == 0 && S_ISDIR(info.st_mode))
    {
        std::string const fileName = OUTPUT_VTK_PATH + std::string("/Export_") + std::to_string(step) + std::string(".vtk");
        FILE * const exportFile = fopen(fileName.c_str(), "w");

        fprintf(exportFile, "# vtk DataFile Version 3.0\n");
        fprintf(exportFile, "LBM CFD simulation velocity\n");
        fprintf(exportFile, "ASCII\n");
        fprintf(exportFile, "DATASET RECTILINEAR_GRID\n");

        fprintf(exportFile, "DIMENSIONS %u %u %u\n", NX, NY, NZ);
        fprintf(exportFile, "X_COORDINATES %u float\n", NX);
        for(unsigned int x = 0; x < NX; ++x)
        {
            fprintf(exportFile,"%u ", x);
        }
        fprintf(exportFile, "\n");
        fprintf(exportFile, "Y_COORDINATES %u float\n", NY);
        for(unsigned int y = 0; y < NY; ++y)
        {
            fprintf(exportFile,"%u ", y);
        }
        fprintf(exportFile, "\n");
        fprintf(exportFile, "Z_COORDINATES %u float\n", NZ);
        for(unsigned int z = 0; z < NZ; ++z)
        {
            fprintf(exportFile, "%u ", z);
        }
        fprintf(exportFile, "\n");

        size_t const numberOfCells = static_cast<size_t>(NX)*static_cast<size_t>(NY)*static_cast<size_t>(NZ);
        fprintf(exportFile, "POINT_DATA %zu\n", numberOfCells);

        fprintf(exportFile, "SCALARS density_variation float 1\n");
        fprintf(exportFile, "LOOKUP_TABLE default\n");
        for(unsigned int z = 0; z < NZ; ++z)
        {
            for(unsigned int y = 0; y < NY; ++y)
            {
                for(unsigned int x = 0; x < NX; ++x)
                {
                    fprintf(exportFile, "%f\n", M_[SpatialToLinear(x, y, z, 0)]);
                }
            }
        }

        fprintf(exportFile, "VECTORS velocity_vector float\n");
        for(unsigned int z = 0; z < NZ; ++z)
        {
            for(unsigned int y = 0; y < NY; ++y)
            {
                for(unsigned int x = 0; x < NX; ++x)
                {
                    fprintf(exportFile, "%f %f %f\n", M_[SpatialToLinear(x, y, z, 1)],
                                                      M_[SpatialToLinear(x, y, z, 2)],
                                                      M_[SpatialToLinear(x, y, z, 3)]);
                }
            }
        }

        fclose(exportFile);
    }
    else
    {
        std::cerr << "Fatal error: Directory '" << OUTPUT_VTK_PATH << "' not found." << std::endl;
        exit(EXIT_FAILURE);
    }
}

#endif // CONTINUUM_EXPORT_HPP_INCLUDED
