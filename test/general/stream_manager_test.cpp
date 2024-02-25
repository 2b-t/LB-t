/**
 * \file     stream_manager_test.cpp
 * \mainpage Tests for output stream manager
 * \author   Tobit Flatscher (github.com/2b-t)
*/

#include <ostream>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "lbt/general/stream_manager.hpp"


namespace lbt {
  namespace test {

    /// Test that redirect is working
    TEST(StreamManagerTest, redirect) {
      std::string const test_str {"Redirect message"};

      std::stringstream ss {};

      lbt::StreamManager stream_manager {std::cout};
      stream_manager.redirect(ss);

      std::cout << test_str;
      EXPECT_TRUE(ss.str() == test_str);
    }

    /// Test that restore is working
    TEST(StreamManagerTest, restore) {
      std::string const test_str_src {"Message for source"};
      std::string const test_str_dst {"Message for destination"};

      std::stringstream ss_src {};
      std::stringstream ss_dst {};
      
      lbt::StreamManager stream_manager {ss_src};
      stream_manager.redirect(ss_dst);

      ss_src << test_str_dst;
      EXPECT_TRUE(ss_dst.str() == test_str_dst);

      stream_manager.restore();
      ss_src << test_str_src;
      EXPECT_TRUE(ss_dst.str() == test_str_dst);
      EXPECT_TRUE(ss_src.str() == test_str_src);
    }

    /// Test that turn off is working
    TEST(StreamManagerTest, turnOff) {
      std::string const test_str_out {"Output this message"};
      std::string const test_str_no_out {"Do not output this message"};
      
      std::stringstream ss {};

      lbt::StreamManager stream_manager {ss};

      ss << test_str_out;
      EXPECT_TRUE(ss.str() == test_str_out);

      ss.str("");
      stream_manager.turnOff();
      ss << test_str_no_out;
      EXPECT_TRUE(ss.str().empty() == true);
    }

    /// Test that turn_on is working (depends on turn_off!)
    TEST(StreamManagerTest, turnOn) {
      std::string const test_str_no_out {"Do not output this message"};
      std::string const test_str_out {"Output this message"};
      
      std::stringstream ss {};

      lbt::StreamManager stream_manager {ss};

      stream_manager.turnOff();
      ss << test_str_no_out;
      EXPECT_TRUE(ss.str().empty() == true);

      stream_manager.turnOn();
      ss << test_str_out;
      EXPECT_TRUE(ss.str() == test_str_out);
    }

    /// Test that destructor is resetting the stream
    TEST(StreamManagerTest, destructorRestore) {
      std::string const test_str_no_out {"Do not output this message"};
      std::string const test_str_out {"Output this message"};

      std::stringstream ss_src {};
      std::stringstream ss_dst {};

      if (true) {
        lbt::StreamManager stream_manager {ss_src};
        stream_manager.redirect(ss_dst);

        ss_src << test_str_no_out;
        EXPECT_TRUE(ss_src.str().empty() == true);
      }

      ss_src << test_str_out;
      EXPECT_TRUE(ss_src.str() == test_str_out);
    }

    TEST(StreamManagerTest, destructorTurnOn) {
      std::string const test_str_no_out {"Do not output this message"};
      std::string const test_str_out {"Output this message"};

      std::stringstream ss {};

      if (true) {
        lbt::StreamManager stream_manager {ss};
        stream_manager.turnOff();

        ss << test_str_no_out;
        EXPECT_TRUE(ss.str().empty() == true);
      }

      ss << test_str_out;
      EXPECT_TRUE(ss.str() == test_str_out);
    }

  }
}
