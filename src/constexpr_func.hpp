#ifndef CONSTEXPR_FUNC_H_INCLUDED
#define CONSTEXPR_FUNC_H_INCLUDED

/**
 * \file     constexpr_func.hpp
 * \mainpage A series of functions that can evaluate to a constant expression and thus
 *           can make use of evaluation and optimisation at compile time.
*/


#include <limits>
#include <assert.h>


namespace cef
{
    /**\fn        cef::sqrt
     * \brief     Square root implementation with recursive Newton-Raphson method that can
     *            be evaluated as a constant expression at compile time
     *
     * \param[in] x      the number of interest
     * \param[in] curr   the result from the current iteration
     * \param[in] prev   the result from the previous iteration
     * \return    The square root of \param x
    */
    constexpr double sqrtNewton(double const x, double const curr, double const prev)
    {
        return curr == prev
               ? curr
               : sqrtNewton(x, 0.5 * (curr + x / curr), curr);
    }

    template <typename T = double>
    constexpr T sqrt(T const x)
    {
        assert(std::numeric_limits<T>::is_specialized);
        assert(x >= 0);

        constexpr T ret_NaN = std::numeric_limits<T>::has_quiet_NaN
                              ? std::numeric_limits<T>::quiet_NaN()
                              : std::numeric_limits<T>::min();

        double const x_d = static_cast<double>(x);
        return ((x_d >= 0.0) &&
                (x_d < std::numeric_limits<double>::infinity()))
               ? static_cast<T>(sqrtNewton(x_d, x_d, 0.0))
               : ret_NaN;
    }
    
    /**\fn        cef::ceil
     * \brief     Ceiling function that can be evaluated as a constant expression at compile time
     *
     * \param[in] num   the number that should be ceiled
     * \return    The square root of \param num
    */
    template <typename T = double>
    constexpr size_t ceil(T num)
    {
        return (static_cast<T>(static_cast<size_t>(num)) == num)
               ? static_cast<size_t>(num)
               : static_cast<size_t>(num) + ((num > 0) ? 1 : 0);
    }
}

#endif // CONSTEXPR_FUNC_H_INCLUDED
