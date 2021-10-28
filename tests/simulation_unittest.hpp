#ifndef LBT_SIMULATION_UNITTEST
#define LBT_SIMULATION_UNITTEST
#pragma once

/**
 * \file     simulation_unittest.hpp
 * \mainpage Tests for the main simulation class
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <string>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "../src/simulation.hpp"


/// Test parse-array function with json string
TEST(ParseArrayFromJsonTest, correctParsing) {
  using json = nlohmann::json;

  std::string const str {"{\"x\":1,\"y\":2,\"z\":3}"};
  json const j = json::parse(str);
  auto const arr = lbt::parseArray<int>(j);

  EXPECT_EQ(arr.at(0), 1);
  EXPECT_EQ(arr.at(1), 2);
  EXPECT_EQ(arr.at(2), 3);
}

/// Check data members with given json string: See if imported correctly

/// Check that after initialisation all pointers initialised

/// Check that imported and exported settings are equal

/// Check copy and move operators

#endif // LBT_SIMULATION_UNITTEST
