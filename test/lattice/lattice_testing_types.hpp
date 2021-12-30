#ifndef LBT_LATTICE_TESTING_TYPES
#define LBT_LATTICE_TESTING_TYPES
#pragma once

/**
 * \file     lattice_testing_types.hpp
 * \mainpage Classes and type definitions for generating all the relevant lattices to be tested
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <tuple>

#include <gtest/gtest.h>

#include "../../src/lattice/D2Q9.hpp"
#include "../../src/lattice/D3Q15.hpp"
#include "../../src/lattice/D3Q19.hpp"
#include "../../src/lattice/D3Q27.hpp"
#include "../../src/general/tuple_utilities.hpp"
#include "../testing_utilities/testing_utilities.hpp"


namespace lbt {
  namespace test {

    /// The different lattices to be tested
    template <typename T> using LatticeTypes = std::tuple<lbt::lattice::D2Q9P10<T>,  lbt::lattice::D2Q9P12<T>, 
                                                          lbt::lattice::D3Q15P16<T>, lbt::lattice::D3Q19P20<T>, 
                                                          lbt::lattice::D3Q27P28<T>, lbt::lattice::D3Q27PC<T>>;
    /// The different floating data types for the lattices
    using LatticeFloatingTypes = std::tuple<double, float>;

    // Merge data types and lattices data types
    using LatticeTestTypes = ToTestingTypes_t<lbt::CartesianProductApply_t<LatticeTypes, LatticeFloatingTypes>>;

  }
}

#endif // LBT_LATTICE_TESTING_TYPES
