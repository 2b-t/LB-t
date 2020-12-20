#ifndef LBT_POPULATION
#define LBT_POPULATION

/**
 * \file     population.hpp
 * \mainpage Class for microscopic populations
 * \author   Tobit Flatscher (github.com/2b-t)
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
#include "../general/constexpr_math.hpp"
#include "indexing/aa_pattern.hpp"
#include "indexing/indexing.hpp"


/**\class  Population
 * \brief  Class that holds macroscopic values
 *
 * \tparam NX     Simulation domain resolution in x-direction
 * \tparam NY     Simulation domain resolution in y-direction
 * \tparam NZ     Simulation domain resolution in z-direction
 * \tparam LT     Static lattice::DdQq class containing discretisation parameters
 * \tparam T      Floating data type used for simulation
 * \tparam NPOP   Number of populations stored side by side in a single merged grid (default = 1)
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP = 1>
class Population
{
    public:
        Population& operator = (Population&) = delete;
        Population(Population&&) = delete;
        Population& operator = (Population&&) = delete;

        /**\brief Class constructor
        */
        Population() noexcept
        {
            static_assert( (LT<T>::DIM == 2) ? (NZ == 1) : true, "Two-dimensional lattice with NZ != 1." );

            if (A == nullptr)
            {
                std::cerr << "Fatal error: Population could not be allocated." << std::endl;
                std::exit(EXIT_FAILURE);
            }

            return;
        }

        /**\brief     Class copy constructor
         * \param[in] p   The population object to be copied
        */
        Population(Population const& p) noexcept
        {
            std::memcpy(A, p.A, memorySize);

            return;
        }

        /**\brief Class destructor
        */
        ~Population() noexcept
        {
            std::cout << "See you, comrade!" << std::endl;
            #ifdef _WIN32
                _aligned_free(A);
            #else
                std::free(A);
            #endif

            return;
        }

        /**\fn        indexRead
         * \brief     Function for determining linear index when reading/writing values before collision depending on even and odd time step.
         * \warning   Inline function! Has to be declared in header!
         *
         * \tparam    TS    Even (0, false) or odd (1, true) time step
         * \param[in] x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in] y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in] z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in] n     Positive (0) or negative (1) index/lattice velocity
         * \param[in] d     Relevant population index
         * \param[in] p     Relevant population (default = 0)
         * \return    Requested linear population index before collision
        */
        template <timestep TS>
        static inline size_t __attribute__((always_inline)) indexRead(std::array<unsigned int,3> const &x, 
                                                                      std::array<unsigned int,3> const &y,
                                                                      std::array<unsigned int,3> const &z,
                                                                      unsigned int               const n,
                                                                      unsigned int               const d,
                                                                      unsigned int               const p = 0) noexcept
        {
            return AaPattern<NX,NY,NZ,LT,T,NPOP>::template indexRead<TS>(x,y,z,n,d,p);
        }

        /**\fn        indexWrite
         * \brief     Function for determining linear index when reading/writing values after collision depending on even and odd time step.
         * \warning   Inline function! Has to be declared in header!
         *
         * \tparam    TS    Even (0, false) or odd (1, true) time step
         * \param[in] x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in] y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in] z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in] n     Positive (0) or negative (1) index/lattice velocity
         * \param[in] d     Relevant population index
         * \param[in] p     Relevant population (default = 0)
         * \return    Requested linear population index after collision
        */
        template <timestep TS>
        static inline size_t __attribute__((always_inline)) indexWrite(std::array<unsigned int,3> const &x, 
                                                                       std::array<unsigned int,3> const &y,
                                                                       std::array<unsigned int,3> const &z,
                                                                       unsigned int               const n,
                                                                       unsigned int               const d,
                                                                       unsigned int               const p = 0) noexcept
        {
            return AaPattern<NX,NY,NZ,LT,T,NPOP>::template indexWrite<TS>(x,y,z,n,d,p);
        }

        /**\fn        read
         * \brief     Function for accessing values before collision depending on even and odd time step.
         * \warning   Sloppy syntax: avoid usage, use the full expression below for indexing
         *
         * \tparam    TS    Even (0, false) or odd (1, true) time step
         * \param[in] x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in] y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in] z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in] n     Positive (0) or negative (1) index/lattice velocity
         * \param[in] d     Relevant population index
         * \param[in] p     Relevant population (default = 0)
         * \return    Requested linear population index before collision (reading)
        */
        template <timestep TS>
        inline auto&       __attribute__((always_inline)) read(std::array<unsigned int,3> const &x, 
                                                               std::array<unsigned int,3> const &y,
                                                               std::array<unsigned int,3> const &z,
                                                               unsigned int               const n,
                                                               unsigned int               const d,
                                                               unsigned int               const p = 0) noexcept
        {
            return A[indexRead<TS>(x,y,z,n,d,p)];
        }

        template <timestep TS>
        inline auto const& __attribute__((always_inline)) read(std::array<unsigned int,3> const &x, 
                                                               std::array<unsigned int,3> const &y,
                                                               std::array<unsigned int,3> const &z,
                                                               unsigned int               const n,
                                                               unsigned int               const d,
                                                               unsigned int               const p = 0) const noexcept
        {
            return A[indexRead<TS>(x,y,z,n,d,p)];
        }

        /**\fn        write
         * \brief     Function for accessing values after collision depending on even and odd time step.
         * \warning   Sloppy syntax: avoid usage, use the full expression below for indexing
         *
         * \tparam    TS    Even (0, false) or odd (1, true) time step
         * \param[in] x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in] y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in] z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in] n     Positive (0) or negative (1) index/lattice velocity
         * \param[in] d     Relevant population index
         * \param[in] p     Relevant population (default = 0)
         * \return    Requested linear population index after collision (writing)
        */
        template <timestep TS>
        inline auto&       __attribute__((always_inline))  write(std::array<unsigned int,3> const &x, 
                                                                 std::array<unsigned int,3> const &y,
                                                                 std::array<unsigned int,3> const &z,
                                                                 unsigned int               const n,
                                                                 unsigned int               const d,
                                                                 unsigned int               const p = 0) noexcept
        {
            return A[indexWrite<TS>(x,y,z,n,d,p)];
        }

        template <timestep TS>
        inline auto const& __attribute__((always_inline)) write(std::array<unsigned int,3> const &x, 
                                                                std::array<unsigned int,3> const &y,
                                                                std::array<unsigned int,3> const &z,
                                                                unsigned int               const n,
                                                                unsigned int               const d,
                                                                unsigned int               const p) const noexcept
        {
            return A[indexWrite<TS>(x,y,z,n,d,p)];
        }

        /**\fn        importBin
         * \brief     import populations from a *.bin file (initialisation)
         * \warning   Please make sure the correct resolution is selected and run the
         *            initialisation of populations afterwards.
         *
         * \param[in] name   The import file name of the scalar
        */
        void importBin(std::string const& name) noexcept;

        /**\fn        exportBin
         * \brief     Export populations at current time step to *.bin file
         *
         * \param[in] name   The export file name of the scalar
        */
        void exportBin(std::string const& name) const noexcept;


        /// lattice characteristics
        static constexpr size_t  memorySize = sizeof(T)*NZ*NY*NX*NPOP*static_cast<size_t>(LT<T>::ND);

        /// pointer to population
        #ifdef _WIN32
            T* const A = static_cast<T*>(_aligned_malloc(memorySize, CACHE_LINE));
        #else
            T* const A = static_cast<T*>(std::aligned_alloc(CACHE_LINE, memorySize));
        #endif
};

/// include related header files
#include "population_backup.hpp"

#endif // LBT_POPULATION
