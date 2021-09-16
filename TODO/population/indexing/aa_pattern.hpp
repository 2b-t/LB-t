#ifndef LBT_AA_PATTERN
#define LBT_AA_PATTERN

/**
 * \file     aa_pattern.hpp
 * \brief    Class members for indexing of populations with A-A access pattern
 * \author   Tobit Flatscher (github.com/2b-t)
 *
 * \mainpage The A-A access pattern avoids the usage of two distinct populations before and
 *           after streaming by treating even and odd time steps differently: Even time steps
 *           perform only a local collision step with a reverse read of the populations and a
 *           regular write while odd steps perform a combined streaming-collision-streaming
 *           step with a regular read and a reverse write.
 *           This is implemented by different macros that determine the population indices for
 *           even and odd time steps.
*/

#include <array>

#include "indexing.hpp"


/**\class  AaPattern
 * \brief  Class that is responsible for indexing according to Bailey's A-A pattern
 *
 * \note   "Accelerating Lattice Boltzmann Fluid Flow Simulations Using Graphics Processors"
 *         P. Bailey, J. Myre, S.D.C. Walsh, D.J. Lilja, M.O. Saar
 *         38th International Conference on Parallel Processing (ICPP), Vienna, Austria (2009)
 *         DOI: 10.1109/ICPP.2009.38
 *
 * \tparam NX     Simulation domain resolution in x-direction
 * \tparam NY     Simulation domain resolution in y-direction
 * \tparam NZ     Simulation domain resolution in z-direction
 * \tparam LT     Static lattice::DdQq class containing discretisation parameters
 * \tparam T      Floating data type used for simulation
 * \tparam NPOP   Number of populations stored side by side in a single merged grid (default = 1)
*/
template <template <typename T> class LT, typename T, std::int32_t NPOP>
class AaPattern
{
    protected:
        Indexing<LT,T,NPOP> indexing;
        std::int32_t NX;
        std::int32_t NY;
        std::int32_t NZ;

    public:
        constexpr AaPattern(std::int32_t const NX, std::int32_t const NY, std::int32_t const NZ) noexcept
            : indexing{NX,NY,NZ}, NX{NX}, NY{NY}, NZ{NZ} {
            return;
        }

        /**\fn        indexRead
         * \brief     Function for determining linear index when reading/writing values before collision depending on even and odd time step.
         * \warning   Inline function! Has to be declared in header!
         *
         * \tparam    TS    Even (0, false) or odd (1, true) time step
         * \param[in] x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in] y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in] z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in] n     Positive (0) or negative (1) index/lattice velocity
         * \param[in] d     Relevant population index
         * \param[in] p     Relevant population (default = 0)
         * \return    Requested linear population index before collision
        */
        template <timestep TS>
        static inline std::int64_t indexRead(std::array<std::int32_t,3> const &x,
                                             std::array<std::int32_t,3> const &y,
                                             std::array<std::int32_t,3> const &z,
                                             std::int32_t               const n,
                                             std::int32_t               const d,
                                             std::int32_t               const p) noexcept
        {
            return indexing.spatialToLinear(x[1 + IDX::template oddEven<TS>(static_cast<std::int32_t>(LT<T>::DX[(!n)*LT<T>::OFF+d]), 0)],
                                            y[1 + IDX::template oddEven<TS>(static_cast<std::int32_t>(LT<T>::DY[(!n)*LT<T>::OFF+d]), 0)],
                                            z[1 + IDX::template oddEven<TS>(static_cast<std::int32_t>(LT<T>::DZ[(!n)*LT<T>::OFF+d]), 0)],
                                            IDX::template oddEven<TS>(n, !n),
                                            d, p);
        }

        /**\fn        indexWrite
         * \brief     Function for determining linear index when reading/writing values after collision depending on even and odd time step.
         * \warning   Inline function! Has to be declared in header!
         *
         * \tparam    TS    Even (0, false) or odd (1, true) time step
         * \param[in] x     x coordinates of current cell and its neighbours [x-1,x,x+1]
         * \param[in] y     y coordinates of current cell and its neighbours [y-1,y,y+1]
         * \param[in] z     z coordinates of current cell and its neighbours [z-1,z,z+1]
         * \param[in] n     Positive (0) or negative (1) index/lattice velocity
         * \param[in] d     Relevant population index
         * \param[in] p     Relevant population (default = 0)
         * \return    Requested linear population index after collision
        */
        template <timestep TS>
        static inline std::int64_t indexWrite(std::array<std::int32_t,3> const &x,
                                              std::array<std::int32_t,3> const &y,
                                              std::array<std::int32_t,3> const &z,
                                              std::int32_t               const n,
                                              std::int32_t               const d,
                                              std::int32_t               const p) noexcept
        {
            return indexing.spatialToLinear(x[1 + IDX::template oddEven<TS>(static_cast<std::int32_t>(LT<T>::DX[n*LT<T>::OFF+d]), 0)],
                                        y[1 + IDX::template oddEven<TS>(static_cast<std::int32_t>(LT<T>::DY[n*LT<T>::OFF+d]), 0)],
                                        z[1 + IDX::template oddEven<TS>(static_cast<std::int32_t>(LT<T>::DZ[n*LT<T>::OFF+d]), 0)],
                                        IDX::template oddEven<TS>(!n, n),
                                        d, p);
        }
};

#endif // LBT_AA_PATTERN
