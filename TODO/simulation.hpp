/**\file     simulation.hpp
 * \mainpage Contains the main class for simulation
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <memory>

#include "nlohmann/json.hpp"

#include "continuum/continuum.hpp"
#include "population/collision/collision.hpp"
#include "population/population.hpp"
#include "general/openmp_manager.hpp"
#include "general/output.hpp"

//#include "general/openmp_manager.hpp"
//#include "general/stream_manager.hpp"
//#include "general/timer.hpp"
//#include "geometry/vtk_import.hpp"
//#include "settings/settings_parsing.hpp"

// Parse settings and determine conversion factors
  // Start Multi-threading manager
  // Turn logging on or off, or use log file
  // Logging: std::clog, std::cout, std::cerr, on/off, add file stream
  
  /*auto openmp_manager {lbt::OpenMpManager::getInstance()};
  openmp_manager.setThreadsNum(8);
  std::cout << lbt::disclaimer() << std::endl;

  // Redirect logging output to file or turn off entirely (std::cout, std::cerr, std::cerr independently)
  lbt::StreamManager clog_manager{std::clog};
  clog_manager.turnOff();

  // Check if file given as a command line argument
  // If not default to relative path to it

  // Calculate unit conversion factors for simulation

  // Import filenames from folder
  std::vector<std::string> filenames = {};
  std::filesystem::path const exe_path = std::filesystem::current_path();
  std::filesystem::path const files_path = exe_path / "NewYork";
  for (auto const& e: std::filesystem::directory_iterator(files_path)) {
    filenames.push_back(e.path());
  }
  // Sort filenames alphabetically
  std::sort(filenames.begin(), filenames.end());

  // Import geometry file
  //std::array<double, 6> const bounding_box = {-230.0, 170.0, -120.0, 160.0, 0.0, 160.0};
  lbt::Timer timer {};
  timer.start();
  // Use less cores for geometry import (max 8 threads)
  auto const geometry = lbt::Geometry::importFromFiles(filenames, resolution_x, reduction_rate);
  timer.stop();
  std::cout << "Import ran for " << timer.getRuntime() << " seconds." << std::endl;
  geometry.exportToFile("geometry", lbt::DataType::MHD, true);*/

namespace lbt {

template <template <template <typename T> class LT, typename T, unsigned int NPOP> class CO, 
           template <typename T> class LT, typename T, unsigned int NPOP = 1>
class Simulation {
    using json = nlohmann::json;

    public:
        std::shared_ptr<Continuum<T>> continuum;
        std::shared_ptr<Population<LT,T,NPOP>> population;
        std::shared_ptr<Output<LT,T,NPOP>> output;
        std::shared_ptr<CollisionOperator<LT,T,NPOP>> collision_operator;
        //std::vector<std::shared_ptr<BoundaryCondition<LT,T,NPOP>> boundary_conditions;
        std::vector<std::shared_ptr<StreamManager>> stream_managers;
        OpenMpManager& openmp_manager;

    public:
        Simulation() {
            return;
        }

        // Add general constructor to construct scenarios like used to

        static Simulation parse(json const& j) {
            Simulation simulation {};
            
            std::int32_t const NX = j["discretisation"]["NX"].get<std::int32_t>();
            std::int32_t const NY = j["discretisation"]["NY"].get<std::int32_t>();
            std::int32_t const NZ = j["discretisation"]["NZ"].get<std::int32_t>();
            double const Re = 0.0; // U*L/nu
            double const RHO = 1.0; // 1.0
            double const U = 0.01; // estimate a suiting velocity here
            double const L = 0.1; // Use domain width in pixels
            double const NT = 10; // Determine number of time steps from physical units
            double const NT_PLOT = 1; // Determine plot time-step
            std::int32_t const number_of_cores = 8;

            // Conversion factors for velocity, density, length
            // Unit conversion: only send to macroscopic values

            // Parallelisation
            auto openmp_manager = OpenMpManager::getInstance();
            openmp_manager.setThreadsNum(number_of_cores);

            // Continuum
            simulation.continuum = std::make_shared<Continuum<T>>(NX, NY, NZ);

            // Population
            simulation.population = std::make_shared<Population<LT,T,NPOP>>(NX,NY,NZ);

            // Output
            simulation.output = std::make_shared<Output<LT,T,NPOP>>(simulation.population,simulation.continuum,NX,NY,NZ,Re,RHO,U,L,NT,NT_PLOT);

            // Set collision operator
            // Pointer to collision operator based on virtual functions

            // Import geometry

            // Register boundary conditions
            // Push-back to vector of boundary conditions

            return simulation;
        }

        // Export settings back to Json
        json toJson() const noexcept {
            json j{};
            // Export all information about the simulation to json
            return j;
        }

        // Unit conversion

        // Run simulation
        void run() noexcept {
            // Collision & streaming
            // Apply boundary conditions
            // Export domain and/or population if desired
            return;
        }
};

}
