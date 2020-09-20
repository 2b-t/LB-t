#ifndef LBT_CONSTEXPR_MATH
#define LBT_CONSTEXPR_MATH

/**
 * \file     constexpr_math.hpp
 * \mainpage A series of functions that can evaluate to a constant expression and thus
 *           can make use of evaluation and optimisation at compile time.
 * \author   Tobit Flatscher (github.com/2b-t)
 * 
 * \warning  Makes optional use of C++20 consteval
*/

#include <cassert>
#include <limits>

namespace cem
{
    /**\fn        cem::abs
     * \brief     Constexpr function for absolute value
     *
     * \tparam    T      Data type of the corresponding number
     * \param[in] x      The number of interest
     * \return    The absolute value of \param x
    */
    template <typename T = double>
    constexpr T abs(T const x)
    {
        return (x > 0.0) ? x : -x;
    }

    /**\fn        cem::sqrt
     * \brief     Square root implementation with recursive Newton-Raphson method that can
     *            be evaluated as a constant expression at compile time
     *
     * \param[in] x      The number of interest
     * \param[in] curr   The result from the current iteration
     * \param[in] prev   The result from the previous iteration
     * \return    The square root of \param x
    */
    #if __cplusplus >= 201709L
    consteval
    #else
    constexpr
    #endif
    double sqrtNewton(double const x, double const curr, double const prev)
    {
        return abs<double>(curr - prev) < std::numeric_limits<double>::epsilon()
               ? curr
               : sqrtNewton(x, 0.5 * (curr + x / curr), curr);
    }

    #if __cplusplus >= 201709L
    consteval
    #else
    constexpr
    #endif
    double sqrt(double const x)
    {
        assert(x >= 0.0);

        return ( (x >= 0.0) && (x < std::numeric_limits<double>::infinity()) )
               ? sqrtNewton(x, x, 0.0)
               : std::numeric_limits<double>::quiet_NaN();
    }

    /**\fn        cem::ceil
     * \brief     Ceiling function that can be evaluated as a constant expression at compile time
     *
     * \tparam    T     Data type of the corresponding number
     * \param[in] num   The number that should be ceiled
     * \return    The ceiled number \param num
    */
    template <typename T = double>
    constexpr size_t ceil(T const num)
    {
        return (static_cast<T>(static_cast<size_t>(num)) == num)
               ? static_cast<size_t>(num)
               : static_cast<size_t>(num) + ((num > 0) ? 1 : 0);
    }
}

#endif // LBT_CONSTEXPR_MATH
