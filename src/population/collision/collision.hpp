#ifndef LBT_COLLISION
#define LBT_COLLISION

/**
 * \file     collision.hpp
 * \mainpage Base class for all collision operators
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <memory>

#include "../../general/constexpr_math.hpp"
#include "../population.hpp"


/**\class  CollisionOperator
 * \brief  Base class for collision operator
 *
 * \tparam NX             Simulation domain resolution in x-direction
 * \tparam NY             Simulation domain resolution in y-direction
 * \tparam NZ             Simulation domain resolution in z-direction
 * \tparam LT             Static lattice::DdQq class containing discretisation parameters
 * \tparam T              Floating data type used for simulation
 * \tparam NPOP           Number of populations stored side by side in a single merged grid
 * \tparam DerivedClass   The derived collision operator that inherits from this base class
*/
template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP, typename DerivedClass>
class CollisionOperator
{
    public:
        /**\fn        collideStream
         * \brief     Curiously Recurring Template Pattern (CRTP) for static polymorphism of the collision operator
         *
         * \tparam    AA       The timestep in the AA-pattern
         * \param[in] isSave   Boolean parameter whether the macroscopic values should be saved or not
        */
        template<timestep AA>
        void collideStream(bool const isSave = false)
        {
            static_cast<DerivedClass*>(this)->template implementation<AA>(isSave);

            return;
        }

        /**\fn        initialise
         * \brief     Initialise the macroscopic continuum and the microscopic populations
         *
         * \tparam    AA      The timestep in the AA-pattern
         * \param[in] RHO_0   The uniform initial density across the flow field
         * \param[in] U_0     The uniform initial velocity in x-direction across the flow field
         * \param[in] V_0     The uniform initial velocity in y-direction across the flow field
         * \param[in] V_0     The uniform initial velocity in z-direction across the flow field
        */
        template<timestep AA>
        void initialise(T const RHO_0, T const U_0, T const V_0, T const W_0);

    protected:
        /**\brief     Constructor
         *
         * \param[in] population   Population object holding microscopic distributions
         * \param[in] continuum    Continuum object holding macroscopic variables
         * \param[in] p            Index of relevant population
        */
        CollisionOperator(std::shared_ptr<Population<NX,NY,NZ,LT,T,NPOP>> population, std::shared_ptr<Continuum<NX,NY,NZ,T>> continuum, unsigned int const p = 0):
            population_(population), continuum_(continuum), p_(p)
        {
            return;
        }

        /**\fn        initialiseContinuum_
         * \brief     Initialise the macroscopic continuum
         *
         * \param[in] RHO_0   The uniform initial density across the flow field
         * \param[in] U_0     The uniform initial velocity in x-direction across the flow field
         * \param[in] V_0     The uniform initial velocity in y-direction across the flow field
         * \param[in] V_0     The uniform initial velocity in z-direction across the flow field
        */
        void initialiseContinuum_(T const RHO_0, T const U_0, T const V_0, T const W_0);

        /**\fn        initialisePopulationFromContinuum_
         * \brief     Initialise the microscopic distributions for the macroscopic continuum
         *
         * \tparam    AA   The timestep in the AA-pattern
        */
        template<timestep AA>
        void initialisePopulationFromContinuum_();

        /// macro and microscopic values
        std::shared_ptr<Population<NX,NY,NZ,LT,T,NPOP>> population_;
        std::shared_ptr<Continuum<NX,NY,NZ,T>>          continuum_;
        unsigned int const p_;

        /// parallelism: 3D blocks
        //  each cell gets a block of cells instead of a single cell
        static constexpr unsigned int   BLOCK_SIZE_ = 32;                                               ///< loop block size
        static constexpr unsigned int NUM_BLOCKS_Z_ = cef::ceil(static_cast<double>(NZ) / BLOCK_SIZE_); ///< number of blocks in each dimension
        static constexpr unsigned int NUM_BLOCKS_Y_ = cef::ceil(static_cast<double>(NY) / BLOCK_SIZE_);
        static constexpr unsigned int NUM_BLOCKS_X_ = cef::ceil(static_cast<double>(NX) / BLOCK_SIZE_);
        static constexpr unsigned int   NUM_BLOCKS_ = NUM_BLOCKS_X_*NUM_BLOCKS_Y_*NUM_BLOCKS_Z_;        ///< total number of blocks
};


template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP, typename DerivedClass> template<timestep AA>
void CollisionOperator<NX,NY,NZ,LT,T,NPOP,DerivedClass>::initialise(T const RHO_0, T const U_0, T const V_0, T const W_0)
{
    initialiseContinuum_(RHO_0, U_0, V_0, W_0);
    initialisePopulationFromContinuum_<AA>();

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP, typename DerivedClass>
void CollisionOperator<NX,NY,NZ,LT,T,NPOP,DerivedClass>::initialiseContinuum_(T const RHO_0, T const U_0, T const V_0, T const W_0)
{
    #pragma omp parallel for default(none) shared(continuum_) firstprivate(RHO_0,U_0,V_0,W_0) schedule(static,1)
    for(unsigned int block = 0; block < NUM_BLOCKS_; ++block)
    {
        unsigned int const z_start = BLOCK_SIZE_ * (block / (NUM_BLOCKS_X_*NUM_BLOCKS_Y_));
        unsigned int const   z_end = std::min(z_start + BLOCK_SIZE_, NZ);

        for(unsigned int z = z_start; z < z_end; ++z)
        {
            unsigned int const y_start = BLOCK_SIZE_*((block % (NUM_BLOCKS_X_*NUM_BLOCKS_Y_)) / NUM_BLOCKS_X_);
            unsigned int const   y_end = std::min(y_start + BLOCK_SIZE_, NY);

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                unsigned int const x_start = BLOCK_SIZE_*(block % NUM_BLOCKS_X_);
                unsigned int const   x_end = std::min(x_start + BLOCK_SIZE_, NX);

                for(unsigned int x = x_start; x < x_end; ++x)
                {
                    continuum_->operator()(x, y, z, 0) = RHO_0;
                    continuum_->operator()(x, y, z, 1) = U_0;
                    continuum_->operator()(x, y, z, 2) = V_0;
                    continuum_->operator()(x, y, z, 3) = W_0;
                }
            }
        }
    }

    return;
}

template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP, typename DerivedClass> template<timestep AA>
void CollisionOperator<NX,NY,NZ,LT,T,NPOP,DerivedClass>::initialisePopulationFromContinuum_()
{
    #pragma omp parallel for default(none) shared(continuum_, population_) firstprivate(p_) schedule(static,1)
    for(unsigned int block = 0; block < NUM_BLOCKS_; ++block)
    {
        unsigned int const z_start = BLOCK_SIZE_ * (block / (NUM_BLOCKS_X_*NUM_BLOCKS_Y_));
        unsigned int const   z_end = std::min(z_start + BLOCK_SIZE_, NZ);

        for(unsigned int z = z_start; z < z_end; ++z)
        {
            unsigned int const z_n[3] = { (NZ + z - 1) % NZ, z, (z + 1) % NZ };

            unsigned int const y_start = BLOCK_SIZE_*((block % (NUM_BLOCKS_X_*NUM_BLOCKS_Y_)) / NUM_BLOCKS_X_);
            unsigned int const   y_end = std::min(y_start + BLOCK_SIZE_, NY);

            for(unsigned int y = y_start; y < y_end; ++y)
            {
                unsigned int const y_n[3] = { (NY + y - 1) % NY, y, (y + 1) % NY };

                unsigned int const x_start = BLOCK_SIZE_*(block % NUM_BLOCKS_X_);
                unsigned int const   x_end = std::min(x_start + BLOCK_SIZE_, NX);

                for(unsigned int x = x_start; x < x_end; ++x)
                {
                    unsigned int const x_n[3] = { (NX + x - 1) % NX, x, (x + 1) % NX };

                    T const rho = continuum_->operator()(x, y, z, 0);
                    T const u   = continuum_->operator()(x, y, z, 1);
                    T const v   = continuum_->operator()(x, y, z, 2);
                    T const w   = continuum_->operator()(x, y, z, 3);

                    T const uu = - 1.0/(2.0*LT<T>::CS*LT<T>::CS)*(u*u + v*v + w*w);

                    #pragma GCC unroll (2)
                    for(unsigned int n = 0; n <= 1; ++n)
                    {
                        #pragma GCC unroll (16)
                        for(unsigned int d = n; d < LT<T>::OFF; ++d)
                        {
                            unsigned int const curr = n*LT<T>::OFF + d;
                            T const cu = 1.0/(LT<T>::CS*LT<T>::CS)*(u*LT<T>::DX[curr] + v*LT<T>::DY[curr] + w*LT<T>::DZ[curr]);
                            population_->F_[population_-> template AA_IndexRead<AA>(x_n,y_n,z_n,n,d,p_)] = LT<T>::W[curr]*(rho + rho*(cu*(1.0 + 0.5*cu) + uu));
                        }
                    }
                }
            }
        }
    }

    return;
}

#endif // LBT_COLLISION
