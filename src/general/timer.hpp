#ifndef LBT_TIMER
#define LBT_TIMER
#pragma once

/**
 * \file     timer.hpp
 * \mainpage An artificial class that allows for a convenient usage of a stopwatch.
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <chrono>


namespace lbt {

  /**\class Timer
   * \brief Class for stopwatch
  */
  class Timer {
    public:
      /**\fn    Timer
       * \brief Constructor
      */
      Timer() noexcept;

      /**\fn    start
       * \brief Start stopwatch
      */
      void start() noexcept;

      /**\fn     stop
       * \brief  Stop stopwatch
       *
       * \return Runtime in seconds
      */
      double stop() noexcept;

      /**\fn     getRuntime
       * \brief  Return runtime in seconds
       *
       * \return Runtime in seconds
      */
      double getRuntime() const noexcept;

    private:
      std::chrono::high_resolution_clock::time_point start_time; ///< chrono library objects for start and end
      std::chrono::high_resolution_clock::time_point stop_time;
      std::chrono::duration<double> start_to_stop;               ///< chrono library objects for duration
      double runtime;                                            ///< double value of runtime in seconds
  };

}

#endif // LBT_TIMER
