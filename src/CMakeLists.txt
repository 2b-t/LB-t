cmake_minimum_required(VERSION 3.1)
PROJECT(lbt)

# Required packages: OpenMP (optional), VTK, nlohmann_json
find_package(OpenMP)
find_package(VTK REQUIRED)
find_package(nlohmann_json REQUIRED)

if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP " ${OpenMP_VERSION} "found!")
else()
  message(WARNING "OpenMP not found!")
endif()
if(NOT VTK_FOUND)
  message(FATAL_ERROR "VTK library not found!")
else()
  message(STATUS "VTK version: " ${VTK_VERSION})
endif()
if (NOT nlohmann_json_FOUND)
  message(FATAL_ERROR "nlohmann-json library not found!")
else()
  message(STATUS "nlohmann-json version: " ${nlohmann_json_VERSION})
endif()

# Set compilation flags
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(VTK_VERSION VERSION_LESS "8.90.0")
  include(${VTK_USE_FILE})
endif()

# Add executable
add_executable(lbt 
               main.cpp
               general/disclaimer.cpp general/openmp_manager.cpp general/timer.cpp general/vtk_utilities.cpp 
               geometry/vtk_import.cpp
              )

# Link executable to libraries
if(OpenMP_CXX_FOUND)
  target_link_libraries(lbt ${VTK_LIBRARIES} nlohmann_json::nlohmann_json OpenMP::OpenMP_CXX)
else()
  target_link_libraries(lbt ${VTK_LIBRARIES} nlohmann_json::nlohmann_json)
endif()
