#ifndef LBT_TIMER_UNITTEST
#define LBT_TIMER_UNITTEST
#pragma once

/**
 * \file     timer_unittest.hpp
 * \mainpage Tests for output stream manager
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include "../../src/general/timer.hpp"
 

/// Test if start stop is working
TEST(TimerTest, startStop) {
  using namespace std::chrono_literals;
  auto const runtime = 1s;
  double const abs_error {0.1};

  lbt::Timer timer {};
  timer.start();
  std::this_thread::sleep_for (runtime);
  double const duration = timer.stop();
  EXPECT_NEAR(duration, static_cast<double>(runtime.count()), abs_error);
}

TEST(TimerTest, startStopStartStop) {
  using namespace std::chrono_literals;
  auto const runtime_1 = 1s;
  auto const runtime_2 = 2s;
  double const abs_error {0.1};

  lbt::Timer timer {};
  timer.start();
  std::this_thread::sleep_for (runtime_1);
  double const duration_1 = timer.stop();
  EXPECT_NEAR(duration_1, static_cast<double>(runtime_1.count()), abs_error);

  timer.start();
  std::this_thread::sleep_for (runtime_2);
  double const duration_2 = timer.stop();
  EXPECT_NEAR(duration_2, static_cast<double>(runtime_2.count()), abs_error);
}

#endif // LBT_TIMER_UNITTEST
