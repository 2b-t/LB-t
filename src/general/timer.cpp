#include "general/timer.hpp"

#include <chrono>


namespace lbt {

  Timer::Timer() noexcept
    : start_time{std::chrono::high_resolution_clock::now()}, 
      stop_time{std::chrono::high_resolution_clock::now()}, 
      start_to_stop{std::chrono::duration<double>::zero()}, runtime{0.0} {
    return;
  }

  void Timer::start() noexcept {
    start_time = std::chrono::high_resolution_clock::now();
    return;
  }

  double Timer::stop() noexcept {
    stop_time = std::chrono::high_resolution_clock::now();
    start_to_stop = std::chrono::duration_cast<std::chrono::duration<double>>(stop_time - start_time);
    runtime = start_to_stop.count();
    return runtime;
  }

  double Timer::getRuntime() const noexcept {
    std::chrono::high_resolution_clock::time_point const current_time {std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> const start_to_current {std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time)};
    return start_to_current.count();
  }

}
