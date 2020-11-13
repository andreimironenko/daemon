//
// Created by amironenko on 31/10/2020.
//
/**!
 * The Linux/Unix daemon baseclass

 */

#pragma once

// C++ STL headers
#include <memory>
#include <string>
#include <filesystem>

// Linux headers
#include <syslog.h>

namespace fs = std::filesystem;

//namespace daemon {
// Forward declarations;
class daemon_base {

    class daemon_base_;
    std::unique_ptr<daemon_base_>_daemon_base;

public:
    explicit daemon_base(std::string name, int argc, char** argv);

    virtual ~daemon_base();

    daemon_base(daemon_base&&) = delete;
    daemon_base(const daemon_base&) = delete;
    daemon_base& operator=(daemon_base&&) = delete;
    daemon_base& operator=(const daemon_base&) = delete;

    fs::path work_directory();
    fs::path config_file();
    int start_daemon();
    void set_log_level(int level);
    int log_level();
};
//} //namespace daemon


