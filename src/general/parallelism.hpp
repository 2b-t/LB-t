#ifndef LBT_PARALLELISM
#define LBT_PARALLELISM

/**
 * \file     parallelism.hpp
 * \brief    Class for parallel computing settings
 *
 * \mainpage An artificial class that allows for convenient changes of parameters that control
 *           the parallel environment OpenMP
*/

#if __has_include (<omp.h>)
    #include <omp.h>
#endif


#ifdef _OPENMP
    /**\class    Parallelism
     * \brief    Class for all variables regarding parallelism
     *           Variables as int instead of size_t because that way in OpenMP
    */
    class Parallelism
    {
        private:
            int const threads_max_ = omp_get_num_procs(); ///< variable for maximum number of threads
            int       threads_num_ = omp_get_num_procs(); ///< number of currently used threads (default all)

        public:
            /**\fn        Parallelism
             * \brief     Default constructor of parallelism class
            */
            Parallelism();

            /**\fn        setNestedParallelism
             * \brief     Activate nested parallelism (parallel environment within parallel environment)
             *
             * \param[in] nested   Boolean true/false parameter
            */
            void setNestedParallelism(bool const nested);

            /**\fn        setThreadsNum
             * \brief     Change number of used threads.
             *
             * \param[in] threads_set   Number of wished threads (max is given by threads_max)
             * \return    Return exit success or failure
            */
            int setThreadsNum(int const threads_num);

            /**\fn        getThreadsMax
             * \brief     Get number of maximum number of possible threads.
             *
             * \return    Return maximum number of possible threads
            */
            int getThreadsMax();

            /**\fn        getThreadsMax
             * \brief     Get number of actively set threads.
             *
             * \return    Return number of active threads
            */
            int getThreadsNum();

            /**\fn        getThreadsCurr
             * \brief     Get number of threads currently active in a particular parallel region.
             *
             * \return    Return number of threads currently active in parallel region
            */
            int getThreadsCurr();
    };
#endif

#endif // LBT_PARALLELISM
