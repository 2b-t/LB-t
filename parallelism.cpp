#include "parallelism.hpp"

#include <stdlib.h>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif


#ifdef _OPENMP
    Parallelism::Parallelism()
    {
        omp_set_num_threads(threads_max);
    }

    void Parallelism::SetNestedParallelism(bool const nested)
    {
        omp_set_nested(nested);
    }

    int Parallelism::SetThreadsNum(size_t const threads_set)
    {
        if(threads_set <= threads_max)
        {
            threads_num = threads_set;
            omp_set_num_threads(threads_num);

            return EXIT_SUCCESS;
        }
        else
        {
            return EXIT_FAILURE;
        }

    }

    size_t Parallelism::GetThreadsMax()
    {
        return threads_max;
    }

    size_t Parallelism::GetThreadsNum()
    {
        return omp_get_max_threads();
    }

    size_t Parallelism::GetThreadsCurr()
    {
        return omp_get_num_threads();
    }
#endif
