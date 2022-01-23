#ifndef LBT_OPENMP_MANAGER
#define LBT_OPENMP_MANAGER
#pragma once

/**
 * \file     openmp_manager.hpp
 * \brief    Class for parallel computing settings
 *
 * \mainpage An artificial class that allows for convenient changes of parameters that control the parallel environment OpenMP
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#include <ostream>


namespace lbt {

  /**\class   OpenMpManager
   * \brief   Singleton class for all variables regarding parallelism
   * \warning Variables as int instead of std::size_t because this way in OpenMP library
  */
  class OpenMpManager {
    public:
      /**\fn     getInstance
       * \brief  Getter of the instance of the singleton class
       *
       * \return Reference to the singleton OpenMpManager class
      */
      static OpenMpManager& getInstance() noexcept;

      /**\fn        setNestedParallelism
       * \brief     Activate nested parallelism (parallel environment within parallel environment)
       *            Source: https://www.openmp.org/spec-html/5.0/openmpsu119.html
       *
       * \param[in] is_nested   Boolean true/false parameter
       * \return    Boolean argument indicating success or failure
      */
      static bool setNestedParallelism(bool const is_nested) noexcept;

      /**\fn        setThreadsNum
       * \brief     Change number of used threads to input parameter if acceptable.
       *            Source: https://www.openmp.org/spec-html/5.0/openmpsu110.html
       *
       * \param[in] number_of_threads   Number of wished threads (max is given by threads_max)
       * \return    Boolean argument indicating success or failure
      */
      static bool setThreadsNum(int const number_of_threads) noexcept;

      /**\fn     getThreadsMax
       * \brief  Get number of maximum number of possible threads.
       *
       * \return Return maximum number of possible threads
      */
      static int getThreadsMax() noexcept;

      /**\fn     getThreadsNum
       * \brief  Get number of actively set threads.
       *         https://www.openmp.org/spec-html/5.0/openmpsu112.html
       *
       * \return Return number of active threads
      */
      static int getThreadsNum() noexcept;

      /**\fn     getThreadsCurrent
       * \brief  Get number of threads currently active in a particular parallel region.
       *         Source: https://www.openmp.org/spec-html/5.0/openmpsu111.html
       *
       * \return Return number of threads currently active in parallel region
      */
      static int getThreadsCurrent() noexcept;

      /**\fn     operator<<
       * \brief  Output stream operator 
       *
       * \param[in,out] os               The stream where the OpenMP manager should be written to
       * \param[in]     openmp_manager   The OpenMP manager that should be printed to the stream
       * \return        The output stream containing the contents of the OpenMP manager
      */
      friend std::ostream& operator<< (std::ostream& os, OpenMpManager const& openmp_manager) noexcept;

      OpenMpManager& operator=(OpenMpManager const&) = default;

    protected:
      /** \brief Default constructor of parallelism class
      */
      OpenMpManager() noexcept;
      OpenMpManager(OpenMpManager const&) = delete;
      OpenMpManager(OpenMpManager&&) = delete;
      OpenMpManager& operator=(OpenMpManager&&) = delete;

      static int threads_max; ///< variable for maximum number of threads
      static int threads_num; ///< number of currently used threads (default all)
  };

}

#endif // LBT_OPENMP_MANAGER
