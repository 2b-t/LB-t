#ifndef LBT_INDEXING
#define LBT_INDEXING

/**
 * \file     indexing.hpp
 * \brief    Base class members for indexing of populations with different access patterns
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <iostream>


/**\enum  timestep
 * \brief Strongly typed for even and odd time steps required for AA access pattern
 */
enum class timestep: bool { even = false, odd = true };

/**\fn        Negation timestep operator
 * \brief     Negation operator for the timestep
 *
 * \param[in] ts   Timestep to be negated
 * \return    Negated timestep
 */
constexpr timestep operator ! (timestep const& ts)
{
    return (ts == timestep::even) ? timestep::odd : timestep::even;
}

/**\fn            Timestep output stream operator
 * \brief         Output stream operator for the timestep
 *
 * \param[in,out] os   Output stream
 * \param[in]     ts   Timestep to be printed to output stream
 * \return        Output stream including the type of timestep
 */
std::ostream& operator << (std::ostream& os, timestep const& ts)
{
    os << ((ts == timestep::even) ? "even time step" : "odd time step");
    return os;
}


/**\class  Indexing
 * \brief  Class that is responsible for indexing
 *
 * \tparam NX     Simulation domain resolution in x-direction
 * \tparam NY     Simulation domain resolution in y-direction
 * \tparam NZ     Simulation domain resolution in z-direction
 * \tparam LT     Static lattice::DdQq class containing discretisation parameters
 * \tparam T      Floating data type used for simulation
 * \tparam NPOP   Number of populations stored side by side in a single merged grid (default = 1)
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP>
class Indexing
{
    public:
        /**\fn        oddEven
         * \brief     Function for access of individual indices in odd and even time steps
         * \warning   Reduces the range of unsigned int -> int and might overflow!
         *
         * \param[in] ts          boolean index for odd (1 = true) and even (0 = false) time steps
         * \param[in] oddIndex    index to be accessed at an odd time step
         * \param[in] evenIndex   index to be accessed at an even time step
        */
        template <timestep TS>
        static inline int __attribute__((always_inline)) oddEven(int const oddIndex, int const evenIndex)
        {
            return (TS == timestep::odd) ? oddIndex : evenIndex;
        }

        /**\fn        spatialToLinear
         * \brief     Inline function for converting 3D population coordinates to scalar index
         * \warning   Inline function! Has to be declared in header!
         *
         * \param[in] x   x coordinate of cell
         * \param[in] y   y coordinate of cell
         * \param[in] z   z coordinate of cell
         * \param[in] n   Positive (0) or negative (1) index/lattice velocity
         * \param[in] d   Relevant population index
         * \param[in] p   Relevant population (default = 0)
         * \return    Requested linear population index
        */
        static inline size_t __attribute__((always_inline)) spatialToLinear(unsigned int const x, unsigned int const y, unsigned int const z,
                                                                            unsigned int const n, unsigned int const d, unsigned int const p)
        {
            return (((z*NY + y)*NX + x)*NPOP + p)*LT<T>::ND + n*LT<T>::OFF + d;
        }

        /**\fn        linearToSpatial
        * \brief      Generate 3D population coordinates from scalar index
        *
        * \param[out] x       Return value x coordinate of cell
        * \param[out] y       Return value y coordinate of cell
        * \param[out] z       Return value z coordinate of cell
        * \param[out] p       Return value number of population
        * \param[out] n       Return positive (0) or negative (1) index
        * \param[out] d       Return value number of relevant population index
        * \param[in]  index   Current linear population index
        */
        void linearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z, unsigned int& p, unsigned int& n, unsigned int& d, size_t const index) const;
};


template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP>
void Indexing<NX,NY,NZ,LT,T,NPOP>::linearToSpatial(unsigned int& x, unsigned int& y, unsigned int& z, unsigned int& p, unsigned int& n, unsigned int& d,
                                                   size_t const index) const
{
    size_t factor = LT<T>::ND*NPOP*NX*NY;
    size_t rest   = index%factor;

    z      = index/factor;

    factor = LT<T>::ND*NPOP*NX;
    y      = rest/factor;
    rest   = rest%factor;

    factor = LT<T>::ND*NPOP;
    x      = rest/factor;
    rest   = rest%factor;

    factor = LT<T>::ND;
    p      = rest/factor;
    rest   = rest%factor;

    factor = LT<T>::OFF;
    n      = rest/factor;
    rest   = rest%factor;

    factor = LT<T>::SPEEDS;
    d      = rest%factor;

    return;
}

#endif // LBT_INDEXING
