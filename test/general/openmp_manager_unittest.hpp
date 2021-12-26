#ifndef LBT_OPENMP_MANAGER_UNITTEST
#define LBT_OPENMP_MANAGER_UNITTEST
#pragma once

/**
 * \file     openmp_manager_unittest.hpp
 * \brief    Unit-test for parallel computing settings
 *
 * \author   Tobit Flatscher (github.com/2b-t)
*/


#if __has_include (<omp.h>)
    #include <omp.h>
#endif

#include <gtest/gtest.h>

#include "../../src/general/openmp_manager.hpp"


namespace lbt {
  namespace test {

    class OpenMpManagerTest : public ::testing::Test {
      protected:
        OpenMpManagerTest() noexcept
          : openmp_manager{lbt::OpenMpManager::getInstance()} {
          #ifdef _OPENMP
          threads_max_expected = ::omp_get_num_procs();
          #else
          threads_max_expected = 1;
          #endif
          return;
        }

        lbt::OpenMpManager& openmp_manager;
        int threads_max_expected;
    };

    TEST_F(OpenMpManagerTest, correctThreadsMax) {
      int const threads_max {openmp_manager.getThreadsMax()};
      EXPECT_EQ(threads_max, threads_max_expected);
    }

    TEST_F(OpenMpManagerTest, startsWithMaximum) {
      int const threads_num = openmp_manager.getThreadsNum();
      EXPECT_EQ(threads_num, threads_max_expected);
    }

    TEST_F(OpenMpManagerTest, setAndGetThreadsNumSmallerThanThreadsMax) {
      int const target_threads_num {threads_max_expected-1};
      if (target_threads_num >= 1) {
        bool const is_success {openmp_manager.setThreadsNum(target_threads_num)};
        EXPECT_TRUE(is_success);
        int const threads_num {openmp_manager.getThreadsNum()};
        EXPECT_EQ(threads_num, target_threads_num);
      } else {
        GTEST_SKIP() << "Only one thread detected!";
      }
    }

    TEST_F(OpenMpManagerTest, cannotSetThreadsNumToNegativeNumber) {
      int const target_threads_num {-1};
      bool const is_success {openmp_manager.setThreadsNum(target_threads_num)};
      EXPECT_FALSE(is_success);
    }

    TEST_F(OpenMpManagerTest, cannotSetThreadsNumToZero) {
      int const target_threads_num {0};
      bool const is_success {openmp_manager.setThreadsNum(target_threads_num)};
      EXPECT_FALSE(is_success);
    }

    TEST_F(OpenMpManagerTest, cannotSetThreadsNumLargerThanThreadsMax) {
      int const target_threads_num {threads_max_expected+1};
      bool const is_success {openmp_manager.setThreadsNum(target_threads_num)};
      EXPECT_FALSE(is_success);
    }

  }
}

#endif // LBT_OPENMP_MANAGER_UNITTEST
