#ifndef LBT_BOUNDARY
#define LBT_BOUNDARY

/**
 * \file     boundary.hpp
 * \mainpage Holds struct for boundary element information
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <filesystem>
#include <fstream>
#include <memory>

#include "../../continuum/continuum.hpp"
#include "../../general/paths.hpp"
#include "../population.hpp"


namespace boundary
{
    /**\struct boundary::Element
     * \brief  Struct holding properties of boundary element
     *
     * \tparam T   Floating data type used for simulation
    */
    template <typename T>
    struct Element
    {
        /// spatial position of element
        unsigned int const x;
        unsigned int const y;
        unsigned int const z;

        /// macroscopic properties
        T const rho; ///< density
        T const u;   ///< velocity in x-direction
        T const v;   ///< velocity in y-direction
        T const w;   ///< velocity in z-direction
    };

}


/**\class BoundaryCondition
 * \brief Base class for all boundary conditions
 *
 * \tparam NX             Simulation domain resolution in x-direction
 * \tparam NY             Simulation domain resolution in y-direction
 * \tparam NZ             Simulation domain resolution in z-direction
 * \tparam LT             Static lattice::DdQq class containing discretisation parameters
 * \tparam T              Floating data type used for simulation
 * \tparam NPOP           Number of populations stored side by side in a single merged grid
 * \tparam DerivedClass   The derived boundary condition that inherits from this base class
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP, typename DerivedClass>
class BoundaryCondition
{
    public:
        BoundaryCondition() = delete;

        /**\fn     beforeCollisionOperator
         * \brief  Curiously Recurring Template Pattern (CRTP) for static polymorphism of the part of the boundary
         *         condition to be performed before collision
         *
         * \tparam TS   Even or odd timestep
        */
        template <timestep TS>
        void beforeCollisionOperator() noexcept
        {
            static_cast<DerivedClass*>(this)->template implementationBeforeCollisionOperator<TS>();

            return;
        }

        /**\fn     afterCollisionOperator
         * \brief  Curiously Recurring Template Pattern (CRTP) for static polymorphism of the part of the boundary
         *         condition to be performed after collision
         *
         * \tparam TS   Even or odd timestep
        */
        template <timestep TS>
        void afterCollisionOperator() noexcept
        {
            static_cast<DerivedClass*>(this)->template implementationAfterCollisionOperator<TS>();

            return;
        }

        /**\fn        exportDomainVtk
         * \brief     Export the domain to a *.vtk-file that can then be read by visualisation applications like ParaView.
         * \warning   Assumes the boundary cells are ordered by x->y->z
         *
         * \param[in] name   The export file name of the domain
        */
        void exportDomainVtk(std::string const& name = "domain") const noexcept;

        std::vector<boundary::Element<T>> const boundaryElements_; ///< public in order to avoid circular dependency with continuum

    protected:
        /**\brief     Constructor
         *
         * \param[in] population         Population object holding microscopic distributions
         * \param[in] boundaryElements   Elements making up the boundary
         * \param[in] p                  Index of relevant population
        */
        BoundaryCondition(std::shared_ptr<Population<NX,NY,NZ,LT,T,NPOP>> population, std::vector<boundary::Element<T>> const& boundaryElements,
                          unsigned int const p = 0) noexcept:
            boundaryElements_(boundaryElements), population_(population), p_(p)
        {
            return;
        }

        std::shared_ptr<Population<NX,NY,NZ,LT,T,NPOP>> population_;
        unsigned int const p_;
};

template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP, typename DerivedClass>
void BoundaryCondition<NX,NY,NZ,LT,T,NPOP,DerivedClass>::exportDomainVtk(std::string const& name) const noexcept
{
    std::filesystem::create_directories(path::outputVtk);

    std::string const fileName = path::outputVtk + std::string("/") + name + std::string(".vtk");
    auto const exportFile = std::unique_ptr<FILE, decltype(&fclose)>( fopen(fileName.c_str(), "w"), &fclose );

    if (exportFile != nullptr)
    {
        std::fprintf(exportFile.get(), "# vtk DataFile Version 3.0\n");
        std::fprintf(exportFile.get(), "LBM CFD simulation\n");
        std::fprintf(exportFile.get(), "ASCII\n");
        std::fprintf(exportFile.get(), "DATASET RECTILINEAR_GRID\n");

        std::fprintf(exportFile.get(), "DIMENSIONS %u %u %u\n", NX, NY, NZ);
        std::fprintf(exportFile.get(), "X_COORDINATES %u float\n", NX);
        for(unsigned int x = 0; x < NX; ++x)
        {
            std::fprintf(exportFile.get(),"%u ", x);
        }
        std::fprintf(exportFile.get(), "\n");
        std::fprintf(exportFile.get(), "Y_COORDINATES %u float\n", NY);
        for(unsigned int y = 0; y < NY; ++y)
        {
            std::fprintf(exportFile.get(),"%u ", y);
        }
        std::fprintf(exportFile.get(), "\n");
        std::fprintf(exportFile.get(), "Z_COORDINATES %u float\n", NZ);
        for(unsigned int z = 0; z < NZ; ++z)
        {
            std::fprintf(exportFile.get(), "%u ", z);
        }
        std::fprintf(exportFile.get(), "\n");

        constexpr size_t numberOfCells = static_cast<size_t>(NX)*static_cast<size_t>(NY)*static_cast<size_t>(NZ);
        std::fprintf(exportFile.get(), "POINT_DATA %zu\n", numberOfCells);

        std::fprintf(exportFile.get(), "SCALARS fluid_solid float 1\n");
        std::fprintf(exportFile.get(), "LOOKUP_TABLE default\n");
        size_t j = 0;
        for(size_t i = 0; i < boundaryElements_.size(); ++i)
        {
            auto const& b = boundaryElements_.at(i);
            size_t const upperIndex = ((static_cast<size_t>(b.z)*NY + static_cast<size_t>(b.y))*NX + static_cast<size_t>(b.x));
            while (j+1 < upperIndex)
            {
                std::fprintf(exportFile.get(), "0\n");
                ++j;
            }
            std::fprintf(exportFile.get(), "1\n");
            j = upperIndex;
        }

        auto const& b = boundaryElements_.back();
        size_t const lowerIndex = ((static_cast<size_t>(b.z)*NY + static_cast<size_t>(b.y))*NX + static_cast<size_t>(b.x));
        for(size_t i = lowerIndex; i < numberOfCells-1; ++i)
        {
            std::fprintf(exportFile.get(), "0\n");
        }
    }

    return;
}

#endif // LBT_BOUNDARY
