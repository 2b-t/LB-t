#include "general/disclaimer.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

#if __has_include (<omp.h>)
    #include <omp.h>
#endif


namespace lbt {
  std::string disclaimer() noexcept {
    std::stringstream ss {};
    ss << "Author: Tobit Flatscher - github.com/2b-t (2021)" << std::endl;
    ss << std::endl;
    ss << "Compilation Settings" << std::endl;
    ss << "Compiled on ";
    #ifdef __linux__
    ss << "Linux";
    #elif _WIN64
    ss << "Windows 64-bit";
    #elif _WIN32
    ss << "Windows 32-bit";
    #else
    ss << "<unknown operating system>";
    #endif
    ss << " with ";
    #if defined(__ICC)
    ss << "ICC (Linux) "   << __ICC << std::endl;
    #elif defined(__ICL)
    ss << "ICC (Windows) " << __ICL << std::endl;
    #elif defined(__clang__)
    ss << "Clang " << __clang_major__      << "."
                   << __clang_minor__      << "."
                   << __clang_patchlevel__ << std::endl;
    #elif defined(__GNUC__) && !defined(__clang__)
    ss << "GCC " << __GNUC__            << "."
                 << __GNUC_MINOR__      << "."
                 << __GNUC_PATCHLEVEL__ << std::endl;
    #elif defined(_MSC_BUILD)
    ss << "Microsoft Visual C++ (MSVC) " << _MSC_VER << std::endl;
    #else
    ss << "<unknown compiler>" << std::endl;
    #endif

    #ifdef __cplusplus
    ss << " C++";
    #if __cplusplus > 201700
    ss << "17";
    #endif
    ss << " standard (version " << __cplusplus / 100 << "."
                                << __cplusplus % 100 << ")" << std::endl;
    #endif

    ss << " OpenMP: ";
    #ifdef _OPENMP
    std::unordered_map<unsigned int, std::string> const omp_version_map {{200505,"2.5"},
                                                                         {200805,"3.0"},
                                                                         {201107,"3.1"},
                                                                         {201307,"4.0"},
                                                                         {201511,"4.5"},
                                                                         {201811,"5.0"},
                                                                         {202011,"5.1"}};

    std::string omp_version {"<unknown>"};
    if (omp_version_map.find(_OPENMP) != omp_version_map.end()) {
      omp_version = omp_version_map.at(_OPENMP);
    }
    ss << "version " << omp_version << " ";
    int const omp_curr_threads = ::omp_get_max_threads();
    int const omp_max_threads  = ::omp_get_num_procs();
    ss << "(currently running on " << omp_curr_threads
       << "/" << omp_max_threads << " threads)" << std::endl;
    #else
    ss << "<not available>" << std::endl;
    #endif

    ss << " Vector intrinsics: ";
    #ifdef __AVX512CD__
    ss << "AVX512 (512 bit, 8 doubles, 16 floats)" << std::endl;
    #elif __AVX2__
    ss << "AVX2 (256 bit, 4 doubles, 8 floats)"    << std::endl;
    #else
    ss << "<not supported>" << std::endl;
    #endif

    return ss.str();
  }
}
