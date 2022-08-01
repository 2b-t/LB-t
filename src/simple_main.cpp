/**
 * \file     main.cpp
 * \mainpage Mainpage of the LB-t computational fluid dynamics solver based on the incompressible Lattice-Boltzmann method
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "general/openmp_manager.hpp"
#include "lattice/lattice.hpp"
#include "material/materials.hpp"
#include "unit/literals.hpp"



int main(int argc, char* argv[]) {
  using namespace lbt::literals;

  // Decide on floating type, lattice and collision operator
  using T = double;
  using Lattice = lbt::lattice::D2Q9<T>;
  // using CollisionOperator = lbt::collision::Bgk
  // Solver settings: Microscopic velocity: Set manually

  constexpr auto length = 1.2_m;
  constexpr auto velocity = 1.7_mps;
  constexpr auto temperature = 0.0_deg;
  constexpr auto pressure = 1.0_atm;
  constexpr auto air = lbt::material::Air(temperature, pressure);
  constexpr auto hydrogen = lbt::material::CarbonDioxide(temperature, pressure);
  std::cout << hydrogen << std::endl;
  std::cout << hydrogen.dynamicViscosity(temperature) << std::endl;
  std::cout << air.dynamicViscosity(temperature, pressure) << std::endl;

  // Compute Reynolds number as well as conversion factors and pass them to the collision operator class
  // Let collision operator class handle unit conversion

  auto& omp_manager = lbt::OpenMpManager::getInstance();
  //omp_manager.setThreadsNum(2);
  std::cout << omp_manager << std::endl;

  // Geometry: Determine boundary conditions either from analytical geometry description

  // Create continuum

  // Create population

  // Collision operator

  return EXIT_SUCCESS;
}
