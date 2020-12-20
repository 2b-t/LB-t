#ifndef LBT_DISCLAIMER
#define LBT_DISCLAIMER

/**
 * \file     disclaimer.hpp
 * \mainpage Print disclaimer and compiler settings to console
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <iostream>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif
#include <unordered_map>


/**\fn    printDisclaimer
 * \brief Print small disclaimer and compiler settings to console
*/
void printDisclaimer() noexcept
{
    std::cout << "Author: Tobit Flatscher - github.com/2b-t (2019/2020)" << std::endl;
    std::cout << std::endl;
    std::cout << "COMPILER SETTINGS" << std::endl;
    std::cout << "Compiled on ";
    #ifdef __linux__
        std::cout << "Linux";
    #elif _WIN64
        std::cout << "Windows 64bit";
    #elif _WIN32
        std::cout << "Windows 32bit";
    #endif
    std::cout << " with ";
    #ifdef __ICC
        std::cout << "ICC (Linux) "   << __ICC << std::endl;
    #elif __ICL
        std::cout << "ICC (Windows) " << __ICL << std::endl;
    #elif __GNUC__
        std::cout << "GCC " << __GNUC__            << "."
                            << __GNUC_MINOR__      << "."
                            << __GNUC_PATCHLEVEL__ << std::endl;
    #else
        std::cout << "unknown compiler" << std::endl;
    #endif

    #ifdef __cplusplus
        std::cout << " C++";
    #if __cplusplus > 201700
        std::cout << "17";
    #endif
        std::cout << " standard (version " << __cplusplus / 100 << "."
                                           << __cplusplus % 100 << ")" << std::endl;
    #endif

    std::cout << " OpenMP ";
    #ifdef _OPENMP
        std::unordered_map<unsigned int,std::string> const mapOMP{{200505,"2.5"},
                                                                  {200805,"3.0"},
                                                                  {201107,"3.1"},
                                                                  {201307,"4.0"},
                                                                  {201511,"4.5"}};

        std::string versionOMP;
        if (mapOMP.find(_OPENMP) == mapOMP.end())
        {
            versionOMP = "unknown";
        }
        else
        {
            versionOMP = mapOMP.at(_OPENMP);
        }
        std::cout << "version " << versionOMP << " ";
        size_t       omp_curr_threads = ::omp_get_max_threads();
        size_t const omp_max_threads  = ::omp_get_num_procs();
        std::cout << "(currently running on " << omp_curr_threads
                  << "/" << omp_max_threads << " threads)" << std::endl;
    #else
        std::cout << "not available" << std::endl;
    #endif

    std::cout << " Vector intrinsics ";
    #ifdef __AVX512CD__
        std::cout << "AVX512 (512bit, 8 doubles, 16 floats)" << std::endl;
    #elif __AVX2__
        std::cout << "AVX2 (256bit, 4 doubles, 8 floats)"    << std::endl;
    #else
        std::cout << "not supported" << std::endl;
    #endif

    return;
}

#endif // LBT_DISCLAIMER
