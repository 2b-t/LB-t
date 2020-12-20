#ifndef LBT_OPENMP_MANAGER
#define LBT_OPENMP_MANAGER

/**
 * \file     openmp_manager.hpp
 * \brief    Class for parallel computing settings
 *
 * \mainpage An artificial class that allows for convenient changes of parameters that control
 *           the parallel environment OpenMP
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#if __has_include (<omp.h>)
    #include <omp.h>
#endif


#ifdef _OPENMP
    /**\class OpenMpManager
     * \brief Singleton class for all variables regarding parallelism
     *        Variables as int instead of size_t because that way in OpenMP
    */
    class OpenMpManager
    {
        public:
            /**\fn     getInstance
             * \brief  Getter of the instance of the singleton class
             *
             * \return Reference to the singleton class
            */
            static OpenMpManager& getInstance() noexcept
            {
                static OpenMpManager singletonOpenMpManager {};

                return singletonOpenMpManager;
            }

            /**\fn        setNestedParallelism
             * \brief     Activate nested parallelism (parallel environment within parallel environment)
             *
             * \param[in] isNested   Boolean true/false parameter
            */
            void setNestedParallelism(bool const isNested) const noexcept
            {
                ::omp_set_nested(isNested);

                return;
            }

            /**\fn        setThreadsNum
             * \brief     Change number of used threads.
             *
             * \param[in] threadsNum   Number of wished threads (max is given by threads_max)
             * \return    Return exit success or failure
            */
            int setThreadsNum(int const threadsNum) noexcept
            {
                int isSuccess = EXIT_FAILURE;

                if(threadsNum <= threadsMax_)
                {
                    threadsNum_ = threadsNum;
                    ::omp_set_num_threads(threadsNum_);

                    isSuccess = EXIT_SUCCESS;
                }

                return isSuccess;
            }

            /**\fn     getThreadsMax
             * \brief  Get number of maximum number of possible threads.
             *
             * \return Return maximum number of possible threads
            */
            int getThreadsMax() const noexcept
            {
                return threadsMax_;
            }

            /**\fn     getThreadsMax
             * \brief  Get number of actively set threads.
             *
             * \return Return number of active threads
            */
            int getThreadsNum() const noexcept
            {
                return ::omp_get_max_threads();
            }

            /**\fn     getThreadsCurrent
             * \brief  Get number of threads currently active in a particular parallel region.
             *
             * \return Return number of threads currently active in parallel region
            */
            int getThreadsCurrent() const noexcept
            {
                return ::omp_get_num_threads();
            }


        private:
            /** \brief Default constructor of parallelism class
            */
            OpenMpManager() noexcept
            {
                ::omp_set_num_threads(::omp_get_num_procs());

                return;
            }

            int threadsMax_ = ::omp_get_num_procs(); ///< variable for maximum number of threads
            int threadsNum_ = ::omp_get_num_procs(); ///< number of currently used threads (default all)
    };
#endif

#endif // LBT_OPENMP_MANAGER
