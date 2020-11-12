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
class core {

    class core_;
    std::unique_ptr<core_> _core;

public:
    explicit core(std::string name, int argc, char** argv);

    virtual ~core();

    core(core&&) = delete;
    core(const core&) = delete;
    core& operator=(core&&) = delete;
    core& operator=(const core&) = delete;

    fs::path work_directory();
    fs::path config_file();
    int start_daemon();
    void set_log_level(int level);
    int log_level();
};
//} //namespace daemon


