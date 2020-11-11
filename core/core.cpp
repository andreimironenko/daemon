//
// Created by amironenko on 31/10/2020.
//

// Local headers
#include "../include/daemon.h"

// STL C++ headers
#include <iostream>
#include <sstream>
#include <array>
#include <memory>
#include <filesystem>

// Boost headers
#include <boost/program_options.hpp>

// Linux headers

using std::cout;
using std::endl;
using std::stringstream;

namespace po = boost::program_options;
namespace fs = std::filesystem;

//namespace daemon
//{
class core::core_ {
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
#if 0
    bool _keep_open_file_descriptor = false;
    bool _keep_std_streams = false;
    bool _skip_umask_update = false;
    int _max_file_descriptor = 8192;
#endif
    int parse_cli_options(int argc, char** argv);

public:
    explicit core_(std::string name, int argc, char** argv);
    virtual ~core_() = default;

    fs::path work_directory();
    void set_work_directory(const fs::path& work_directory);
    fs::path config_file();
    void set_config_file(const fs::path& config_file);

};

core::core(std::string name, int argc, char** argv):
  _core(std::make_unique<core_>(name, argc, argv))
{
}

core::~core() {}


core::core_::core_(std::string name, int argc, char** argv) :
  _name(name),
  _desc("Available options")
{
  parse_cli_options(argc, argv);
}
fs::path core::work_directory() {return _core->work_directory();}
fs::path core::config_file() {return _core->config_file();}

int core::core_::parse_cli_options(int argc, char** argv)
{
    std::string work_directory;
    std::string config_file;
    _desc.add_options()
            ("help", " This help")
            ("work-directory", po::value<std::string>(&work_directory),
             "set daemon's working directory")
            ("config-file", po::value<std::string>(&config_file),
             "full path to configuration file")
            ;
#if 0
    ("keeps-open-file-descriptors", po::bool_switch(&this->_keep_open_file_descriptor),
            "either to keep opened file descriptors, default false")
    ("keeps-std-streams", po::bool_switch(&this->_keep_std_streams),
          "either to keep std steams in, out and error, default false")
          ;
  desc.add_options()
          ( "skip-umask-update", po::bool_switch(&this->_skip_umask_update),
            "either to skip umask update")
            ("max-file-descriptor", po::value<int>(&this->_max_file_descriptor),
             "maximum file descriptor ")
          ;
  auto dummy_option = "dummy";
  auto value = po::value<int>()->default_value(10);
  auto dummy_option_description = "Just one of the dummy options";
  _desc.add_options()
          (dummy_option, value, dummy_option_description)
          ;
#endif

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, _desc), vm);
    po::notify(vm);

    if(!config_file.empty())
    {
        po::store(po::parse_config_file(config_file.c_str(), _desc), vm);
        po::notify(vm);
    }

    set_work_directory(work_directory);
    set_config_file(config_file);

    if (vm.count("help")) {
        cout << _desc << "\n";
    }

    return 0;
}

fs::path core::core_::work_directory() {return _work_directory;}
void core::core_::set_work_directory(const fs::path& work_directory) {_work_directory = work_directory;}
fs::path core::core_::config_file() {return _config_file;}
void core::core_::set_config_file(const fs::path& config_file) {_config_file = config_file;}

//} // namespace daemon

