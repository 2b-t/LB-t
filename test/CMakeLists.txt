cmake_minimum_required(VERSION 3.1)
project(lbt_tests)

# Set compilation flags
add_definitions(-DNDEBUG)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17) # The vast majority of code is written for C++17 but certain tests require C++20
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Required packages: OpenMP (optional), VTK, nlohmann_json
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP version " ${OpenMP_VERSION} " found!")
else()
  message(WARNING "OpenMP not found!")
endif()

find_package(VTK REQUIRED)
if(NOT VTK_FOUND)
  message(FATAL_ERROR "VTK library not found!")
else()
  message(STATUS "VTK version: " ${VTK_VERSION})
endif()
if(VTK_VERSION VERSION_LESS "8.90.0")
  include(${VTK_USE_FILE})
endif()

find_package(nlohmann_json REQUIRED)
if (NOT nlohmann_json_FOUND)
  message(FATAL_ERROR "nlohmann-json library not found!")
else()
  message(STATUS "nlohmann-json version: " ${nlohmann_json_VERSION})
endif()

enable_testing()
include(GoogleTest)
if (NOT GTest_FOUND)
  # Add executable
  add_executable(run_lbt_tests
                 ../src/general/disclaimer.cpp
                 ../src/general/openmp_manager.cpp
                 ../src/general/timer.cpp
                 ../src/general/vtk_utilities.cpp
                 ../src/geometry/vtk_import.cpp
                 constexpr_math/constexpr_math_test.cpp
                 continuum/simple_continuum_test.cpp
                 continuum/vtk_continuum_test.cpp
                 general/disclaimer_test.cpp
                 general/openmp_manager_test.cpp
                 general/output_utilities_test.cpp
                 general/stream_manager_test.cpp
                 general/timer_test.cpp
                 general/tuple_utilities_test.cpp
                 general/type_definitions_test.cpp
                 general/vtk_utilities_test.cpp
                 geometry/vtk_import_test.cpp
                 lattice/lattice_test.cpp
                 material/materials_test.cpp
                 population/aa_population_test.cpp
                 population/ab_population_test.cpp
                 population/boundary/guo_test.cpp
                 population/boundary/normal_test.cpp
                 population/boundary/orientation_test.cpp
                 population/indexing/aa_pattern_test.cpp
                 population/indexing/indexing_test.cpp
                 testing_utilities/testing_utilities_test.cpp
                 unit/characteristic_numbers_test.cpp
                 unit/literals_test.cpp
                 unit/units_test.cpp
                 converter_test.cpp
                 simulation_test.cpp
                 run_tests.cpp
  )

  gtest_discover_tests(run_lbt_tests
                  TEST_SUFFIX .noArgs
                  TEST_LIST   noArgsTests
  )
  set_tests_properties(${noArgsTests}   PROPERTIES TIMEOUT 10)

  # Link executable to libraries
  if(OpenMP_CXX_FOUND)
    target_link_libraries(run_lbt_tests gtest pthread ${VTK_LIBRARIES} nlohmann_json::nlohmann_json OpenMP::OpenMP_CXX)
  else()
    target_link_libraries(run_lbt_tests gtest pthread ${VTK_LIBRARIES} nlohmann_json::nlohmann_json)
  endif()
else()
  message(FATAL_ERROR "GoogleTest not found!")
endif()
