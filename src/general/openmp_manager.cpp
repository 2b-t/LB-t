#include "general/openmp_manager.hpp"

#include <ostream>

#if __has_include (<omp.h>)
    #include <omp.h>
#endif


namespace lbt {

  OpenMpManager& OpenMpManager::getInstance() noexcept {
    static OpenMpManager openmp_manager {};
    return openmp_manager;
  }

  bool OpenMpManager::setNestedParallelism(bool const is_nested) noexcept {
    bool is_success {false};
    #ifdef _OPENMP
    ::omp_set_nested(is_nested);
    is_success = true;
    #endif
    return is_success;
  }

  bool OpenMpManager::setThreadsNum(int const number_of_threads) noexcept {
    bool is_success {false};

    #ifdef _OPENMP
    if ((number_of_threads > 0) && (number_of_threads <= threads_max)) {
      threads_num = number_of_threads;
      ::omp_set_num_threads(threads_num);
      is_success = true;
    }
    #endif

    return is_success;
  }

  int OpenMpManager::getThreadsMax() noexcept {
    return threads_max;
  }

  int OpenMpManager::getThreadsNum() noexcept {
    #ifdef _OPENMP
    int const max_number_of_threads {::omp_get_max_threads()};
    #else
    int const max_number_of_threads {1};
    #endif
    return max_number_of_threads;
  }

  int OpenMpManager::getThreadsCurrent() noexcept {
    #ifdef _OPENMP
    int const number_of_active_threads {::omp_get_num_threads()};
    #else
    int const number_of_active_threads {1};
    #endif
    return number_of_active_threads;
  }

  std::ostream& operator<< (std::ostream& os, OpenMpManager const& openmp_manager) noexcept {
    os << "Threads (active/max): " << openmp_manager.threads_num << "/" << openmp_manager.threads_max;
    return os;
  }

  OpenMpManager::OpenMpManager() noexcept {
    OpenMpManager::setThreadsNum(threads_num);
    return;
  }

  #ifdef _OPENMP
  int OpenMpManager::threads_max {::omp_get_num_procs()};
  int OpenMpManager::threads_num {::omp_get_num_procs()};
  #else
  int OpenMpManager::threads_max {1};
  int OpenMpManager::threads_num {1};
  #endif

}

