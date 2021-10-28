/**\file     run_unittests.cpp
 * \mainpage Performs all unit tests for LB-t
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <gtest/gtest.h>

#include "continuum/continuum_unittest.hpp"
#include "general/constexpr_math_unittest.hpp"
#include "general/disclaimer_unittest.hpp"
#include "general/stream_manager_unittest.hpp"
#include "general/timer_unittest.hpp"
#include "lattice/lattice_unittest.hpp"
#include "simulation_unittest.hpp"


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
