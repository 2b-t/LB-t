#ifndef CONTINUUM_EXPORT_HPP_INCLUDED
#define CONTINUUM_EXPORT_HPP_INCLUDED

/**
 * \file     continuum_export.hpp
 * \mainpage Class members for exporting macroscopic values
*/

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../population/boundary/boundary.hpp"
#include "../general/paths.hpp"


template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T> template <class LT, typename DerivedClass>
void Continuum<NX,NY,NZ,T>::setBoundary(BoundaryCondition<NX,NY,NZ,LT,T,DerivedClass> const& boundary)
{
    for(size_t i = 0; i < boundary.boundaryElements_.size(); ++i)
    {
        auto const& boundaryElement = boundary.boundaryElements_[i];
        M_[spatialToLinear(boundaryElement.x, boundaryElement.y, boundaryElement.z, 0)] = boundaryElement.rho;
        M_[spatialToLinear(boundaryElement.x, boundaryElement.y, boundaryElement.z, 1)] = boundaryElement.u;
        M_[spatialToLinear(boundaryElement.x, boundaryElement.y, boundaryElement.z, 2)] = boundaryElement.v;
        M_[spatialToLinear(boundaryElement.x, boundaryElement.y, boundaryElement.z, 3)] = boundaryElement.w;
    }

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::exportBin(std::string const& name, unsigned int const step) const
{
    std::filesystem::create_directories(OUTPUT_BIN_PATH);

    std::string const fileName = OUTPUT_BIN_PATH + std::string("/") + name + std::string("_") + std::to_string(step) + std::string(".bin");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "wb+"), &fclose );

    if (exportFile != nullptr)
    {
        fwrite(M_, 1, MEM_SIZE_, exportFile.get());
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::exportScalarVtk(unsigned int const m, std::string const& name, unsigned int const step) const
{
    std::filesystem::create_directories(OUTPUT_VTK_PATH);

    std::string const fileName = OUTPUT_VTK_PATH + std::string("/") + name + std::string("_") + std::to_string(step) + std::string(".vtk");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "w"), &fclose );

    if (exportFile != nullptr)
    {
        fprintf(exportFile.get(), "# vtk DataFile Version 3.0\n");
        fprintf(exportFile.get(), "LBM CFD simulation scalar %s\n", name.c_str());
        fprintf(exportFile.get(), "ASCII\n");
        fprintf(exportFile.get(), "DATASET RECTILINEAR_GRID\n");

        fprintf(exportFile.get(), "DIMENSIONS %u %u %u\n", NX, NY, NZ);
        fprintf(exportFile.get(), "X_COORDINATES %u float\n", NX);
        for(unsigned int x = 0; x < NX; ++x)
        {
            fprintf(exportFile.get(),"%u ", x);
        }
        fprintf(exportFile.get(), "\n");
        fprintf(exportFile.get(), "Y_COORDINATES %u float\n", NY);
        for(unsigned int y = 0; y < NY; ++y)
        {
            fprintf(exportFile.get(),"%u ", y);
        }
        fprintf(exportFile.get(), "\n");
        fprintf(exportFile.get(), "Z_COORDINATES %u float\n", NZ);
        for(unsigned int z = 0; z < NZ; ++z)
        {
            fprintf(exportFile.get(), "%u ", z);
        }
        fprintf(exportFile.get(), "\n");

        size_t const numberOfCells = static_cast<size_t>(NX)*static_cast<size_t>(NY)*static_cast<size_t>(NZ);
        fprintf(exportFile.get(), "POINT_DATA %zu\n", numberOfCells);

        fprintf(exportFile.get(), "SCALARS transport_scalar float 1\n");
        fprintf(exportFile.get(), "LOOKUP_TABLE default\n");
        for(unsigned int z = 0; z < NZ; ++z)
        {
            for(unsigned int y = 0; y < NY; ++y)
            {
                for(unsigned int x = 0; x < NX; ++x)
                {
                    fprintf(exportFile.get(), "%f\n", M_[spatialToLinear(x, y, z, m)]);
                }
            }
        }
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::exportVtk(unsigned int const step) const
{
    std::filesystem::create_directories(OUTPUT_VTK_PATH);

    std::string const fileName = OUTPUT_VTK_PATH + std::string("/Export_") + std::to_string(step) + std::string(".vtk");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "w"), &fclose );

    if (exportFile != nullptr)
    {
        fprintf(exportFile.get(), "# vtk DataFile Version 3.0\n");
        fprintf(exportFile.get(), "LBM CFD simulation velocity\n");
        fprintf(exportFile.get(), "ASCII\n");
        fprintf(exportFile.get(), "DATASET RECTILINEAR_GRID\n");

        fprintf(exportFile.get(), "DIMENSIONS %u %u %u\n", NX, NY, NZ);
        fprintf(exportFile.get(), "X_COORDINATES %u float\n", NX);
        for(unsigned int x = 0; x < NX; ++x)
        {
            fprintf(exportFile.get(),"%u ", x);
        }
        fprintf(exportFile.get(), "\n");
        fprintf(exportFile.get(), "Y_COORDINATES %u float\n", NY);
        for(unsigned int y = 0; y < NY; ++y)
        {
            fprintf(exportFile.get(),"%u ", y);
        }
        fprintf(exportFile.get(), "\n");
        fprintf(exportFile.get(), "Z_COORDINATES %u float\n", NZ);
        for(unsigned int z = 0; z < NZ; ++z)
        {
            fprintf(exportFile.get(), "%u ", z);
        }
        fprintf(exportFile.get(), "\n");

        size_t const numberOfCells = static_cast<size_t>(NX)*static_cast<size_t>(NY)*static_cast<size_t>(NZ);
        fprintf(exportFile.get(), "POINT_DATA %zu\n", numberOfCells);

        fprintf(exportFile.get(), "SCALARS density_variation float 1\n");
        fprintf(exportFile.get(), "LOOKUP_TABLE default\n");
        for(unsigned int z = 0; z < NZ; ++z)
        {
            for(unsigned int y = 0; y < NY; ++y)
            {
                for(unsigned int x = 0; x < NX; ++x)
                {
                    fprintf(exportFile.get(), "%f\n", M_[spatialToLinear(x, y, z, 0)]);
                }
            }
        }

        fprintf(exportFile.get(), "VECTORS velocity_vector float\n");
        for(unsigned int z = 0; z < NZ; ++z)
        {
            for(unsigned int y = 0; y < NY; ++y)
            {
                for(unsigned int x = 0; x < NX; ++x)
                {
                    fprintf(exportFile.get(), "%f %f %f\n", M_[spatialToLinear(x, y, z, 1)],
                                                            M_[spatialToLinear(x, y, z, 2)],
                                                            M_[spatialToLinear(x, y, z, 3)]);
                }
            }
        }
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

#endif // CONTINUUM_EXPORT_HPP_INCLUDED
