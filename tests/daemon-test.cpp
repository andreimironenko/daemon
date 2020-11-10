// C++ STL headers
#include <memory>
// Created by amironenko on 09/11/2020.
#include <gtest/gtest.h>

#include "daemon.h"


class DaemonTest: public ::testing::Test {
  protected:
    std::string _mq_name;
    std::unique_ptr<core> _dm;

  public:


    DaemonTest()
      : _mq_name("deamon-test")
    {
      // initialization;

    }

    void SetUp( ) override {
      // initialization or some code to run before each test
    }

    void TearDown( ) override {
      // code to run after each test;
      // can be used instead of a destructor,
      // but exceptions can be handled in this function only
    }

    ~DaemonTest( )  override {
      //resources cleanup, no exceptions allowed
    }

    // shared user data
};

// Creating exclusive message queue with default flags which O_CREAT|O_EXCL
TEST_F(DaemonTest, CliParsing1 ) {
  EXPECT_TRUE(true == true);
    char* args[] = {
            const_cast<char*>("deamon"),
            const_cast<char*>("--config-file"),
            const_cast<char*>("/home/amironenko/tmp/daemon.cfg"),
    };
    _dm = std::make_unique<core>(args[0], 3, static_cast<char**>(args));
}

TEST_F(DaemonTest, CliParsing2 ) {
    EXPECT_TRUE(true == true);
    char* args[] = {
            const_cast<char*>("deamon"),
            const_cast<char*>("--config-file"),
            const_cast<char*>("/home/amironenko/tmp/daemon2.cfg"),
    };
    _dm = std::make_unique<core>(args[0], 3, static_cast<char**>(args));
}

TEST_F(DaemonTest, CliParsingHelp ) {
    EXPECT_TRUE(true == true);
    char* args[] = {
            const_cast<char*>("deamon"),
            const_cast<char*>("--help"),
    };
    _dm = std::make_unique<core>(args[0], 3, static_cast<char**>(args));
}

