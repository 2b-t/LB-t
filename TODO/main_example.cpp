/**\file     main.cpp
 * \mainpage Mainpage of the LBM solver
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include "general/type_definitions.hpp"
#include "geometry/vtk_import.hpp"


#include "settings/settings_parsing.hpp"

int main(int argc, char* argv[]) {
    LbmSettings lbm_settings{0, 0, FloatingPointPrecision::Double, std::make_shared<BgkSmagorinsky>()};
    Discretisation discretisation{};
    Physics physics{};
    std::vector<std::shared_ptr<BoundaryCondition>> boundary_conditions = {};
    InitialConditions initial_conditions{};
    Geometry geometry{};
    auto target = std::make_shared<CpuTarget>();
    Output output {};
    std::string start_from{};
    double duration = 0.0;
    
    boundary_conditions.push_back(std::make_shared<SlipBoundaryCondition>());
    boundary_conditions.push_back(std::make_shared<NoSlipBoundaryCondition>());
    boundary_conditions.push_back(std::make_shared<VelocityBoundaryCondition>());
    boundary_conditions.push_back(std::make_shared<PressureBoundaryCondition>());
    boundary_conditions.push_back(std::make_shared<PeriodicBoundaryCondition>());
    
    Settings settings{lbm_settings, discretisation, physics, boundary_conditions, 
                      initial_conditions, geometry, target, output, start_from, duration};


    std::cout << settings.toJson().dump(4) << std::endl;

    // Parse command-line arguments
    if ((argc != 3) && (argc != 4)) {
        std::string const command_name {argv[0]};
        std::cerr << "Converts STL or OBJ geometry files to voxels with the resolution given by resolution_x" << std::endl;
        std::cerr << "Usage: " << command_name << " <filename.stl/.obj> <resolution_x> <reduction_rate>" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::string const filename {argv[1]};
    int const resolution_x {std::atoi(argv[2])};
    double const reduction_rate {(argc == 4) ? std::atof(argv[3]) : 0.0};

    // Check input parameters
    if (std::filesystem::exists(filename) == false) {
        std::cerr << "Error: file '" << filename << "' not found!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (resolution_x <= 0) {
        std::cerr << "Parameter <resolution_x> (" << resolution_x << ") has to be positive!" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Import geometry file
    lbt::ImportGeometry(filename, resolution_x, reduction_rate);

    return EXIT_SUCCESS;
}
