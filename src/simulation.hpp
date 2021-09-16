/**\file     simulation.hpp
 * \mainpage Contains the templated class for simulation
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdlib>
#include <filesystem>
#include <memory>

#include "nlohmann/json.hpp"

#include "continuum/continuum.hpp"
/*#include "population/collision/collision.hpp"
#include "population/population.hpp"
#include "general/openmp_manager.hpp"
#include "general/output.hpp"*/
#include "base_simulation.hpp"

namespace lbt {

  // Add documentation
  template <template <typename T> class LT, typename T, unsigned int NPOP = 1>
  class Simulation final: public BaseSimulation {
    using json = nlohmann::json;

    public:
      std::shared_ptr<Continuum<T>> continuum;
      //std::shared_ptr<Population<LT,T,NPOP>> population;
      //std::shared_ptr<Output<LT,T,NPOP>> output;
      //std::shared_ptr<CollisionOperator<LT,T,NPOP>> collision_operator;

      //std::vector<std::shared_ptr<BoundaryCondition<LT,T,NPOP>> boundary_conditions;
      //std::vector<std::shared_ptr<StreamManager>> stream_managers;
      //std::unique_ptr<OpenMpManager> openmp_manager;

    public:
      Simulation() = delete;

      Simulation(json const& settings, std::filesystem::path const& output_directory) {
        // Parse settings
        std::int32_t const NX {settings["discretisation"]["NX"].get<std::int32_t>()};
        std::int32_t const NY {settings["discretisation"]["NY"].get<std::int32_t>()};
        std::int32_t const NZ {settings["discretisation"]["NZ"].get<std::int32_t>()};

        // Discretisation
        // Velocity
        // Density
        // kinematic viscosity
        // Geometry files
        // Bounding box
        // Initial conditions
        // Parallelism
        // Export steps


        // Based on geometry make estimates for LBM velocity

        // Determine conversion factors
        // Add conversion factors to macroscopic calculation
        
        // Calculate dimensionless numbers

        // Initialise simulation:
        // Add continuum
        continuum = std::make_shared<Continuum<T>>(NX, NY, NZ, output_directory);
        // Add population
        // Add collision operator
        // Add boundary conditions all to vector of boundary conditions

        // Configure console output and parallelism
        
        return;
      }

      // Export settings back to Json
      json toJson() const noexcept override final {
        json settings {};
        return settings;
      }

      // Run simulation
      void run() noexcept override final {
        // Collision & streaming
        // Apply boundary conditions
        // Export domain and/or population if desired
        return;
      }
  };

}
