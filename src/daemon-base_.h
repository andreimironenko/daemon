//
// Created by amironenko on 13/11/2020.
//
#pragma once

// Boost headers
#include <boost/program_options.hpp>

using signal_handler_t = void (*) (int);

namespace po = boost::program_options;
namespace fs = std::filesystem;

//namespace daemon
//{
class daemon_base::daemon_base_ {
protected:
    std::string _name;
    /**! Command and config options:
     * --work-directory
     * --keep-open-file-descriptors
     * --keep-std-streams
     * --no-umask-update
     * --max-file-descriptor (to close. default 8192)
     */
    fs::path _work_directory;
    fs::path _config_file;
    po::options_description _desc;
    pid_t _pid;
    int _log_options;
    int _log_facility;
    int _log_level;

    std::map<int, signal_handler_t> _signals;


#if 0
    bool _keep_open_file_descriptor = false;
    bool _keep_std_streams = false;
    bool _skip_umask_update = false;
    int _max_file_descriptor = 8192;
#endif
    int parse_cli_options(int argc, char** argv);

public:
    explicit daemon_base_(std::string name, int argc, char** argv);
    virtual ~daemon_base_() = default;

    fs::path work_directory();
    void set_work_directory(const fs::path& work_directory);
    fs::path config_file();
    void set_config_file(const fs::path& config_file);
    int start_daemon();
    void set_log_level(int level);
    int log_level();

    void set_signal_handler(int sig, signal_handler_t handler);
};
