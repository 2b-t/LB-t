/**
 * \file     simulation_test.hpp
 * \mainpage Tests for the main simulation class
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#ifndef LBT_SIMULATION_UNITTEST
#define LBT_SIMULATION_UNITTEST
#pragma once

#include <cstdint>
#include <string>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "../src/simulation.hpp"


namespace lbt {
  namespace test {

    /// Test parse-array function with json string
    TEST(ParseArrayFromJsonTest, parseIntArray) {
      using json = nlohmann::json;

      std::string const str {"{\"x\":1,\"y\":2,\"z\":3}"};
      constexpr std::array<int, 3> expected_result = {1, 2, 3};

      json const j = json::parse(str);
      auto const arr = lbt::parseArray<int>(j);

      EXPECT_EQ(arr.at(0), expected_result.at(0));
      EXPECT_EQ(arr.at(1), expected_result.at(1));
      EXPECT_EQ(arr.at(2), expected_result.at(2));
    }

    /// Test writing array to json
    TEST(WriteArrayToJson, writeIntArray) {
      using json = nlohmann::json;

      std::array<int, 3> const arr = {1, 2, 3};
      std::string const expected_result {"{\"x\":1,\"y\":2,\"z\":3}"};

      json const j = lbt::toJson(arr);

      EXPECT_EQ(j.dump(), expected_result);
    }

    TEST(DiscretisationTest, constructAndGetResult) {
      constexpr std::int32_t NX {100};
      constexpr std::int32_t NY {200};
      constexpr std::int32_t NZ {300};

      lbt::settings::Discretisation const discretisation {NX, NY, NZ};

      EXPECT_EQ(discretisation.getNx(), NX);
      EXPECT_EQ(discretisation.getNy(), NY);
      EXPECT_EQ(discretisation.getNz(), NZ);
    }

    TEST(DiscretisationTest, constructFromJsonAndGetResult) {
      using json = nlohmann::json;

      std::string const str {"{\"NX\":100,\"NY\":200,\"NZ\":300}"};
      constexpr std::int32_t NX {100};
      constexpr std::int32_t NY {200};
      constexpr std::int32_t NZ {300};

      json const j = json::parse(str);
      lbt::settings::Discretisation const discretisation {j};

      EXPECT_EQ(discretisation.getNx(), NX);
      EXPECT_EQ(discretisation.getNy(), NY);
      EXPECT_EQ(discretisation.getNz(), NZ);
    }

    TEST(DiscretisationTest, constructAndConvertToJson) {
      using json = nlohmann::json;

      constexpr std::int32_t NX {100};
      constexpr std::int32_t NY {200};
      constexpr std::int32_t NZ {300};
      std::string const expected_result {"{\"NX\":100,\"NY\":200,\"NZ\":300}"};

      lbt::settings::Discretisation const discretisation {NX, NY, NZ};
      json const j = discretisation.toJson();

      EXPECT_EQ(j.dump(), expected_result);
    }

    /// Check data members with given json string: See if imported correctly

    /// Check that after initialisation all pointers initialised

    /// Check that imported and exported settings are equal

    /// Check copy and move operators

  }
}

#endif // LBT_SIMULATION_UNITTEST
