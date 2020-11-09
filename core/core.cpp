//
// Created by amironenko on 31/10/2020.
//

// Local headers
#include "../include/daemon.h"

// STL C++ headers
#include <iostream>
#include <sstream>
#include <array>

// Boost headers
#include <boost/program_options.hpp>

// Linux headers
#include <mqueue.h>
#include <fcntl.h>

using std::cout;
using std::endl;
using std::stringstream;

namespace po = boost::program_options;

//namespace daemon
//{
class core::core_ {
  std::string _name;

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
  std::cout << "start daemon " << _name << endl;

  std::cout << "argc = " << argc << endl;
  std::cout << "argv : " << argv << endl;
}

int core::core_::parse_cli_options(int argc, char** argv)
{
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help", "produce help message")
    ("compression", po::value<int>(), "set compression level")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("help")) {
    cout << desc << "\n";
    return 0;
  }

  if (vm.count("compression")) {
    cout << "Compression level was set to " 
      << vm["compression"].as<int>() << ".\n";
    return 0;
  } else {
    cout << "Compression level was not set.\n";
    return -1;
  }

  return -1;
}

//} // namespace daemon

