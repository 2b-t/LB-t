cmake_minimum_required(VERSION 3.1)
project(lbt)

if (CMAKE_COMPILER_IS_GNUCC)
  option(ENABLE_COVERAGE "Enable coverage reporting for GCC/Clang" False)

  if (ENABLE_COVERAGE)
    message(STATUS "Building with coverage reporting for GCC/Clang.")
    add_compile_options(--coverage -O0)
    link_libraries(--coverage)
    #set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
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

# Libraries: VTK, nlohmann_json, OpenMP (optional)
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

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP version " ${OpenMP_VERSION} " found!")
else()
  message(WARNING "OpenMP not found: Multi-threading disabled!")
endif()

include_directories(
  include
)

# Add executable
add_library(lbt_obj OBJECT
  src/general/disclaimer.cpp
  src/general/openmp_manager.cpp
  src/general/timer.cpp
  src/general/vtk_utilities.cpp 
  src/geometry/vtk_import.cpp
)
add_executable(lbt 
  $<TARGET_OBJECTS:lbt_obj>
  src/main.cpp
)

# Link executable to libraries
set(LBT_LIBRARIES ${VTK_LIBRARIES} nlohmann_json::nlohmann_json)
if(OpenMP_CXX_FOUND)
  set(LBT_LIBRARIES ${LBT_LIBRARIES} OpenMP::OpenMP_CXX)
endif()
target_link_libraries(lbt_obj ${LBT_LIBRARIES})
target_link_libraries(lbt ${LBT_LIBRARIES})


# Testing
enable_testing()

include(GoogleTest)
if (NOT GTest_FOUND)
  set(LBT_TEST_LIBRARIES ${LBT_LIBRARIES} gtest pthread)

  # Add executable
  add_library(lbt_test_obj OBJECT
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
  )
  add_executable(run_lbt_tests
    $<TARGET_OBJECTS:lbt_obj>
    $<TARGET_OBJECTS:lbt_test_obj>
    test/run_tests.cpp
  )

  target_compile_definitions(run_lbt_tests PUBLIC NDEBUG)

  gtest_discover_tests(run_lbt_tests
    TEST_SUFFIX .noArgs
    TEST_LIST noArgsTests
  )
  set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 10)

  # Link executable to libraries
  target_link_libraries(lbt_test_obj ${LBT_TEST_LIBRARIES})
  target_link_libraries(run_lbt_tests ${LBT_TEST_LIBRARIES})
else()
  message(FATAL_ERROR "GoogleTest not found!")
endif()
