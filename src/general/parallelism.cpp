#include "parallelism.hpp"

#include <stdlib.h>
#if __has_include (<omp.h>)
    #include <omp.h>
#endif


#ifdef _OPENMP
    Parallelism::Parallelism()
    {
        omp_set_num_threads(threads_max_);
    }

    void Parallelism::SetNestedParallelism(bool const nested)
    {
        omp_set_nested(nested);
    }

    int Parallelism::SetThreadsNum(int const threads_num)
    {
        if(threads_num <= threads_max_)
        {
            threads_num_ = threads_num;
            omp_set_num_threads(threads_num_);

            return EXIT_SUCCESS;
        }
        else
        {
            return EXIT_FAILURE;
        }

    }

    int Parallelism::GetThreadsMax()
    {
        return threads_max_;
    }

    int Parallelism::GetThreadsNum()
    {
        return omp_get_max_threads();
    }

    int Parallelism::GetThreadsCurr()
    {
        return omp_get_num_threads();
    }
#endif
