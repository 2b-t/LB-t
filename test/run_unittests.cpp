/**\file     run_unittests.cpp
 * \mainpage Performs all unit tests for LB-t with and without the VTK library
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <gtest/gtest.h>

#include "continuum/characteristic_numbers_unittest.hpp"
// TODO: Implement these unit-tests
#include "continuum/simple_continuum_unittest.hpp"
#include "general/constexpr_math_unittest.hpp"
#include "general/disclaimer_unittest.hpp"
#include "general/literals_unittest.hpp"
#include "general/openmp_manager_unittest.hpp"
#include "general/output_utilities_unittest.hpp"
#include "general/stream_manager_unittest.hpp"
#include "general/timer_unittest.hpp"
#include "general/tuple_utilities_unittest.hpp"
#include "general/type_definitions_unittest.hpp"
#include "general/units_unittest.hpp"
#include "lattice/lattice_unittest.hpp"
// TODO: Implement these unit-tests
#include "population/indexing/aa_pattern_unittest.hpp"
#include "population/indexing/indexing_unittest.hpp"
// TODO: Double check these two headers
#include "population/aa_population_unittest.hpp"
#include "population/ab_population_unittest.hpp"
#include "testing_utilities/testing_utilities_unittest.hpp"
#include "simulation_unittest.hpp"

#if __has_include (<vtkSmartPointer.h>)
  // TODO: Implement these unit tests
  #include "continuum/vtk_continuum_unittest.hpp"
  #include "general/vtk_utilities_unittest.hpp"
  #include "geometry/vtk_import_unittest.hpp"
#endif


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
