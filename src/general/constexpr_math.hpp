#ifndef LBT_CONSTEXPR_MATH
#define LBT_CONSTEXPR_MATH

/**
 * \file     constexpr_math.hpp
 * \mainpage A series of functions that can evaluate to a constant expression and thus
 *           can make use of evaluation and optimisation at compile time.
 * \author   Tobit Flatscher (github.com/2b-t)
 * 
 * \warning  Requires C++17 and makes optional use of C++20 consteval
*/

#include <array>
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

    /**\fn        cem::sum
     * \brief     Constexpr sum with variadic templates and fold expressions
     *
     * \tparam    Args   Data type of the corresponding arguments to be summed
     * \param[in] args   Arguments to be summed up
     * \return    The summed up arguments
    */
    template <typename... Args>
    constexpr auto sum(Args const&... args)
    {
        return (args + ...);
    }

    /**\fn        cem::mergeArrays
     * \brief     Merge arrays constexpr with variadic templates
     *
     * \tparam    T     Data-type of the arrays to be merged
     * \tparam    SZ    Size of the arrays to be merged
     * \param[in] arr   The arrays to be merged
     * \return    The arrays merged into a single array
    */
    template <typename T, size_t... SZ>
    constexpr auto mergeArrays(std::array<T, SZ> const&... arr)
    {
        constexpr size_t numberOfArrays = sizeof...(SZ);
        std::array<T const*,numberOfArrays> const arrayDatas = {arr.data()...};
        constexpr std::array<size_t,numberOfArrays> arraySizes = {SZ...};
        constexpr size_t cumulativeLength = sum(SZ...);
        std::array<T,cumulativeLength> mergedArray = {};

        size_t index = 0;
        for(size_t i = 0; i < numberOfArrays; ++i)
        {
            for(size_t j = 0; j < arraySizes[i]; ++j)
            {
                mergedArray[index] = arrayDatas[i][j];
                ++index;
            }
        }

        return mergedArray;
    }
}

#endif // LBT_CONSTEXPR_MATH
