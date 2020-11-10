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

// Boost headers
#include <boost/program_options.hpp>

// Linux headers

using std::cout;
using std::endl;
using std::stringstream;

namespace po = boost::program_options;

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
    std::string _work_directory;
#if 0
    bool _keep_open_file_descriptor = false;
    bool _keep_std_streams = false;
    bool _skip_umask_update = false;
    int _max_file_descriptor = 8192;
#endif
    std::string _config_fn;



  int parse_cli_options(int argc, char** argv);

  public:
  explicit core_(std::string name, int argc, char** argv);
  virtual ~core_() = default;

};

core::core(std::string name, int argc, char** argv):
  _core(std::make_unique<core_>(name, argc, argv))
{
}

core::~core() {}


core::core_::core_(std::string name, int argc, char** argv) :
  _name(name)
{
  parse_cli_options(argc, argv);
  if (!_work_directory.empty()) {
      std::cout << "work directory = " << _work_directory << std::endl;
  }
  else {
      std::cout << "work directory is not provided!" << std::endl;
  }
}

int core::core_::parse_cli_options(int argc, char** argv)
{
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", " This help")
    ("work-directory", po::value<std::string>(&this->_work_directory),
            "set daemon's working directory")
    ("config-file", po::value<std::string>(&this->_config_fn),
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
#endif

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  if(!this->_config_fn.empty())
  {
      po::store(po::parse_config_file(this->_config_fn.c_str(), desc), vm);
      po::notify(vm);
  }

  if (vm.count("help")) {
    cout << desc << "\n";
    return 0;
  }

  return -1;
}

//} // namespace daemon

