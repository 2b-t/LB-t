#ifndef TIMER_HPP_INCLUDED
#define TIMER_HPP_INCLUDED

/**
 * \file     timer.hpp
 * \mainpage An artificial class that allows for a convenient usage of a stopwatch.
*/


#include <chrono>

/**\class Timer
 * \brief Class for stopwatch
*/
class Timer
{
    private:
        std::chrono::high_resolution_clock::time_point start_      = std::chrono::high_resolution_clock::now(); ///< chrono library objects for start and end
        std::chrono::high_resolution_clock::time_point stop_       = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double>                  start_stop_ = std::chrono::duration<double>::zero();     ///< chrono library objects for duration
        double                                         runtime_    = 0.0;                                       ///< double value of runtime in seconds

    public:

    /**\fn    Start
	 * \brief Start stopwatch
	*/
	void Start()
	{
        start_ = std::chrono::high_resolution_clock::now();
    }

	/**\fn     Stop
	 * \brief  Stop stopwatch
	 *
	 * \return Runtime in seconds
	*/
    double Stop()
    {
        stop_       = std::chrono::high_resolution_clock::now();
        start_stop_ = std::chrono::duration_cast<std::chrono::duration<double>>(stop_ - start_);
        runtime_    = start_stop_.count();
        return runtime_;
    }

	/**\fn     GetRuntime
	 * \brief  Return runtime in seconds
	 *
	 * \return Runtime in seconds
	*/
    double GetRuntime()
    {
        std::chrono::high_resolution_clock::time_point const       current_ = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double>                  const start_current_ = std::chrono::duration_cast<std::chrono::duration<double>>(current_ - start_);
        return start_current_.count();
    }
};

#endif // TIMER_HPP_INCLUDED
