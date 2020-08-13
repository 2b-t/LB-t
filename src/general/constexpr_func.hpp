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
    /**\fn        cef::abs
     * \brief     Constexpr function for absolute value
     *
     * \tparam    T      data type of the corresponding number
     * \param[in] x      the number of interest
     * \return    The absolute value of \param x
    */
    template <typename T = double>
    constexpr T abs(T const x)
    {
        return (x > 0.0) ? x : -x;
    }

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
        return abs<double>(curr - prev) < std::numeric_limits<double>::epsilon()
               ? curr
               : sqrtNewton(x, 0.5 * (curr + x / curr), curr);
    }

    constexpr double sqrt(double const x)
    {
        assert(x >= 0.0);

        return ( (x >= 0.0) && (x < std::numeric_limits<double>::infinity()) )
               ? sqrtNewton(x, x, 0.0)
               : std::numeric_limits<double>::quiet_NaN();
    }

    /**\fn        cef::ceil
     * \brief     Ceiling function that can be evaluated as a constant expression at compile time
     *
     * \tparam    T     data type of the corresponding number
     * \param[in] num   the number that should be ceiled
     * \return    The ceiled number \param num
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
