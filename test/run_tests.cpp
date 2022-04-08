/**
 * \file     run_tests.cpp
 * \mainpage Performs all unit tests for LB-t with or without the VTK library
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <gtest/gtest.h>


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
