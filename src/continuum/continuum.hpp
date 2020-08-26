#ifndef CONTINUUM_HPP_INCLUDED
#define CONTINUUM_HPP_INCLUDED

/**
 * \file     continuum.hpp
 * \mainpage Class for continuum properties
*/

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../population/boundary/boundary.hpp"
#include "../general/memory_alignment.hpp"


/**\class  Continuum
 * \brief  Class for the macroscopic variables
 *
 * \tparam NX   Simulation domain resolution in x-direction
 * \tparam NY   Simulation domain resolution in y-direction
 * \tparam NZ   Simulation domain resolution in z-direction
 * \tparam T    Floating data type used for simulation
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T>
class Continuum
{
    public:
        Continuum& operator = (Continuum&) = delete;
        Continuum(Continuum&&) = delete;
        Continuum& operator = (Continuum&&) = delete;

        /**\brief Class constructor
        */
        Continuum()
        {
            if (M_ == nullptr)
            {
                std::cerr << "Fatal error: Continuum could not be allocated." << std::endl;
                std::exit(EXIT_FAILURE);
            }

            return;
        }
        
        /**\brief     Class copy constructor
         * \param[in] c   The continuum object to be copied
        */
        Continuum(Continuum const& c)
        {
            std::memcpy(M_, c.M_, MEM_SIZE_);

            return;
        }

        /**\brief Class destructor
        */
        ~Continuum()
        {
            std::free(M_);

            return;
        }

        /**\fn         spatialToLinear
         * \brief      Inline function for converting 3D population coordinates to scalar index
         * \warning    Inline function! Has to be declared in header!
         *
         * \param[in]  x   x coordinate of cell
         * \param[in]  y   y coordinate of cell
         * \param[in]  z   z coordinate of cell
         * \param[in]  m   macroscopic value (0: density, 1-3: ux, uy,uz)
         * \return     requested linear scalar index
        */
        inline size_t spatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                      unsigned int const m) const;

        /**\fn         linearToSpatial
         * \brief      Generate 3D population coordinates from scalar index
         *
         * \param[out] x       return value x coordinate of cell
         * \param[out] y       return value y coordinate of cell
         * \param[out] z       return value z coordinate of cell
         * \param[in]  index   current linear scalar index
        */
        void          linearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z,
                                      unsigned int& m, size_t const index) const;

        /**\fn         operator()
         * \brief      Access scalar values at given coordinates
         *
         * \param[in]  x   x coordinate of cell
         * \param[in]  y   y coordinate of cell
         * \param[in]  z   z coordinate of cell
         * \param[in]  m   macroscopic value (0: density, 1-3: ux, uy,uz)
         * \return     the requested scalar value
        */
        inline T&       operator() (unsigned int const x, unsigned int const y, unsigned int const z, unsigned int const m);
        inline T const& operator() (unsigned int const x, unsigned int const y, unsigned int const z, unsigned int const m) const;

        /**\fn        setBoundary
         * \brief     Set not relevant nodes (e.g. boundary nodes) of a the hydrodynamic
         *            field (velocities and density) to corresponding velocity
         *
         * \param[in] boundary   The boundary condition
        */
        template <class LT, typename DerivedClass>
        void setBoundary(BoundaryCondition<NX,NY,NZ,LT,T,DerivedClass> const& boundary);
        
        /**\fn        exportBin
         * \brief     Export any scalar quantity at current time step to *.bin file writing to *.bin-files
         *            is significantly faster than using non-binary *.txt-files
         *
         * \param[in] name   The export file name of the scalar
         * \param[in] step   The current time step that will be used for the name
        */
        void exportBin(std::string const& name, unsigned int const step) const;
        
        /**\fn        exportScalarVtk
         * \brief     Export arbitrary scalar at current time step to a *.vtk-file that can then be read
         *            by visualisation applications like ParaView.
         * \warning   *.vtk export is comparably slow! Better export to .bin-files and then convert them
         *            to *.vtk-files with the external converter app afterwards.
         *
         * \param[in] name   The export file name of the scalar
         * \param[in] step   The current time step that will be used for the name
        */
        void exportScalarVtk(unsigned int const m, std::string const& name, unsigned int const step) const;
        
        /**\fn        exportVtk
         * \brief     Export velocity and density at current time step to a *.vtk-file that can then be
         *            read by visualisation applications like ParaView.
         * \warning   *.vtk export is comparably slow! Better export to .bin-files and then convert them
         *            to *.vtk-files with the external converter app afterwards.
         *
         * \param[in] step   The current time step that will be used for the name
        */
        void exportVtk(unsigned int const step) const;

        /**\fn         importBin
         * \brief      Import macroscopic values from *.bin-file
         *
         * \param[in]  name   The import file name holding the macroscopic quantities
         * \param[in]  step   The current time step that will be used for the name
        */
        void importBin(std::string const& name, unsigned int const step);
        
        
        static constexpr unsigned int NM_ = 4; // number of macroscopic values: rho, ux, uy, uz
        static constexpr size_t MEM_SIZE_ = sizeof(T)*NZ*NY*NX*static_cast<size_t>(NM_); // size of array in byte

        /// population allocated in heap
        T* const M_ = static_cast<T*>(std::aligned_alloc(CACHE_LINE, MEM_SIZE_));
};

#include "continuum_indexing.hpp"
#include "continuum_import.hpp"
#include "continuum_export.hpp"

#endif // CONTINUUM_HPP_INCLUDED
