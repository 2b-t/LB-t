#ifndef CONTINUUM_HPP_INCLUDED
#define CONTINUUM_HPP_INCLUDED


#include <memory>
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <stdlib.h>
#include <iostream>

#include "memory_alignment.hpp"
#include "boundary.hpp"


/**\class Continuum
 * \brief Class for the macroscopic variables
*/
template <typename T = double>
class Continuum
{
    public:

        unsigned int const NM = 4; //number of macroscopic values: rho, ux, uy, uz
        unsigned int const NX = 0; //resolution
        unsigned int const NY = 0;
        unsigned int const NZ = 0;

        size_t const MEM_SIZE = sizeof(T)*NZ*NY*NX*NM; //size of array in byte

        /// population
        T* M = nullptr;


        /// constructor
        Continuum(unsigned int const X, unsigned int const Y, unsigned int const Z):
            NX(X), NY(Y), NZ(Z)
        {
            M = (T*) aligned_alloc(CACHE_LINE, MEM_SIZE);

            if (M == nullptr)
            {
                std::cout << "ERROR" << std::endl;
            }
        }

        /// destructor
        ~Continuum()
        {
            free(M);
        }

        /// indexing functions
        inline size_t SpatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                      unsigned int const m) const;
        void   LinearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z,
                               unsigned int& m, size_t const index) const;
        inline T&       operator() (unsigned int const x, unsigned int const y, unsigned int const z, unsigned int const m);
        inline T const& operator() (unsigned int const x, unsigned int const y, unsigned int const z, unsigned int const m) const;

        /// export
        void SetZero(std::vector<boundary_element<T>>& boundary);
        void Export(char const * name, unsigned int const step) const;
        void ExportScalarVtk(unsigned int const m, char const * name, unsigned int const step) const;
        void ExportVtk(unsigned int const step) const;

        /// import time step
        void Import(char const * name, unsigned int const step);
};

#include "continuum_indexing.hpp"
#include "continuum_backup.hpp"
#include "continuum_export.hpp"

#endif //CONTINUUM_HPP_INCLUDED
