/**
 * \file     main.cpp
 * \mainpage Mainpage of the LB-t computational fluid dynamics solver based on the incompressible Lattice-Boltzmann method
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>

#include "lbt/continuum/continuum.hpp"
#include "lbt/general/openmp_manager.hpp"
#include "lbt/lattice/lattices.hpp"
#include "lbt/material/materials.hpp"
#include "lbt/population/collision_operators.hpp"
#include "lbt/population/population.hpp"
#include "lbt/unit/characteristic_numbers.hpp"
#include "lbt/unit/literals.hpp"
#include "lbt/converter.hpp"


int main(int argc, char* argv[]) {
  using namespace lbt::literals;

  // Flow and material properties
  constexpr auto velocity = 1.7_mps;
  constexpr auto length = 1.2_m;
  using Material = lbt::material::Air;
  constexpr auto temperature = 20.0_deg;
  constexpr auto pressure = 1.0_atm;
  constexpr auto density {Material::equationOfState(temperature, pressure)};
  constexpr auto kinematic_viscosity {Material::kinematicViscosity(temperature, pressure)};
  constexpr lbt::unit::ReynoldsNumber reynolds_number {velocity, length, kinematic_viscosity};

  // Solver settings: Floating type, lattice and collision operator
  constexpr auto simulation_time {3.0_min};
  using T = double;
  using Lattice = lbt::lattice::D2Q9<T>;
  using CollisionOperator = lbt::collision::Bgk<Lattice>;
  constexpr auto lbm_speed_of_sound {lbt::lattice::D2Q9<T>::CS};
  constexpr auto lbm_velocity = 0.5*lbm_speed_of_sound;
  constexpr long double lbm_density {1.0};
  constexpr std::int32_t NX {100};
  constexpr std::int32_t NY {100};
  constexpr std::int32_t NZ {100};
  auto const output_path {std::filesystem::current_path()};
  constexpr lbt::Converter unit_converter {length, static_cast<long double>(NX), velocity, lbm_velocity, density, lbm_density};

  // Multi-threading
  auto& omp_manager = lbt::OpenMpManager::getInstance();
  omp_manager.setThreadsNum(lbt::OpenMpManager::getThreadsMax());

  // Geometry and boundary conditions
  // TODO(tobit): Add boundary conditions

  // Continuum, population and collision operator
  auto continuum {std::make_shared<lbt::Continuum<T>>(NX, NY, NZ, output_path)};
  auto population {std::make_shared<lbt::Population<Lattice>>(NX, NY, NZ)};
  // auto CollisionOperator collision_operator {continuum, population, reynolds_number, lbm_velocity, lbm_length};
  // collision_operator.initialize<lbt::Timestep::Even>({u, v, w}, lbm_density);

  // Convert time-steps to simulation time
  constexpr auto NT {static_cast<std::int64_t>(unit_converter.toLbm(simulation_time))};
  for (std::int64_t i = 0; i < NT; i += 2) {
    // Loop performs two combined iterations at once

    // TODO(tobit): Currently saving every 100th step
    bool const is_save {i % (NT/100)};

    // collision_operator.collideAndStream<lbt::Timestep::Even>(is_save);
    // collision_operator.collideAndStream<lbt::Timestep::Odd>(is_save);

    if (is_save) {
      // Enforce boundary condition on continuum
      auto const simulation_time {unit_converter.toPhysical<lbt::unit::Time>(i)};
      continuum->save(static_cast<double>(simulation_time.get()));
    }
  }

  return EXIT_SUCCESS;
}
