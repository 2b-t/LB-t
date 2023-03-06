/**
 * \file     simulation.hpp
 * \mainpage Contains the templated class for simulation
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_SIMULATION
#define LBT_SIMULATION
#pragma once

#include <array>
#include <cstdint>
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
#include "general/output_utilities.hpp"*/
#include "general/vtk_utilities.hpp"
#include "geometry/vtk_import.hpp"
#include "base_simulation.hpp"


namespace lbt {

  /**\fn    parseArray
   * \brief Function for parsing a three-dimensional array from a given Json string
   * 
   * \tparam    T   The numerical data type of the array
   * \tparam    Dummy parameter used for SFINAE
   * \param[in] j   The input json to be parsed to a three-dimensional array (x, y, z)
   * \return    The input json array parsed to a three-dimensional array of type \p T
  */
  template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
  std::array<T, 3> parseArray(nlohmann::json const& j) {
    std::array<T, 3> const arr {j["x"].get<T>(), j["y"].get<T>(), j["z"].get<T>()};
    return arr;
  }

  /**\fn    toJson
   * \brief Function for writing a three-dimensional array to a formatted Json string
   * 
   * \tparam    T   The numerical data type of the array
   * \tparam    Dummy parameter used for SFINAE
   * \param[in] arr   The three-dimensional array (x, y, z) to be written to a formatted Json string
   * \return    The three-dimensional array of type \p T as a formatted Json string
  */
  template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
  nlohmann::json toJson(std::array<T, 3> const& arr) noexcept {
    nlohmann::json j{};
    j["x"] = arr.at(0);
    j["y"] = arr.at(1);
    j["z"] = arr.at(2);
    return j;
  }

  namespace settings {

    /**\class Discretisation
     * \brief Class holding the number of cells used to resolve the computational domain
    */
    class Discretisation {
      using json = nlohmann::json;

      public:
        /**\fn    Discretisation
         * \brief Constructor of Discretisation class that parses the settings from a Json string
         * 
         * \param[in] NX   Number of cells in x
         * \param[in] NY   Number of cells in y
         * \param[in] NZ   Number of cells in z
        */
        constexpr Discretisation(std::int32_t const NX, std::int32_t const NY, std::int32_t const NZ) noexcept 
          : NX{NX}, NY{NY}, NZ{NZ} {
          return;
        };

        /**\fn    Discretisation
         * \brief Constructor of Discretisation class that parses the settings from a Json string
         * 
         * \param[in] j   The Json string to be parsed
        */
        Discretisation(json const& j) {
          NX = j["NX"].get<std::int32_t>();
          NY = j["NY"].get<std::int32_t>();
          NZ = j["NZ"].get<std::int32_t>();
          return;
        }

        /**\fn    toJson
         * \brief Function for saving the Discretisation parameters to a formatted Json string
         * 
         * \return The Discretisation class serialised as a Json string
        */
        json toJson() const noexcept {
          json j {};
          j["NX"] = NX;
          j["NY"] = NY;
          j["NZ"] = NZ;
          return j;
        }

        /**\fn    getDiscretisation
         * \brief Function for getting the discretisation parameters
         * 
         * \return A tuple containing the discretisation parameters for all three Cartesian directions
        */
        constexpr std::tuple<std::int32_t, std::int32_t, std::int32_t> getDiscretisation() const noexcept {
          return std::make_tuple(NX, NY, NZ);
        }

        /**\fn    getNx
         * \brief Getter for number of cells in x-direction
         * 
         * \return Number of cells in x-direction
        */
        constexpr auto getNx() const noexcept {
          return NX;
        }

        /**\fn    getNy
         * \brief Getter for number of cells in y-direction
         * 
         * \return Number of cells in y-direction
        */
        constexpr auto getNy() const noexcept {
          return NY;
        }

        /**\fn    getNz
         * \brief Getter for number of cells in z-direction
         * 
         * \return Number of cells in z-direction
        */
        constexpr auto getNz() const noexcept {
          return NZ;
        }
      protected:
        std::int32_t NX; // Number of cells in x-direction
        std::int32_t NY; // Number of cells in y-direction
        std::int32_t NZ; // Number of cells in z-direction
    };

    /**\class Physics
     * \brief Class holding the physical parameters of the discretisation
    */
    class Physics {
      using json = nlohmann::json;

      public:
        /**\fn    Physics
         * \brief Constructor of Physics class that parses the settings from a Json string
         * 
         * \param[in] density               Density of the fluid
         * \param[in] kinematic_viscosity   Kinematic viscosity of the fluid
        */
        constexpr Physics(double const density, double const kinematic_viscosity) noexcept 
          : density{density}, kinematic_viscosity{kinematic_viscosity} {
          return;
        }

        /**\fn    Physics
         * \brief Constructor of Physics class that parses the settings from a Json string
         * 
         * \param[in] j   The Json string to be parsed
        */
        Physics(json const& j) {
          density = j["density"].get<double>();
          kinematic_viscosity = j["kinematicViscosity"].get<double>();
          return;
        }

        /**\fn    toJson
         * \brief Function for saving the physics parameters to a formatted Json string
         * 
         * \return The Physics class serialised as a Json string
        */
        json toJson() const noexcept {
          json j {};
          j["density"] = density;
          j["kinematicViscosity"] = kinematic_viscosity;
          return j;
        }

        /**\fn    getDensity
         * \brief Getter for fluid density
         * 
         * \return Fluid density in SI-units
        */
        constexpr auto getDensity() const noexcept {
          return density;
        }

        /**\fn    getKinematicViscosity
         * \brief Getter for kinematic viscosity of the fluid
         * 
         * \return Fluid kinematic viscosity in SI units
        */
        constexpr auto getKinematicViscosity() const noexcept {
          return kinematic_viscosity;
        }

      protected:
        double density; // Density of the fluid
        double kinematic_viscosity; // Kinematic viscosity of the fluid
    };

    /**\class  InitialConditions
     * \brief  Class holding the initial conditions of the simulation
     * 
     * \tparam T   The data type of the underlying arrays
     * \tparam Dummy parameter used for SFINAE
    */
    template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
    class InitialConditions {
      using json = nlohmann::json;

      public:
        /**\fn    InitialConditions
         * \brief Constructor of InitialConditions class that parses the settings from a Json string
         * 
         * \param[in] initial_velocity   Initial velocity inside the domain
        */
        constexpr InitialConditions(std::array<T, 3> const& initial_velocity) noexcept 
          : initial_velocity{initial_velocity} {
          return;
        }

        /**\fn    InitialConditions
         * \brief Constructor of InitialConditions class that parses the settings from a Json string
         * 
         * \param[in] j   The Json string to be parsed
        */
        InitialConditions(json const& j) {
          initial_velocity = parseArray<T>(j["velocity"]);
          return;
        }

        /**\fn    toJson
         * \brief Function for saving the initial condition to a formatted Json string
         * 
         * \return The InitialCondition class serialised as a Json string
        */
        json toJson() const noexcept {
          return lbt::toJson(initial_velocity);
        }

        /**\fn    getInitialVelocity
         * \brief Getter for initial velocity
         * 
         * \return Initial velocity in SI-units
        */
        constexpr auto getInitialVelocity() const noexcept {
          return initial_velocity;
        }

      protected:
        std::array<T, 3> initial_velocity; // Initial velocity inside the domain
    };

    /**\class Geometry
     * \brief Class holding the geometry parameters such as files to consider and bounding box
    */
    class Geometry {
      using json = nlohmann::json;

      public:
        /**\fn    Geometry
         * \brief Constructor of Geometry class that parses the settings from a Json string
         * 
         * \param[in] files            Files to be used for the generation of the geometry
         * \param[in] bounding_box     Bounding box to be considered for the geometries
         * \param[in] reduction_rate   Geometric reduction rate of the polygons as a fraction [0, 1]
        */
        Geometry(std::vector<std::string> const& files, std::array<double, 6> const& bounding_box,
                 double const reduction_rate) noexcept
          : files{files}, bounding_box{bounding_box}, reduction_rate{reduction_rate} {
          return;
        }

        /**\fn    Geometry
         * \brief Constructor of Geometry class that parses the settings from a Json string
         * 
         * \param[in] j   The Json string to be parsed
        */
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

        /**\fn    toJson
         * \brief Function for saving the geometry parameters to a formatted Json string
         * 
         * \return The Geometry class serialised as a Json string
        */
        json toJson() const {
          json ja {};
          for (auto const& f:files) {
            ja.push_back(f);
          }
          json j {};
          j["models"] = ja;
          
          j["boundingBox"]["min"] = lbt::toJson(std::array<double,3>{bounding_box.at(0), bounding_box.at(2), bounding_box.at(4)});
          j["boundingBox"]["max"] = lbt::toJson(std::array<double,3>{bounding_box.at(1), bounding_box.at(3), bounding_box.at(5)});

          if (reduction_rate != std::nullopt) {
            j["reductionRate"] = reduction_rate.value();
          }
          return j;
        }

        /**\fn    getFilesWithPath
         * \brief Function for getting a list of files with their full path
         * 
         * \param[in] parent_directory   The parent directory of the files
         * \return    The full output path for all files
        */
        std::vector<std::filesystem::path> getFilesWithPath(std::filesystem::path const& parent_directory) const noexcept {
          std::vector<std::filesystem::path> files_with_path {};
          for (auto const& f: files) {
            files_with_path.emplace_back(parent_directory / f);
          }
          return files_with_path;
        }

        /**\fn    getBoundingBox
         * \brief Getter for bounding box
         * 
         * \return Geometric bounding box used for the simulation
        */
        constexpr auto getBoundingBox() const noexcept {
          return bounding_box;
        }

        /**\fn    getReductionRate
         * \brief Getter for reduction rate
         * 
         * \return Geometric reduction rate of the polygons
        */
        constexpr auto getReductionRate() const noexcept {
          return reduction_rate;
        }
        
      protected:
        std::vector<std::string> files; // Files to be used for the generation of the geometry
        std::array<double, 6> bounding_box; // Bounding box to be considered for the geometries
        std::optional<double> reduction_rate; // Geometric reduction rate of the polygons
    };

    /**\class Parallelism
     * \brief Class holding the settings for multi-threading parallelism
    */
    class Parallelism {
      using json = nlohmann::json;

      public:
        /**\fn    Parallelism
         * \brief Constructor of Parallelism class that parses the settings from a Json string
         * 
         * \param[in] number_of_threads   Number of CPU threads to be used for computations
        */
        constexpr Parallelism(int const number_of_threads) noexcept 
          : number_of_threads{number_of_threads} {
          return;
        }

        /**\fn    Parallelism
         * \brief Constructor of Parallelism class that parses the settings from a Json string
         * 
         * \param[in] j   The Json string to be parsed
        */
        Parallelism(json const& j) {
          number_of_threads = j["numberOfThreads"].get<int>();
          return;
        }

        /**\fn    toJson
         * \brief Function for saving the parallelism parameters to a formatted Json string
         * 
         * \return The Parallelism class serialised as a Json string
        */
        json toJson() const noexcept {
          json j {};
          j["numberOfThreads"] = number_of_threads;
          return j;
        }

        /**\fn    getNumberOfThreads
         * \brief Getter for number of parallel CPU threads used for simulation and geometry pre-processing
         * 
         * \return Number of CPU threads used for simulation
        */
        constexpr auto getNumberOfThreads() const noexcept {
          return number_of_threads;
        }

      protected:
        int number_of_threads; // Number of CPU threads to be used for computations
    };

    /**\class Output
     * \brief Class holding the settings for output of simulation files
    */
    class Output {
      using json = nlohmann::json;

      public:
        /**\fn    Output
         * \brief Constructor of Output class that parses the settings from a Json string
         * 
         * \param[in] data_type        File format of the output
         * \param[in] folder           Folder name where the files should be written to
         * \param[in] first_output     First time step where a file should be outputted
         * \param[in] write_interval   Interval in between writing time steps
        */
        Output(DataType const data_type, std::string const& folder, double const first_output, 
              double const write_interval) noexcept 
          : data_type{data_type}, folder{folder}, first_output{first_output}, write_interval{write_interval} {
          return;
        }

        /**\fn    Output
         * \brief Constructor of Output class that parses the settings from a Json string
         * 
         * \param[in] j   The Json string to be parsed
        */
        Output(json const& j) {
          data_type = DataType::MHD;
          if (j["dataFormat"] == "vtk") {
            data_type = DataType::VTK;
          }
          folder = j["outputFolder"].get<std::string>();
          first_output = j["firstOutput"].get<double>();
          write_interval = j["writeInterval"].get<double>();
          return;
        }

        /**\fn    toJson
         * \brief Function for saving the output parameters to a formatted Json string
         * 
         * \return The Output class serialised as a Json string
        */
        json toJson() const noexcept {
          json j {};
          if (data_type == DataType::VTK) {
            j["dataFormat"] = "vtk";
          } else if (data_type == DataType::MHD) {
            j["dataFormat"] == "mhd";
          }
          j["outputFolder"] = folder;
          j["firstOutput"] = first_output;
          j["writeInterval"] = write_interval;
          return j;
        }

        /**\fn    getFullOutputPath
         * \brief Function for saving getting the full output path by combining the parent directory with the output folder
         * 
         * \param[in] parent_directory   The parent directory of the folder
         * \return    The full output path
        */
        std::filesystem::path getFullOutputPath(std::filesystem::path const& parent_directory) const noexcept {
          std::filesystem::path const full_path {parent_directory / folder};
          return full_path;
        }

        /**\fn    getFormat
         * \brief Getter for export file format
         * 
         * \return Used export file format
        */
        constexpr auto getFormat() const noexcept {
          return data_type;
        }

        /**\fn    getFirstOutput
         * \brief Getter for first output time step
         * 
         * \return Time step where first output file is written
        */
        constexpr auto getFirstOutput() const noexcept {
          return first_output;
        }

        /**\fn    getWriteInterval
         * \brief Getter for interval at which output file is written
         * 
         * \return Interval at which output file is written
        */
        constexpr auto getWriteInterval() const noexcept {
          return write_interval;
        }

      protected:
        DataType data_type; // File format of the output
        std::string folder; // Folder name where the files should be written to
        double first_output; // First time step where a file should be outputted
        double write_interval; // Interval in between writing time steps
    };

    /**\class Times
     * \brief Class holding the simulation times
    */
    class Times {
      using json = nlohmann::json;

      public:
        /**\fn    Times
         * \brief Constructor of Times class that parses the settings from a Json string
         * 
         * \param[in] warmup   Simulation runtime before the actual simulation starts
         * \param[in] end      Final time step of the simulation
        */
        constexpr Times(double const warmup, double const end) noexcept 
          : warmup{warmup}, end{end} {
          return;
        }

        /**\fn    Times
         * \brief Constructor of Times class that parses the settings from a Json string
         * 
         * \param[in] j   The Json string to be parsed
        */
        Times(json const& j) {
          warmup = j["warmUp"].get<double>();
          if (j.contains("startTime") == true) {
            start = j["startTime"];
          }
          end = j["endTime"].get<double>();
          return;
        }

        /**\fn    toJson
         * \brief Function for saving the timing parameters to a formatted Json string
         * 
         * \return The Time class serialised as a Json string
        */
        json toJson() const noexcept {
          json j {};
          j["warmUp"] = warmup;
          if (start != std::nullopt) {
            j["startTime"] = start.value();
          }
          j["endTime"] = end;
          return j;
        }

        /**\fn    getWarmupTime
         * \brief Getter for warm-up simulation time to be performed before the actual simulation
         * 
         * \return The warm-up time before the simualtion starts in SI-units
        */
        constexpr auto getWarmupTime() const noexcept {
          return warmup;
        }

        /**\fn    getStartTime
         * \brief Getter for optional start time from existing time step
         * 
         * \return The time where the simulation should be foreced to start from in SI-units
        */
        constexpr auto getStartTime() const noexcept {
          return start;
        }

        /**\fn    getEndTime
         * \brief Getter for end time of the simulation
         * 
         * \return The time where the simulation should stop in SI-units
        */
        constexpr auto getEndTime() const noexcept {
          return end;
        }

      protected:
        double warmup; // Simulation runtime before the actual simulation starts
        std::optional<double> start; // Optional time for starting from an existing time step
        double end; // Final time step of the simulation
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
       * \brief Constructor of Simulation class that parses the settings from a Json string
       * 
       * \param[in] settings           The json string containing all the settings
       * \param[in] parent_directory   The directory where the simulation results should be outputted to
      */
      Simulation(json const& settings, std::filesystem::path const& parent_directory)
        : discretisation{settings["discretisation"]}, physics{settings["physics"]},
          initial_conditions{settings["initial_conditions"]}, geometry{settings["geometry"]},
          parallelism{settings["target"]}, output{settings["output"]}, times{settings["times"]} {

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

      /**\fn    toJson
       * \brief Function for saving the simulation parameters to a formatted Json string by calling the underlying setting serialisation functions
       * 
       * \return The Simulation class serialised as a Json string
      */
      json toJson() const noexcept override final {
        json settings {};

        settings["discretisation"] = discretisation.toJson();
        settings["physics"] = physics.toJson();
        settings["initial_conditions"] = initial_conditions.toJson();
        settings["geometry"] = geometry.toJson();
        settings["target"] = parallelism.toJson();
        settings["output"] = output.toJson();
        settings["times"] = times.toJson();

        return settings;
      }

      /**\fn    run
       * \brief Function for running the simulation
      */
      void run() noexcept override final {
        // Collision & streaming
        // Apply boundary conditions
        // Export domain and/or population if desired
        return;
      }

    private:
      settings::Discretisation discretisation;
      settings::Physics physics;
      settings::InitialConditions<T> initial_conditions;
      settings::Geometry geometry;
      settings::Parallelism parallelism;
      settings::Output output;
      settings::Times times;

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
