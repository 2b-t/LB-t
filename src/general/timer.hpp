#ifndef LBT_TIMER
#define LBT_TIMER

/**
 * \file     timer.hpp
 * \mainpage An artificial class that allows for a convenient usage of a stopwatch.
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <chrono>


/**\class Timer
 * \brief Class for stopwatch
*/
class Timer
{
    public:
        /**\fn    start
         * \brief Start stopwatch
        */
        void start()
        {
            start_ = std::chrono::high_resolution_clock::now();

            return;
        }

        /**\fn     stop
         * \brief  Stop stopwatch
         *
         * \return Runtime in seconds
        */
        double stop()
        {
            stop_       = std::chrono::high_resolution_clock::now();
            start_stop_ = std::chrono::duration_cast<std::chrono::duration<double>>(stop_ - start_);
            runtime_    = start_stop_.count();

            return runtime_;
        }

        /**\fn     getRuntime
         * \brief  Return runtime in seconds
         *
         * \return Runtime in seconds
        */
        double getRuntime()
        {
            std::chrono::high_resolution_clock::time_point const       current_ = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double>                  const start_current_ = std::chrono::duration_cast<std::chrono::duration<double>>(current_ - start_);

            return start_current_.count();
        }

    private:
        std::chrono::high_resolution_clock::time_point start_      = std::chrono::high_resolution_clock::now(); ///< chrono library objects for start and end
        std::chrono::high_resolution_clock::time_point stop_       = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double>                  start_stop_ = std::chrono::duration<double>::zero();     ///< chrono library objects for duration
        double                                         runtime_    = 0.0;                                       ///< double value of runtime in seconds
};

#endif // LBT_TIMER
