#ifndef LATTICE_TEST_HPP_INCLUDED
#define LATTICE_TEST_HPP_INCLUDED


#include <array>
#include <iostream>
#include <numeric>

#include "memory_alignment.hpp"


template<class C>
void printStdContainer(C const& stdContainer)
{
    for (auto it = stdContainer.begin(); it != stdContainer.end(); ++it)
    {
        std::cout << "\t" << *it;
    }
    std::cout << std::endl;
    return;
}

template<class C>
void testStdContainerAlignment(C const& stdContainer)
{
    std::cout.setf(std::ios::boolalpha);
    std::cout << " first_element%cache_line: " << (size_t)(stdContainer.data()) % CACHE_LINE  << std::endl;
    std::cout << " length%cache_line:        " << sizeof(stdContainer) % CACHE_LINE           << std::endl;
    return;
}

template <class latticeClass>
class latticeUnitTest
{
    public:
        void printInfo()
        {
            std::cout << "Number of spatial dimensions (DIM):    " << latticeClass::DIM    << std::endl;
            std::cout << "Number of lattice speeds     (SPEEDS): " << latticeClass::SPEEDS << std::endl;
            std::cout << "Amount of padding            (PAD):    " << latticeClass::PAD    << std::endl;
            std::cout << "Number of bytes per node     (ND):     " << latticeClass::ND     << std::endl;
            std::cout << std::endl;
            std::cout << "Weights (W): " << std::endl;
            testStdContainerAlignment(latticeClass::W);
            printStdContainer(latticeClass::W);
            std::cout << "Lattice velocity in x-direcion (DX): " << std::endl;
            testStdContainerAlignment(latticeClass::DX);
            printStdContainer(latticeClass::DX);
            std::cout << "Lattice velocity in y-direcion (DY): " << std::endl;
            testStdContainerAlignment(latticeClass::DY);
            printStdContainer(latticeClass::DY);
            std::cout << "Lattice velocity in z-direcion (DZ): " << std::endl;
            testStdContainerAlignment(latticeClass::DZ);
            printStdContainer(latticeClass::DZ);
        }

        int testClass()
        {
            assert(latticeClass::DIM > 0);
            assert(latticeClass::SPEEDS > 0);
            assert(latticeClass::ND == latticeClass::SPEEDS + latticeClass::PAD);
            assert(std::accumulate(latticeClass::W.begin(),  latticeClass::W.end(),  0.0) - latticeClass::W.at(0) == 1.0);
            assert(std::accumulate(latticeClass::DX.begin(), latticeClass::DX.end(), 0.0) == 0.0);
            assert(std::accumulate(latticeClass::DY.begin(), latticeClass::DY.end(), 0.0) == 0.0);
            assert(std::accumulate(latticeClass::DZ.begin(), latticeClass::DZ.end(), 0.0) == 0.0);
            assert(alignof(latticeClass::W)  == CACHE_LINE);
            assert(alignof(latticeClass::DX) == CACHE_LINE);
            assert(alignof(latticeClass::DY) == CACHE_LINE);
            assert(alignof(latticeClass::DZ) == CACHE_LINE);
            std::cout << "Test passed" << std::endl;
            return EXIT_SUCCESS;
        }

        latticeUnitTest()
        {
            printInfo();
            testClass();
        }
};


#endif //LATTICE_TEST_HPP_INCLUDED
