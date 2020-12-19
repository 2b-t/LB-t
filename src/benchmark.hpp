#ifndef LBT_BENCHMARK
#define LBT_BENCHMARK

/**
 * \file     benchmark.hpp
 * \mainpage Functions for benchmarking the collision operators
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <iostream>
#include <memory>

#include "continuum/continuum.hpp"
#include "general/openmp_manager.hpp"
#include "general/timer.hpp"
#include "lattice/D2Q9.hpp"
#include "lattice/D3Q15.hpp"
#include "lattice/D3Q19.hpp"
#include "lattice/D3Q27.hpp"
#include "population/collision/collision_bgk.hpp"
#include "population/collision/collision_bgk_avx2.hpp"
#include "population/collision/collision_bgk_avx512.hpp"
#include "population/collision/collision_bgk_smagorinsky.hpp"
#include "population/collision/collision_trt.hpp"
#include "population/population.hpp"


/**\fn            benchmarkSingleLatticeSingleCollisionOperator
 * \brief         Benchmark a single lattice with a single collision operator
 * \warning       New collision operators have to be added to this function manually!
 * 
 * \tparam        NX                  Simulation domain resolution in x-direction
 * \tparam        NY                  Simulation domain resolution in y-direction
 * \tparam        NZ                  Simulation domain resolution in z-direction
 * \tparam        LT                  Static lattice::DdQq class containing discretisation parameters
 * \tparam        T                   Floating data type used for simulation
 * \param[in,out] collisionOperator   Collision operator to be used
 * \param[in]     NT                  Number of timesteps that should be executed
 */
template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T, unsigned int NPOP, typename DerivedClass>
void benchmarkSingleLatticeSingleCollisionOperator(CollisionOperator<NX,NY,NZ,LT,T,NPOP,DerivedClass>& collisionOperator, unsigned int const NT)
{
    collisionOperator.template initialise<timestep::even>(1.0, 0.05, 0.05, 0.05);

    Timer timer;
    timer.start();

    for (unsigned int i = 0; i < NT; i+=2)
    {
        collisionOperator.template collideStream<timestep::even>();
        collisionOperator.template collideStream<timestep::odd>();
    }

    double const      runtime = timer.stop();
    size_t const nodesUpdated = static_cast<size_t>(NT)*NX*NY*static_cast<size_t>(NZ);
    double const        speed = 1e-6*nodesUpdated/runtime;
    std::printf("speed: %.2f [Mlups] (runtime: %.2f [s])\n", speed, runtime);

    return;
}

/**\fn        benchmarkSingleLatticeAllCollisionOperators
 * \brief     Benchmark a single lattice with all possible collision operators
 * \warning   New collision operators have to be added to this function manually!
 * 
 * \tparam    NX   Simulation domain resolution in x-direction
 * \tparam    NY   Simulation domain resolution in y-direction
 * \tparam    NZ   Simulation domain resolution in z-direction
 * \tparam    LT   Static lattice::DdQq class containing discretisation parameters
 * \tparam    T    Floating data type used for simulation
 * \param[in] NT   Number of timesteps that should be executed
 */
template <unsigned int NX, unsigned int NY, unsigned int NZ, template <typename T> class LT, typename T>
void benchmarkSingleLatticeAllCollisionOperators(unsigned int const NT)
{
    std::cout << "- D" << LT<T>::DIM << "Q" << LT<T>::SPEEDS << " lattice with padding to " << LT<T>::ND << std::endl;

    auto continuum  = std::make_shared<Continuum<NX,NY,NZ,T>>();
    auto population = std::make_shared<Population<NX,NY,NZ,LT,T>>();

    std::cout << "  + BGK: ";
    BGK bgk {population, continuum, 10.0, 0.05, NX};
    ::benchmarkSingleLatticeSingleCollisionOperator(bgk, NT);

    #ifdef __AVX2__
        if constexpr (LT<T>::ND % 4 == 0)
        {
            std::cout << "  + BGK with manual AVX2 vector intrinsics: ";
            BGK_AVX2 bgk_avx2 {population, continuum, 10.0, 0.05, NX};
            ::benchmarkSingleLatticeSingleCollisionOperator(bgk_avx2, NT);
        }
    #endif

    #ifdef __AVX512CD__
        if constexpr (LT<T>::ND % 8 == 0)
        {
            std::cout << "  + BGK with manual AVX512 vector intrinsics: ";
            BGK_AVX512 bgk_avx512 {population, continuum, 10.0, 0.05, NX};
            ::benchmarkSingleLatticeSingleCollisionOperator(bgk_avx512, NT);
        }
    #endif

    std::cout << "  + TRT: ";
    TRT trt {population, continuum, 10.0, 0.05, NX};
    ::benchmarkSingleLatticeSingleCollisionOperator(trt, NT);

    std::cout << "  + BGK with Smagorinsky turbulence model: ";
    BGK_Smagorinsky bgk_smagorinsky {population, continuum, 500.0, 0.05, NX};
    ::benchmarkSingleLatticeSingleCollisionOperator(bgk_smagorinsky, NT);

    return;
}


/**\fn      benchmarkAllLatticesAllCollisionOperators
 * \brief   Benchmark all lattices and their corresponding collision operators
 * \warning New lattices have to be added to this function manually!
 */
void benchmarkAllLatticesAllCollisionOperators()
{
    std::cout << "Starting benchmark..." << std::endl;

    std::cout << "D2Q9 lattice" << std::endl;
    ::benchmarkSingleLatticeAllCollisionOperators<512,512,1,lattice::D2Q9P10,double>(1000);
    ::benchmarkSingleLatticeAllCollisionOperators<512,512,1,lattice::D2Q9P12,double>(1000);

    std::cout << "D3Q15 lattice" << std::endl;
    ::benchmarkSingleLatticeAllCollisionOperators<128,128,128,lattice::D3Q15P16,double>(100);

    std::cout << "D3Q19 lattice" << std::endl;
    ::benchmarkSingleLatticeAllCollisionOperators<128,128,128,lattice::D3Q19P20,double>(100);

    std::cout << "D3Q27 lattice" << std::endl;
    ::benchmarkSingleLatticeAllCollisionOperators<128,128,128,lattice::D3Q27P28,double>(100);
    ::benchmarkSingleLatticeAllCollisionOperators<128,128,128,lattice::D3Q27PC,double>(100);

    return;
}

#endif // LBT_BENCHMARK
