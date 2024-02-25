/**
 * \file     base_simulation.hpp
 * \mainpage Contains the main base class for simulation
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_BASE_SIMULATION
#define LBT_BASE_SIMULATION
#pragma once

#include <nlohmann/json.hpp>


namespace lbt {

  /**\class BaseSimulation
   * \brief Pure abstract base class for simulation classes
  */
  class BaseSimulation {
    using json = nlohmann::json;

    protected:
      /**\fn        BaseSimulation
       * \brief     Default constructor
      */
      constexpr BaseSimulation() noexcept {
        return;
      }

    public:
      /**\fn    run
       * \brief Function for starting the simulation
      */
      virtual void run() noexcept = 0;

      /**\fn    toJson
       * \brief Function for exporting the simulation settings as a *.json file
      */
      virtual json toJson() const noexcept = 0;
  };

}

#endif // LBT_BASE_SIMULATION