cmake_minimum_required(VERSION 3.1)
project(lbt)

if (CMAKE_COMPILER_IS_GNUCC)
  option(ENABLE_COVERAGE "Enable coverage reporting for GCC/Clang" False)

  if (ENABLE_COVERAGE)
    message(STATUS "Building with coverage reporting for GCC/Clang.")
    add_compile_options(--coverage -O0)
    link_libraries(--coverage)
  else ()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  endif()
else()
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# Set compilation flags
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17) # The vast majority of code is written for C++17 but certain tests require C++20
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Libraries: OpenMP (optional), VTK, nlohmann_json
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP version " ${OpenMP_VERSION} " found!")
else()
  message(WARNING "OpenMP not found: Multi-threading disabled!")
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

include_directories(include)

# Add executable
add_executable(lbt 
  src/general/disclaimer.cpp
  src/general/openmp_manager.cpp
  src/general/timer.cpp
  src/general/vtk_utilities.cpp 
  src/geometry/vtk_import.cpp
  src/main.cpp
)

# Link executable to libraries
target_link_libraries(lbt ${VTK_LIBRARIES} nlohmann_json::nlohmann_json)
if(OpenMP_CXX_FOUND)
  target_link_libraries(lbt OpenMP::OpenMP_CXX)
endif()


# Testing
enable_testing()

include(GoogleTest)
if (NOT GTest_FOUND)
  # Add executable
  add_executable(run_lbt_tests
    src/general/disclaimer.cpp
    src/general/openmp_manager.cpp
    src/general/timer.cpp
    src/general/vtk_utilities.cpp
    src/geometry/vtk_import.cpp
    test/constexpr_math/constexpr_math_test.cpp
    test/continuum/simple_continuum_test.cpp
    test/continuum/vtk_continuum_test.cpp
    test/general/disclaimer_test.cpp
    test/general/openmp_manager_test.cpp
    test/general/output_utilities_test.cpp
    test/general/stream_manager_test.cpp
    test/general/timer_test.cpp
    test/general/tuple_utilities_test.cpp
    test/general/type_definitions_test.cpp
    test/general/vtk_utilities_test.cpp
    test/geometry/vtk_import_test.cpp
    test/lattice/lattice_test.cpp
    test/material/materials_test.cpp
    test/population/aa_population_test.cpp
    test/population/ab_population_test.cpp
    test/population/boundary/guo_test.cpp
    test/population/boundary/normal_test.cpp
    test/population/boundary/orientation_test.cpp
    test/population/indexing/aa_pattern_test.cpp
    test/population/indexing/indexing_test.cpp
    test/testing_utilities/testing_utilities_test.cpp
    test/unit/characteristic_numbers_test.cpp
    test/unit/literals_test.cpp
    test/unit/units_test.cpp
    test/converter_test.cpp
    test/simulation_test.cpp
    test/run_tests.cpp
  )

  target_compile_definitions(run_lbt_tests PUBLIC NDEBUG)

  gtest_discover_tests(run_lbt_tests
    TEST_SUFFIX .noArgs
    TEST_LIST noArgsTests
  )
  set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 10)

  # Link executable to libraries
  target_link_libraries(run_lbt_tests gtest pthread ${VTK_LIBRARIES} nlohmann_json::nlohmann_json)
  if(OpenMP_CXX_FOUND)
    target_link_libraries(run_lbt_tests OpenMP::OpenMP_CXX)
  endif()
else()
  message(FATAL_ERROR "GoogleTest not found!")
endif()
