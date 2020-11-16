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

    fs::path work_directory() const;
    void set_work_directory(const fs::path& wd);

    fs::path config_file() const;
    void set_config_file(const fs::path cf);

    int log_level() const;
    void set_log_level(int level);

    virtual int start_daemon();
};
//} //namespace daemon


