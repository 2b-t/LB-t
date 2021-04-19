#ifndef LBT_CONTINUUM_EXPORT
#define LBT_CONTINUUM_EXPORT

/**
 * \file     continuum_export.hpp
 * \mainpage Class members for exporting macroscopic values
 * \author   Tobit Flatscher (github.com/2b-t)
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


template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T> template <template <typename U> class LT, typename U, unsigned int NPOP,
          typename DerivedClass>
void Continuum<NX,NY,NZ,T>::setBoundary(BoundaryCondition<NX,NY,NZ,LT,U,NPOP,DerivedClass> const& boundary) noexcept
{
    for(size_t i = 0; i < boundary.boundaryElements_.size(); ++i)
    {
        auto const& boundaryElement = boundary.boundaryElements_[i];
        M[spatialToLinear(boundaryElement.x, boundaryElement.y, boundaryElement.z, 0)] = boundaryElement.rho;
        M[spatialToLinear(boundaryElement.x, boundaryElement.y, boundaryElement.z, 1)] = boundaryElement.u;
        M[spatialToLinear(boundaryElement.x, boundaryElement.y, boundaryElement.z, 2)] = boundaryElement.v;
        M[spatialToLinear(boundaryElement.x, boundaryElement.y, boundaryElement.z, 3)] = boundaryElement.w;
    }

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::exportBin(std::string const& name, unsigned int const step) const noexcept
{
    std::filesystem::create_directories(path::outputBin);

    std::string const fileName = path::outputBin + std::string("/") + name + std::string("_") + std::to_string(step) + std::string(".bin");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "wb+"), &fclose );

    if (exportFile != nullptr)
    {
        std::fwrite(M, 1, memorySize, exportFile.get());
    }
    else
    {
        std::cerr << "Fatal Error: File '" << fileName << "' could not be opened." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
void Continuum<NX,NY,NZ,T>::exportScalarVtk(unsigned int const m, std::string const& name, unsigned int const step) const noexcept
{
    std::filesystem::create_directories(path::outputVtk);

    std::string const fileName = path::outputVtk + std::string("/") + name + std::string("_") + std::to_string(step) + std::string(".vtk");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "w"), &fclose );

    if (exportFile != nullptr)
    {
        std::fprintf(exportFile.get(), "# vtk DataFile Version 3.0\n");
        std::fprintf(exportFile.get(), "LBM CFD simulation scalar %s\n", name.c_str());
        std::fprintf(exportFile.get(), "ASCII\n");
        std::fprintf(exportFile.get(), "DATASET STRUCTURED_POINTS\n");

        std::fprintf(exportFile.get(), "DIMENSIONS %u %u %u\n", NX+1, NY+1, NZ+1);
        std::fprintf(exportFile.get(), "SPACING 1 1 1\n");
        std::fprintf(exportFile.get(), "ORIGIN 0 0 0\n");

        constexpr size_t numberOfCells = static_cast<size_t>(NX)*static_cast<size_t>(NY)*static_cast<size_t>(NZ);
        std::fprintf(exportFile.get(), "CELL_DATA %zu\n", numberOfCells);

        std::fprintf(exportFile.get(), "SCALARS transport_scalar float 1\n");
        std::fprintf(exportFile.get(), "LOOKUP_TABLE default\n");
        for(unsigned int z = 0; z < NZ; ++z)
        {
            for(unsigned int y = 0; y < NY; ++y)
            {
                for(unsigned int x = 0; x < NX; ++x)
                {
                    std::fprintf(exportFile.get(), "%f\n", M[spatialToLinear(x, y, z, m)]);
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
void Continuum<NX,NY,NZ,T>::exportVtk(unsigned int const step) const noexcept
{
    std::filesystem::create_directories(path::outputVtk);

    std::string const fileName = path::outputVtk + std::string("/Export_") + std::to_string(step) + std::string(".vtk");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "w"), &fclose );

    if (exportFile != nullptr)
    {
        std::fprintf(exportFile.get(), "# vtk DataFile Version 3.0\n");
        std::fprintf(exportFile.get(), "LBM CFD simulation velocity\n");
        std::fprintf(exportFile.get(), "ASCII\n");
        std::fprintf(exportFile.get(), "DATASET STRUCTURED_POINTS\n");

        std::fprintf(exportFile.get(), "DIMENSIONS %u %u %u\n", NX+1, NY+1, NZ+1);
        std::fprintf(exportFile.get(), "SPACING 1 1 1\n");
        std::fprintf(exportFile.get(), "ORIGIN 0 0 0\n");

        constexpr size_t numberOfCells = static_cast<size_t>(NX)*static_cast<size_t>(NY)*static_cast<size_t>(NZ);
        std::fprintf(exportFile.get(), "CELL_DATA %zu\n", numberOfCells);

        std::fprintf(exportFile.get(), "SCALARS density_variation float 1\n");
        std::fprintf(exportFile.get(), "LOOKUP_TABLE default\n");
        for(unsigned int z = 0; z < NZ; ++z)
        {
            for(unsigned int y = 0; y < NY; ++y)
            {
                for(unsigned int x = 0; x < NX; ++x)
                {
                    std::fprintf(exportFile.get(), "%f\n", M[spatialToLinear(x, y, z, 0)]);
                }
            }
        }

        std::fprintf(exportFile.get(), "VECTORS velocity_vector float\n");
        for(unsigned int z = 0; z < NZ; ++z)
        {
            for(unsigned int y = 0; y < NY; ++y)
            {
                for(unsigned int x = 0; x < NX; ++x)
                {
                    std::fprintf(exportFile.get(), "%f %f %f\n", M[spatialToLinear(x, y, z, 1)],
                                                                 M[spatialToLinear(x, y, z, 2)],
                                                                 M[spatialToLinear(x, y, z, 3)]);
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

#endif // LBM_CONTINUUM_EXPORT
