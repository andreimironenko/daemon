// C++ STL headers
#include <memory>
// Created by amironenko on 09/11/2020.
#include <gtest/gtest.h>

#include "daemon.h"


class DaemonTest: public ::testing::Test {
  protected:
    std::string _mq_name;

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
TEST_F(DaemonTest, CliParsing ) {
  EXPECT_TRUE(true == true);
  char* args[3];
  args[0] = const_cast<char*>("deamon");
  args[1] = const_cast<char*>("--compression");
  args[2] = const_cast<char*>("10");
  
  auto dm = std::unique_ptr<core>(new core(args[0], 3, args));

}


