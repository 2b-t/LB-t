/**\file     run_tests.cpp
 * \mainpage Performs all unit tests for LB-t with or without the VTK library
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <gtest/gtest.h>

//#include "continuum/simple_continuum_test.hpp"
#include "general/constexpr_math_test.hpp"
#include "general/disclaimer_test.hpp"
#include "general/openmp_manager_test.hpp"
#include "general/output_utilities_test.hpp"
#include "general/stream_manager_test.hpp"
#include "general/timer_test.hpp"
#include "general/tuple_utilities_test.hpp"
#include "general/type_definitions_test.hpp"
#include "lattice/lattice_test.hpp"
#include "population/boundary/guo_test.hpp"
#include "population/boundary/normal_test.hpp"
#include "population/boundary/orientation_test.hpp"
#include "population/indexing/aa_pattern_test.hpp"
#include "population/indexing/indexing_test.hpp"
#include "population/aa_population_test.hpp"
#include "population/ab_population_test.hpp"
#include "testing_utilities/testing_utilities_test.hpp"
#include "unit/characteristic_numbers_test.hpp"
#include "unit/literals_test.hpp"
#include "unit/units_test.hpp"
#include "unit/material/air_test.hpp"
#include "unit/material/ideal_gas_test.hpp"
#include "unit/material/water_test.hpp"
#include "simulation_test.hpp"

// Only compiled if VTK is installed
#include "continuum/vtk_continuum_test.hpp"
#include "general/vtk_utilities_test.hpp"
#include "geometry/vtk_import_test.hpp"


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
