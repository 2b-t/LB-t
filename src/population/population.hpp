#ifndef POPULATION_HPP_INCLUDED
#define POPULATION_HPP_INCLUDED

/**
 * \file     population.hpp
 * \mainpage Class for microscopic populations
*/

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#ifdef _WIN32
#include <malloc.h>
#endif
#include <memory>
#include <string>

#include "../general/memory_alignment.hpp"
#include "../general/constexpr_func.hpp"


/**\enum  timestep
 * \brief Strongly typed for even and odd time steps required for AA access pattern
 */
enum class timestep: bool { even = false, odd = true };

/**\fn        Negation timestep operator
 * \brief     Negation operator for the timestep
 * 
 * \param[in] ts   Timestep to be negated
 * \return    Negated timestep
 */
constexpr timestep operator ! (timestep const& ts)
{
    return (ts == timestep::even) ? timestep::odd : timestep::even;
}


/**\class  Population
 * \brief  Class that holds macroscopic values
 * 
 * \tparam NX     Simulation domain resolution in x-direction
 * \tparam NY     Simulation domain resolution in y-direction
 * \tparam NZ     Simulation domain resolution in z-direction
 * \tparam LT     Static lattice::DdQq class containing discretisation parameters
 * \tparam NPOP   Number of populations stored side by side in the lattice (default = 1)
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, class LT, unsigned int NPOP = 1>
class Population
{
    /// import current lattice floating data type
    typedef typename std::remove_const<decltype(LT::CS)>::type T;

    public:
        Population& operator = (Population&) = delete;
        Population(Population&&) = delete;
        Population& operator = (Population&&) = delete;

        /**\brief Class constructor
        */
        Population()
        {
            if (F_ == nullptr)
            {
                std::cerr << "Fatal error: Population could not be allocated." << std::endl;
                std::exit(EXIT_FAILURE);
            }

            return;
        }
        
        /**\brief     Class copy constructor
         * \param[in] p   The population object to be copied
        */
        Population(Population const& p)
        {
            std::memcpy(F_, p.F_, MEM_SIZE_);

            return;
        }

        /**\brief Class destructor
        */
        ~Population()
        {
            std::cout << "See you, comrade!" << std::endl;
            #ifdef _WIN32
            _aligned_free(F_);
            #else
            std::free(F_);
            #endif

            return;
        }

        /**\fn        spatialToLinear
         * \brief     Inline function for converting 3D population coordinates to scalar index
         * \warning   Inline function! Has to be declared in header!
         *
         * \param[in] x   x coordinate of cell
         * \param[in] y   y coordinate of cell
         * \param[in] z   z coordinate of cell
         * \param[in] n   Positive (0) or negative (1) index/lattice velocity
         * \param[in] d   Relevant population index
         * \param[in] p   Relevant population (default = 0)
         * \return    Requested linear population index
        */
        inline size_t spatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                      unsigned int const n, unsigned int const d, unsigned int const p = 0) const;

        /**\fn        linearToSpatial
        * \brief      Generate 3D population coordinates from scalar index
        *
        * \param[out] x       Return value x coordinate of cell
        * \param[out] y       Return value y coordinate of cell
        * \param[out] z       Return value z coordinate of cell
        * \param[out] p       Return value number of population
        * \param[out] n       Return positive (0) or negative (1) index
        * \param[out] d       Return value number of relevant population index
        * \param[in]  index   Current linear population index
        */
        void          linearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z,
                                      unsigned int& p, unsigned int& n, unsigned int& d,
                                      size_t const index) const;

        /**\fn         AA_IndexRead
         * \brief      Function for determining linear index when reading/writing values before collision depending on even and odd time step.
         * \warning    Inline function! Has to be declared in header!
         *
         * \tparam     AA    Even (0, false) or odd (1, true) time step
         * \param[in]  x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in]  y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in]  z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in]  n     Positive (0) or negative (1) index/lattice velocity
         * \param[in]  d     Relevant population index
         * \param[in]  p     Relevant population (default = 0)
         * \return     Requested linear population index before collision
        */
        template <timestep AA>
        inline size_t AA_IndexRead(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                   unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const;

        /**\fn         AA_IndexWrite
         * \brief      Function for determining linear index when reading/writing values after collision depending on even and odd time step.
         * \warning    Inline function! Has to be declared in header!
         *
         * \tparam     AA    Even (0, false) or odd (1, true) time step
         * \param[in]  x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in]  y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in]  z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in]  n     Positive (0) or negative (1) index/lattice velocity
         * \param[in]  d     Relevant population index
         * \param[in]  p     Relevant population (default = 0)
         * \return     Requested linear population index after collision
        */
        template <timestep AA>
        inline size_t AA_IndexWrite(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                    unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const;

        /**\fn         AA_Read
         * \brief      Function for accessing values before collision depending on even and odd time step.
         * \warning    Sloppy syntax: avoid usage, use the full expression below for indexing
         *
         * \tparam     AA    Even (0, false) or odd (1, true) time step
         * \param[in]  x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in]  y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in]  z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in]  n     Positive (0) or negative (1) index/lattice velocity
         * \param[in]  d     Relevant population index
         * \param[in]  p     Relevant population (default = 0)
         * \return     Requested linear population index before collision (reading)
        */
        template <timestep AA>
        inline auto&       AA_Read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                   unsigned int const n,       unsigned int const d,       unsigned int const p = 0);
        template <timestep AA>
        inline auto const& AA_Read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                   unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const;

        /**\fn         AA_Write
         * \brief      Function for accessing values after collision depending on even and odd time step.
         * \warning    Sloppy syntax: avoid usage, use the full expression below for indexing
         *
         * \tparam     odd   Even (0, false) or odd (1, true) time step
         * \param[in]  x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in]  y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in]  z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in]  n     Positive (0) or negative (1) index/lattice velocity
         * \param[in]  d     Relevant population index
         * \param[in]  p     Relevant population (default = 0)
         * \return     Requested linear population index after collision (writing)
        */
        template <timestep AA>
        inline auto&       AA_Write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                    unsigned int const n,       unsigned int const d,       unsigned int const p = 0);
        template <timestep AA>
        inline auto const& AA_Write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                    unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const;

        /**\fn        importBin
         * \brief     import populations from a *.bin file (initialisation)
         * \warning   Please make sure the correct resolution is selected and run the
         *            initialisation of populations afterwards.
         *
         * \param[in] name   The import file name of the scalar
        */
        void importBin(std::string const& name);

        /**\fn        exportBin
         * \brief     Export populations at current time step to *.bin file
         *
         * \param[in] name   The export file name of the scalar
        */
        void exportBin(std::string const& name) const;


        /// lattice characteristics
        static constexpr unsigned int    DIM_ = LT::DIM;
        static constexpr unsigned int SPEEDS_ = LT::SPEEDS;
        static constexpr unsigned int HSPEED_ = LT::HSPEED;

        /// linear memory layout
        static constexpr unsigned int PAD_ = LT::PAD;
        static constexpr unsigned int  ND_ = LT::ND;
        static constexpr unsigned int OFF_ = LT::OFF;
        static constexpr size_t  MEM_SIZE_ = sizeof(T)*NZ*NY*NX*NPOP*static_cast<size_t>(ND_);

        /// pointer to population
        #ifdef _WIN32
        T* const F_ = static_cast<T*>(_aligned_malloc(MEM_SIZE_, CACHE_LINE));
        #else
        T* const F_ = static_cast<T*>(std::aligned_alloc(CACHE_LINE, MEM_SIZE_));
        #endif
};

/// include related header files
#include "population_indexing.hpp"
#include "population_backup.hpp"

#endif // POPULATION_HPP_INCLUDED
