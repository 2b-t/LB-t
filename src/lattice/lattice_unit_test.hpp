#ifndef LATTICE_UNIT_TEST_HPP_INCLUDED
#define LATTICE_UNIT_TEST_HPP_INCLUDED

/**
 * \file     lattice_unit_test.hpp
 * \mainpage Output of static lattice parameters and unit test for lattices
*/

#include <array>
#include <iostream>
#include <numeric>

#include "../general/memory_alignment.hpp"


namespace lattice
{
    /**\fn         Array stream operator
     * \brief      For printing a std::array in an easy way
     *
     * \tparam     T     Data type of array
     * \tparam     N     Number of elements in array
     * \param[in]  os    Input stream
     * \param[in]  arr   Array to print
     * \return     Output stream
    */
    template <typename T, size_t N>
    std::ostream& operator<<(std::ostream& os, std::array<T,N> const& arr)
    {
        for (auto it = arr.begin(); it != arr.end(); ++it)
        {
            os << "\t" << *it;
        }
 
        return os;
    }

    /**\class    UnitTest
     * \brief    Unit test for lattice classes
     *
     * \tparam   LatticeClass   The corresponding static lattice object
    */
    template <class latticeClass>
    class UnitTest
    {
        public:
            /**\fn        printStdContainer
             * \brief     Print any standard container
             *
             * \tparam    C               A std:: container class
             * \param[in] stdConatainer   A std container
            */
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

            /**\fn        testStdContainerAlignment
             * \brief     Test alignment in memory of any standard container
             *
             * \tparam    C               A std:: container class
             * \param[in] stdConatainer   A std container
            */
            template<class C>
            void testStdContainerAlignment(C const& stdContainer)
            {
                std::cout.setf(std::ios::boolalpha);
                std::cout << " first_element%cache_line: " << (size_t)(stdContainer.data()) % CACHE_LINE  << std::endl;
                std::cout << " length%cache_line:        " << sizeof(stdContainer) % CACHE_LINE           << std::endl;
 
                return;
            }

            /**\fn    printInfo
             * \brief Print info about lattice class
            */
            void printInfo()
            {
                std::cout << "Number of spatial dimensions (DIM):    " << latticeClass::DIM    << std::endl;
                std::cout << "Number of lattice speeds     (SPEEDS): " << latticeClass::SPEEDS << std::endl;
                std::cout << "Amount of padding            (PAD):    " << latticeClass::PAD    << std::endl;
                std::cout << "Number of bytes per node     (ND):     " << latticeClass::ND     << std::endl;
                std::cout << std::endl;

                std::cout << "Lattice velocity in x-direcion (DX): " << std::endl;
                testStdContainerAlignment(latticeClass::DX);
                std::cout << latticeClass::DX << std::endl;
                std::cout << "Lattice velocity in y-direcion (DY): " << std::endl;
                testStdContainerAlignment(latticeClass::DY);
                std::cout << latticeClass::DY << std::endl;
                std::cout << "Lattice velocity in z-direcion (DZ): " << std::endl;
                testStdContainerAlignment(latticeClass::DZ);
                std::cout << latticeClass::DZ << std::endl;

                std::cout << "Weights (W): " << std::endl;
                testStdContainerAlignment(latticeClass::W);
                std::cout << latticeClass::W << std::endl;
 
                return;
            }

            /**\fn    testClass
             * \brief Test class for different properties
            */
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

            /**\fn    UnitTest
             * \brief Constructor that performs unit test
            */
            UnitTest()
            {
                printInfo();
                testClass();
 
                return;
            }
    };
}

#endif //LATTICE_UNIT_TEST_HPP_INCLUDED
