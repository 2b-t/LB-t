/**\file     main.cpp
 * \mainpage Mainpage of the LB-t computational fluid dynamics solver based on the incompressible Lattice-Boltzmann method
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <cstdlib>
#include <iostream>

#include "unit/literals.hpp"
#include "unit/materials.hpp"
#include "general/openmp_manager.hpp"

#include "lattice/lattice.hpp"


int main(int argc, char* argv[]) {
  using namespace lbt::literals;

  // Decide on floating type, lattice and collision operator
  using T = double;
  using Lattice = lbt::lattice::D2Q9<T>;
  // using CollisionOperator = lbt::collision::Bgk

  constexpr auto length = 1.2_m;
  constexpr auto velocity = 1.7_mps;
  constexpr auto temperature = 20.0_deg;
  constexpr auto pressure = 1.0_atm;

  constexpr auto density = lbt::material::Air::density(temperature, pressure);
  constexpr auto kinematic_viscosity = lbt::material::Air::kinematic_viscosity(temperature, pressure);

  auto& omp_manager = lbt::OpenMpManager::getInstance();
  std::cout << omp_manager << std::endl;

  // Create continuum

  // Create population

  // Collision operator

  return EXIT_SUCCESS;
}
