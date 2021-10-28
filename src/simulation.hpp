#ifndef LBT_SIMULATION
#define LBT_SIMULATION
#pragma once

/**\file     simulation.hpp
 * \mainpage Contains the templated class for simulation
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdlib>
#include <filesystem>
#include <memory>
#include <optional>
#include <tuple>
#include <type_traits>

#include <nlohmann/json.hpp>

#include "continuum/continuum.hpp"
/*#include "population/collision/collision.hpp"
#include "population/population.hpp"
#include "general/openmp_manager.hpp"
#include "general/output.hpp"*/
#include "geometry/vtk_import.hpp"
#include "general/type_definitions.hpp"
#include "base_simulation.hpp"

namespace lbt {
  /**\fn    parseArray
   * \brief Function for parsing a three-dimensional array from a given json string
   * 
   * \tparam    T   The numerical data type of the array
   * \param[in] j   The input json to be parsed to a three-dimensional array (x, y, z)
   * \return    The input json array parsed to a three-dimensional array of type \p T
  */
  template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
  lbt::array<T, 3> parseArray(nlohmann::json const& j) {
    lbt::array<T, 3> const arr {j["x"].get<T>(), j["y"].get<T>(), j["z"].get<T>()};
    return arr;
  }

  template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
  nlohmann::json toJson(lbt::array<T, 3> const& arr) noexcept {
    nlohmann::json j {};
    j["x"] = arr.at(0);
    j["y"] = arr.at(1);
    j["z"] = arr.at(2);
    return j;
  }

  namespace settings {

    class Discretisation {
      using json = nlohmann::json;

      public:
        constexpr Discretisation(std::int32_t const& NX, std::int32_t const& NY, std::int32_t const& NZ) noexcept 
          : NX{NX}, NY{NY}, NZ{NZ} {
          return;
        };

        Discretisation(json const& j) {
          NX = j["NX"].get<std::int32_t>();
          NY = j["NY"].get<std::int32_t>();
          NZ = j["NZ"].get<std::int32_t>();
          return;
        }

        json toJson() const noexcept {
          json j {};
          j["NX"] = NX;
          j["NY"] = NY;
          j["NZ"] = NZ;
          return j;
        }

        std::tuple<std::int32_t, std::int32_t, std::int32_t> getDiscretisation() const noexcept {
          return std::make_tuple(NX, NY, NZ);
        }
      protected:
        std::int32_t NX;
        std::int32_t NY;
        std::int32_t NZ;
    };

    class Physics {
      using json = nlohmann::json;

      public:
        constexpr Physics(double const density, double const kinematic_viscosity) noexcept 
          : density{density}, kinematic_viscosity{kinematic_viscosity} {
          return;
        }

        Physics(json const& j) {
          density = j["density"].get<double>();
          kinematic_viscosity = j["kinematicViscosity"].get<double>();
          return;
        }

        json toJson() const noexcept {
          json j {};
          j["density"] = density;
          j["kinematicViscosity"] = kinematic_viscosity;
          return j;
        }

      protected:
        double density;
        double kinematic_viscosity;
    };

    template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    class InitialConditions {
      using json = nlohmann::json;

      public:
        constexpr InitialConditions(lbt::array<T, 3> const& initial_velocity) noexcept 
          : initial_velocity{initial_velocity} {
          return;
        }

        InitialConditions(json const& j) {
          initial_velocity = parseArray<T>(j["velocity"]);
          return;
        }

        json toJson() const noexcept {
          return toJson(initial_velocity);
        }
      
      protected:
        lbt::array<T, 3> initial_velocity;
    };

    class Geometry {
      using json = nlohmann::json;

      public:
        Geometry(std::vector<std::string> const& files, std::array<double, 6> const& bounding_box,
                 double const reduction_rate) noexcept
          : files{files}, bounding_box{bounding_box}, reduction_rate{reduction_rate} {
          return;
        }

        Geometry(json const& j) {
          json const& j_files {j["models"]};
          for (auto const& f: j_files) {
            files.emplace_back(f.get<std::string>());
          }
          json const bounding_box_min {j["boundingBox"]["min"]};
          json const bounding_box_max {j["boundingBox"]["max"]};
          std::array<double, 6> const bounding_box {bounding_box_min["x"], bounding_box_max["x"],
                                                    bounding_box_min["y"], bounding_box_max["y"],
                                                    bounding_box_min["z"], bounding_box_max["z"]};
          if (j.contains("reductionRate") == true) {
            reduction_rate = j["reductionRate"];
          }
          return;
        }

        json toJson() const {
          json ja {};
          for (auto const& f:files) {
            ja.push_back(f);
          }
          json j {};
          j["models"] = ja;
          
          j["boundingBox"]["min"] = lbt::toJson(lbt::array<double,3>{bounding_box.at(0), bounding_box.at(2), bounding_box.at(4)});
          j["boundingBox"]["max"] = lbt::toJson(lbt::array<double,3>{bounding_box.at(1), bounding_box.at(3), bounding_box.at(5)});

          if (reduction_rate != std::nullopt) {
            j["reductionRate"] = reduction_rate.value();
          }
          return j;
        }

        std::vector<std::filesystem::path> getFilesWithPath(std::filesystem::path const& parent_directory) const noexcept {
          std::vector<std::filesystem::path> files_with_path {};
          for (auto const& f: files) {
            files_with_path.emplace_back(parent_directory / f);
          }
          return files_with_path;
        }
        
      protected:
        std::vector<std::string> files;
        std::array<double, 6> bounding_box;
        std::optional<double> reduction_rate;
    };

    class Parallelism {
      using json = nlohmann::json;

      public:
        constexpr Parallelism(int const number_of_threads) noexcept 
          : number_of_threads{number_of_threads} {
          return;
        }

        Parallelism(json const& j) {
          number_of_threads = j["numberOfThreads"].get<int>();
          return;
        }

        json toJson() const noexcept {
          json j {};
          j["numberOfThreads"] = number_of_threads;
          return j;
        }

      protected:
        int number_of_threads ;
    };

    class Output {
      using json = nlohmann::json;

      public:
        Output(DataType const format, std::string const& folder, double const first_output, 
              double const write_interval) noexcept 
          : format{format}, folder{folder}, first_output{first_output}, write_interval{write_interval} {
          return;
        }

        Output(json const& j) {
          format = DataType::MHD;
          if (j["dataFormat"] == "vtk") {
            format = DataType::VTK;
          }
          folder = j["outputFolder"].get<std::string>();
          first_output = j["firstOutput"].get<double>();
          write_interval = j["writeInterval"].get<double>();
          return;
        }

        json toJson() const noexcept {
          json j {};
          if (format == DataType::VTK) {
            j["dataFormat"] = "vtk";
          } else if (format == DataType::MHD) {
            j["dataFormat"] == "mhd";
          }
          j["outputFolder"] = folder;
          j["firstOutput"] = first_output;
          j["writeInterval"] = write_interval;
          return j;
        }

        std::filesystem::path getFullOutputPath(std::filesystem::path const& parent_directory) const noexcept {
          std::filesystem::path const full_path {parent_directory / folder};
          return full_path;
        }     

      protected:
        DataType format;
        std::string folder;
        double first_output;
        double write_interval;
    };

    class Times {
      using json = nlohmann::json;

      public:
        constexpr Times(double const warmup, double const end) noexcept 
          : warmup{warmup}, end{end} {
          return;
        }

        Times(json const& j) {
          warmup = j["warmUp"].get<double>();
          if (j.contains("startTime") == true) {
            start = j["startTime"];
          }
          end = j["endTime"].get<double>();
          return;
        }

        json toJson() const noexcept {
          json j {};
          j["warmUp"] = warmup;
          if (start != std::nullopt) {
            j["startTime"] = start.value();
          }
          j["endTime"] = end;
          return j;
        }

      protected:
        double warmup;
        std::optional<double> start;
        double end;
    };
  }
  
  /**\class  Simulation
   * \brief  Templated simulation class
   *
   * \tparam LT     Static lattice::DdQq class containing discretisation parameters
   * \tparam T      Floating data type used for simulation
   * \tparam NPOP   Number of populations stored side by side in a single merged grid
  */
  template <template <typename T> class LT, typename T, unsigned int NPOP = 1>
  class Simulation final: public BaseSimulation {
    using json = nlohmann::json;

    public:
      Simulation() = delete;

      /**\fn    Simulation
       * \brief Constructor of Simulation class that parses the settings from a json string
       * 
       * \param[in] settings           The json string containing all the settings
       * \param[in] parent_directory   The directory where the simulation results should be outputted to
      */
      Simulation(json const& settings, std::filesystem::path const& parent_directory) {
        /// Parse settings
        settings::Discretisation const discretisation {settings["discretisation"]};
        settings::Physics const physics {settings["physics"]};
        settings::InitialConditions<T> const initial_conditions {settings["initial_conditions"]};
        settings::Geometry const geometry {settings["geometry"]};
        settings::Parallelism const parallelism {settings["target"]};
        settings::Output const output {settings["output"]};
        settings::Times const times {settings["times"]};

        // Import geometry files
        /*auto const geometry = Geometry::importFromFiles(geometry_files, NX, reduction_rate, bounding_box);

        /// Auto-generate LBM solver settings
        // Determine maximum velocity
        auto real_max_velocity {real_initial_velocity};
        json const& boundary_conditions {settings["boundaryConditions"]};
        for (auto const& b: boundary_conditions) {
          if (b["boundaryType"] == "velocity") {
            if (std::abs(b["velocity"]["x"].get<T>()) > std::abs(real_max_velocity[0])) {
              real_max_velocity[0] = b["velocity"]["x"];
            }
            if (std::abs(b["velocity"]["y"].get<T>()) > std::abs(real_max_velocity[1])) {
              real_max_velocity[1] = b["velocity"]["y"];
            }
            if (std::abs(b["velocity"]["z"].get<T>()) > std::abs(real_max_velocity[2])) {
              real_max_velocity[2] = b["velocity"]["z"];
            }
          }
        }
        // Based on geometry (each direction independently) make estimates for LBM velocity
        constexpr lbt::array<T, 3> contraction_estimate {4.0, 4.0, 4.0};
        for (std::size_t i = 0; i < real_max_velocity.size(); ++i) {
          real_max_velocity *= contraction_estimate.at(i);
        }
        constexpr auto lbm_max_velocity {0.3*LT<T>::CS};*/
        // Downscale all boundary conditions with this factor: translate to LBM

        // Put all parsed information into subclasses and write export functions for subclasses

        /// Determine unit conversion factors
        /*auto const length_conversion_factor {real_length/lbm_length};
        auto const velocity_conversion_factor {real_velocity/lbm_velocity};
        auto const density_conversion_factor {real_density/lbm_density};
        auto const time_conversion_factor {length_conversion_factor/velocity_conversion_factor};
        auto const viscosity_conversion_factor {length_conversion_factor*velocity_conversion_factor};
        auto const pressure_conversion_factor {density_conversion_factor*velocity_conversion_factor*velocity_conversion_factor};*/

        // Initialise simulation:
        // Add continuum
        // continuum = std::make_shared<Continuum<T>>(NX, NY, NZ, parent_directory);
        // Add population
        // Add collision operator
        // Add boundary conditions all to vector of boundary conditions

        // Configure console output and parallelism
        
        return;
      }

      json toJson() const noexcept override final {
        json settings {};
        return settings;
      }

      void run() noexcept override final {
        // Collision & streaming
        // Apply boundary conditions
        // Export domain and/or population if desired
        return;
      }

    private:
      std::shared_ptr<Continuum<T>> continuum;
      //std::shared_ptr<Population<LT,T,NPOP>> population;
      //std::shared_ptr<Output<LT,T,NPOP>> output;
      //std::shared_ptr<CollisionOperator<LT,T,NPOP>> collision_operator;

      //std::vector<std::shared_ptr<BoundaryCondition<LT,T,NPOP>> boundary_conditions;
      //std::vector<std::shared_ptr<StreamManager>> stream_managers;
      //std::unique_ptr<OpenMpManager> openmp_manager;
  };

}

#endif // LBT_SIMULATION