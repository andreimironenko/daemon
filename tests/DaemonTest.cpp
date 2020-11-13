// C++ STL headers
#include <memory>
#include <filesystem>
#include <fstream>
#include <functional>

// Created by amironenko on 09/11/2020.
#include <gtest/gtest.h>

#include "daemon.h"

namespace fs = std::filesystem;
class DaemonTest: public ::testing::Test {
  protected:
    std::string _mq_name;
    std::unique_ptr<daemon_base> _dm;

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

TEST_F(DaemonTest, CliParsing2 ) {
    auto cfg_path = fs::path(fs::temp_directory_path());
    cfg_path /= "daemon2.cfg";
    if(fs::exists(cfg_path))
    {
        fs::remove(cfg_path);
    }
#if 0
    auto ofs = std::shared_ptr<std::ofstream>(
            new std::ofstream(cfg_path.c_str()),
            [](std::ofstream* fs){fs->close();}
            );
#endif

    auto ofs = std::unique_ptr<std::ofstream, void (*)(std::ofstream*)>(
            new std::ofstream(cfg_path.c_str(), std::ios_base::out|std::ios_base::trunc),
            [](std::ofstream* fs) {fs->close();}
            );

    EXPECT_TRUE(ofs->good());

    *ofs << "work-directory = /" << std::endl;
    char* args[] = {
            const_cast<char*>("deamon"),
            const_cast<char*>("--config-file"),
            const_cast<char *>(cfg_path.c_str())
    };

    _dm = std::make_unique<daemon_base>(args[0], 3, static_cast<char**>(args));
    const char* expected = "/";
    ASSERT_STREQ(_dm->work_directory().c_str(), expected);
}

TEST_F(DaemonTest, CliParsingHelp ) {
    EXPECT_TRUE(true == true);
    char* args[] = {
            const_cast<char*>("deamon"),
            const_cast<char*>("--help"),
    };
    testing::internal::CaptureStdout();
    _dm = std::make_unique<daemon_base>(args[0], 3, static_cast<char**>(args));
    std::string output = testing::internal::GetCapturedStdout();
#if 0
    const char* expected = "Allowed options:"
                             "--help                 This help"
                             "--work-directory arg  set daemon's working directory"
                             "--config-file arg     full path to configuration file";
#endif
    const char* expected ="Allowed options:\n  --help                 This help\n  --work-directory arg  set daemon's working directory\n  --config-file arg     full path to configuration file\n\n";
    ASSERT_STREQ(output.c_str(), expected);
}

