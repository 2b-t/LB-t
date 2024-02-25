/**
 * \file     main.cpp
 * \mainpage Mainpage of the LB-t computational fluid dynamics solver based on the incompressible Lattice-Boltzmann method
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include "lbt/general/disclaimer.hpp"
#include "lbt/general/openmp_manager.hpp"
#include "lbt/lattice/D3Q15.hpp"
#include "lbt/simulation.hpp"


int main(int argc, char* argv[]) {
  using json = nlohmann::json;

  /// Check input arguments
  if ((argc != 1) && (argc != 2)) {
    std::string const command_name {argv[0]};
    std::cerr << "LB-t: Computational fluid dynamics simulation with the Lattice-Boltzmann method" << std::endl;
    std::cerr << "Usage: " << command_name << " <settings.json>" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  if ((argc == 2) && ((std::strcmp(argv[1], "--version") == 0) || (std::strcmp(argv[1], "-v") == 0))) {
    std::cout << lbt::disclaimer() << std::endl;
    std::exit(EXIT_SUCCESS);
  }

  // Parse input (defaults to "settings.json")
  std::filesystem::path file_path {std::filesystem::current_path() / std::string{"settings.json"}};
  if (argc == 2) {
    file_path = argv[1];
  }
  // Check if settings file exists
  if (std::filesystem::exists(file_path) == false) {
    std::cerr << "Error: file '" << file_path << "' not found!" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::filesystem::path const output_path {file_path.parent_path()};
  std::ifstream filestream {file_path.string()};
  json settings {};
  filestream >> settings;

  /// Set-up and run simulation
  std::shared_ptr<lbt::BaseSimulation> simulation {nullptr};
  // TODO: Add different possible discretisations and floating-point accuracies here
  simulation = std::make_shared<lbt::Simulation<lbt::lattice::D3Q15, double>>(settings, output_path);
  // TODO: Add possibility to run and stop the simulation periodically
  simulation->run();

  return EXIT_SUCCESS;
}
