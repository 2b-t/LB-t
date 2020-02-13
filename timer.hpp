#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

/**
 * \file     Timer.h
 * \mainpage An artificial class that allows for a convenient usage of a stopwatch.
*/


#include <chrono>


/**\class Timer
 * \brief Class for stopwatch
*/
class Timer
{
    private:
        std::chrono::high_resolution_clock::time_point _start      = std::chrono::high_resolution_clock::now(); ///< chrono library objects for start and end
        std::chrono::high_resolution_clock::time_point _stop       = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double>                  _start_stop = std::chrono::duration<double>::zero();     ///< chrono library objects for duration
        double                                         _runtime    = 0;                                         ///< double value of runtime in seconds

    public:

    /**\fn    Start
	 * \brief Start stopwatch
	*/
	void Start()
	{
        _start = std::chrono::high_resolution_clock::now();
    }

	/**\fn     Stop
	 * \brief  Stop stopwatch
	 *
	 * \return Runtime in seconds
	*/
    double Stop()
    {
        _stop = std::chrono::high_resolution_clock::now();
        _start_stop = std::chrono::duration_cast<std::chrono::duration<double>>(_stop-_start);
        _runtime = _start_stop.count();
        return _runtime;
    }

	/**\fn     GetRuntime
	 * \brief  Return runtime in seconds
	 *
	 * \return Runtime in seconds
	*/
    double GetRuntime()
    {
        Stop();
        return _runtime;
    }
};

#endif // TIMER_H_INCLUDED
