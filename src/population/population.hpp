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
        Population()
        {
            static_assert( (LT<T>::DIM == 2) ? (NZ == 1) : true, "Two-dimensional lattice with NZ != 1." );

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

        template <timestep TS>
        static inline size_t __attribute__((always_inline)) indexRead(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                      unsigned int const n,       unsigned int const d,       unsigned int const p)
        {
            return AaPattern<NX,NY,NZ,LT,T,NPOP>::template indexRead<TS>(x,y,z,n,d,p);
        }

        template <timestep TS>
        static inline size_t __attribute__((always_inline)) indexWrite(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                      unsigned int const n,       unsigned int const d,       unsigned int const p)
        {
            return AaPattern<NX,NY,NZ,LT,T,NPOP>::template indexWrite<TS>(x,y,z,n,d,p);
        }


        /**\fn         read
         * \brief      Function for accessing values before collision depending on even and odd time step.
         * \warning    Sloppy syntax: avoid usage, use the full expression below for indexing
         *
         * \tparam     TS    Even (0, false) or odd (1, true) time step
         * \param[in]  x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in]  y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in]  z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in]  n     Positive (0) or negative (1) index/lattice velocity
         * \param[in]  d     Relevant population index
         * \param[in]  p     Relevant population (default = 0)
         * \return     Requested linear population index before collision (reading)
        */
        template <timestep TS>
        inline auto&       __attribute__((always_inline)) read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                               unsigned int const n,       unsigned int const d,       unsigned int const p = 0)
        {
            return F_[indexRead<TS>(x,y,z,n,d,p)];
        }
        
        template <timestep TS>
        inline auto const& __attribute__((always_inline)) read(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                               unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const
        {
            return F_[indexRead<TS>(x,y,z,n,d,p)];
        }

        /**\fn         write
         * \brief      Function for accessing values after collision depending on even and odd time step.
         * \warning    Sloppy syntax: avoid usage, use the full expression below for indexing
         *
         * \tparam     TS    Even (0, false) or odd (1, true) time step
         * \param[in]  x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in]  y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in]  z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in]  n     Positive (0) or negative (1) index/lattice velocity
         * \param[in]  d     Relevant population index
         * \param[in]  p     Relevant population (default = 0)
         * \return     Requested linear population index after collision (writing)
        */
        template <timestep TS>
        inline auto&       __attribute__((always_inline))  write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                 unsigned int const n,       unsigned int const d,       unsigned int const p = 0)
        {
            return F_[indexWrite<TS>(x,y,z,n,d,p)];
        }
        
        template <timestep TS>
        inline auto const& __attribute__((always_inline)) write(unsigned int const (&x)[3], unsigned int const (&y)[3], unsigned int const (&z)[3],
                                                                unsigned int const n,       unsigned int const d,       unsigned int const p = 0) const
        {
            return F_[indexWrite<TS>(x,y,z,n,d,p)];
        }

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
        static constexpr unsigned int    DIM_ = LT<T>::DIM;
        static constexpr unsigned int SPEEDS_ = LT<T>::SPEEDS;
        static constexpr unsigned int HSPEED_ = LT<T>::HSPEED;

        /// linear memory layout
        static constexpr unsigned int PAD_ = LT<T>::PAD;
        static constexpr unsigned int  ND_ = LT<T>::ND;
        static constexpr unsigned int OFF_ = LT<T>::OFF;
        static constexpr size_t  MEM_SIZE_ = sizeof(T)*NZ*NY*NX*NPOP*static_cast<size_t>(ND_);

        /// pointer to population
        #ifdef _WIN32
        T* const F_ = static_cast<T*>(_aligned_malloc(MEM_SIZE_, CACHE_LINE));
        #else
        T* const F_ = static_cast<T*>(std::aligned_alloc(CACHE_LINE, MEM_SIZE_));
        #endif
};

/// include related header files
#include "population_backup.hpp"

#endif // LBT_POPULATION
