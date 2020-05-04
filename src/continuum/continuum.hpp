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
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "../population/boundary/boundary.hpp"
#include "../general/memory_alignment.hpp"


/**\class Continuum
 * \brief Class for the macroscopic variables
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, typename T = double>
class Continuum
{
    public:
        static constexpr unsigned int NM_ = 4; //number of macroscopic values: rho, ux, uy, uz
        static constexpr size_t MEM_SIZE_ = sizeof(T)*NZ*NY*NX*static_cast<size_t>(NM_); //size of array in byte

        /// population allocated in heap
        T* const M_ = static_cast<T*>(aligned_alloc(CACHE_LINE, MEM_SIZE_));


        /// class constructor
        Continuum()
        {
            if (M_ == nullptr)
            {
                std::cerr << "Fatal error: Continuum could not be allocated." << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        /// class destructor
        ~Continuum()
        {
            free(M_);
        }

        /// lattice indexing functions
        inline size_t SpatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                      unsigned int const m) const;
        void          LinearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z,
                                      unsigned int& m, size_t const index) const;
        inline T&       operator() (unsigned int const x, unsigned int const y, unsigned int const z, unsigned int const m);
        inline T const& operator() (unsigned int const x, unsigned int const y, unsigned int const z, unsigned int const m) const;

        /// export to disk
        void SetZero(std::vector<boundaryElement<T>> const& boundary);
        void Export(std::string const name, unsigned int const step) const;
        void ExportScalarVtk(unsigned int const m, std::string const name, unsigned int const step) const;
        void ExportVtk(unsigned int const step) const;

        /// import time step from disk
        void Import(std::string const name, unsigned int const step);
};

#include "continuum_indexing.hpp"
#include "continuum_import.hpp"
#include "continuum_export.hpp"

#endif // CONTINUUM_HPP_INCLUDED
