#ifndef LBT_CONTINUUM_UNITTEST
#define LBT_CONTINUUM_UNITTEST
#pragma once

/**
 * \file     continuum_unittest.hpp
 * \mainpage Tests for continuum class
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <cstdint>
#include <filesystem>
#include <functional>
#include <ostream>
#include <string>
#include <tuple>
#include <type_traits>

#include "../../src/general/constexpr_math.hpp"
#include "../testing_utilities/testing_utilities.hpp"
#include "continuum_unittest.hpp"


namespace lbt {
  namespace test{
    
    /**\class  ContinuumTest
     * \brief  Helper class for tests of different continua
     *
     * \tparam C   The continuum class
    */
    template <typename C>
    class ContinuumTest {
        using T = TemplateDataType_t<C>;
      public:
        /**\fn        ContinuumTest
         * \brief     (Default) constructor
         *
         * \param[in] NX            Resolution of the continuum in x-direction
         * \param[in] NY            Resolution of the continuum in y-direction
         * \param[in] NZ            Resolution of the continuum in z-direction
         * \param[in] output_path   Path where the output of the class should be generated
         * 
         * \return    The classes inside a ::testing::Types type list
        */
        ContinuumTest(std::int32_t const NX, std::int32_t const NY, std::int32_t const NZ, 
                      std::filesystem::path const& output_path) noexcept
          : NX{NX}, NY{NX}, NZ{NZ}, output_path{output_path}, c{NX, NY, NZ, output_path} {
          return;
        }

        /**\fn     testSetAndGetPressure
         * \brief  Helper method for testing setting and getting values for pressure
         *
         * \return Boolean argument signaling whether the test succeeded (true) or failed (false)
        */
        bool testSetAndGetPressure() noexcept {
          using namespace std::placeholders;
          auto setter {std::bind(&C::setP, std::ref(c), _1, _2, _3, _4)};
          auto getter {std::bind(&C::getP, std::ref(c), _1, _2, _3)};
          return testSetAndGetHelper(setter, getter);
        }
        bool testSetAndGetVelocityX() noexcept {
          using namespace std::placeholders;
          auto setter {std::bind(&C::setU, std::ref(c), _1, _2, _3, _4)};
          auto getter {std::bind(&C::getU, std::ref(c), _1, _2, _3)};
          return testSetAndGetHelper(setter, getter);
        }
        bool testSetAndGetVelocityY() noexcept {
          using namespace std::placeholders;
          auto setter {std::bind(&C::setV, std::ref(c), _1, _2, _3, _4)};
          auto getter {std::bind(&C::getV, std::ref(c), _1, _2, _3)};
          return testSetAndGetHelper(setter, getter);
        }
        bool testSetAndGetVelocityZ() noexcept {
          using namespace std::placeholders;
          auto setter {std::bind(&C::setW, std::ref(c), _1, _2, _3, _4)};
          auto getter {std::bind(&C::getW, std::ref(c), _1, _2, _3)};
          return testSetAndGetHelper(setter, getter);
        }
      protected:
        /**\fn        testSetAndGetHelper
         * \brief     Helper method for testing setting and getting values for the different entities
         *
         * \param[in] setter   The function to be called for setting a value
         * \param[in] getter   The function to be called for getting a value
         * \return    Boolean argument signaling whether the test succeeded (true) or failed (false)
        */
        bool testSetAndGetHelper(std::function<void(std::int32_t const, std::int32_t const, std::int32_t const, T const)> setter, 
                                 std::function<T(std::int32_t const, std::int32_t const, std::int32_t const)> getter) noexcept {
          bool is_success {true};
          std::int64_t i {0};
          for (std::int32_t x = 0; x < NX; ++x) {
            for (std::int32_t y = 0; y < NY; ++y) {
              for (std::int32_t z = 0; z < NZ; ++z) {
                setter(x,y,z,static_cast<T>(i));
                ++i;
              }
            }
          }
          i = 0;
          for (std::int32_t x = 0; x < NX; ++x) {
            for (std::int32_t y = 0; y < NY; ++y) {
              for (std::int32_t z = 0; z < NZ; ++z) {
                is_success &= lbt::cem::nearlyEqual(getter(x,y,z), static_cast<T>(i));
                ++i;
              }
            }
          }
          return is_success;
        }

        std::int32_t NX;
        std::int32_t NY;
        std::int32_t NZ;
        std::filesystem::path output_path;
        C c;
    };
  }
}

#endif // LBT_CONTINUUM_UNITTEST
