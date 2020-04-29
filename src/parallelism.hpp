#ifndef PARALLELISM_H_INCLUDED
#define PARALLELISM_H_INCLUDED

/**
 * \file     parallelism.h
 * \brief    Class for parallel computing settings
 *
 * \mainpage An artificial class that allows for convenient changes of parameters that control
 *           the parallel environment OpenMP
*/

#include <stdlib.h>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#ifdef _OPENMP
    /**\class    Parallelism
     * \brief    Class for all variables regarding parallelism
    */
    class Parallelism
    {
        private:
            const size_t _threads_max = omp_get_num_procs(); //variable for maximum number of threads
            size_t       _threads_num = omp_get_num_procs(); //number of currently used threads (default all)

        public:
            /**\fn        Parallelism
             * \brief     Default constructor of parallelism class
            */
            Parallelism();

            /**\fn        SetNestedParallelism
             * \brief     Activate nested parallelism (parallel environment within parallel environment)
             *
             * \param[in] nested: boolean true/false parameter
            */
            void SetNestedParallelism(bool const nested);

            /**\fn        SetThreadsNum
             * \brief     Change number of used threads.
             *
             * \param[in] threads_set: number of wished threads (max is given by threads_max)
             * \return    Return exit success or failure
            */
            int SetThreadsNum(size_t const threads_set);

            /**\fn        GetThreadsMax
             * \brief     Get number of maximum number of possible threads.
             *
             * \return    Return maximum number of possible threads
            */
            size_t GetThreadsMax();

            /**\fn        GetThreadsMax
             * \brief     Get number of actively set threads.
             *
             * \return    Return number of active threads
            */
            size_t GetThreadsNum();

            /**\fn        GetThreadsCurr
             * \brief     Get number of threads currently active in a particular parallel region.
             *
             * \return    Return number of threads currently active in parallel region
            */
            size_t GetThreadsCurr();
    };
#endif

#endif // PARALLELISM_H_INCLUDED
