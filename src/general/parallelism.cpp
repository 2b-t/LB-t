#include "parallelism.hpp"

#if __has_include (<omp.h>)
    #include <omp.h>
#endif
#include <cstdlib>


#ifdef _OPENMP
    Parallelism::Parallelism()
    {
        omp_set_num_threads(threads_max_);

        return;
    }

    void Parallelism::setNestedParallelism(bool const nested)
    {
        omp_set_nested(nested);

        return;
    }

    int Parallelism::setThreadsNum(int const threads_num)
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

    int Parallelism::getThreadsMax()
    {
        return threads_max_;
    }

    int Parallelism::getThreadsNum()
    {
        return omp_get_max_threads();
    }

    int Parallelism::getThreadsCurr()
    {
        return omp_get_num_threads();
    }
#endif
