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
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>

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
    pid_t _pid;
    int _log_options;
    int _log_facility;
    int _log_level;

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
    int start_daemon();
    void set_log_level(int level);
    int log_level();
};

core::core(std::string name, int argc, char** argv):
  _core(std::make_unique<core_>(name, argc, argv))
{
}

int core::start_daemon()
{
  return _core->start_daemon();
}

core::~core() {}


core::core_::core_(std::string name, int argc, char** argv) :
  _name(name),
  _desc("Available options"),
  _log_options(LOG_PID|LOG_CONS|LOG_NOWAIT),
  _log_facility(LOG_LOCAL0),
  _log_level(LOG_MASK(LOG_EMERG) | LOG_MASK(LOG_ALERT) | LOG_MASK(LOG_CRIT) | LOG_MASK(LOG_ERR))
{
  parse_cli_options(argc, argv);

  // syslog initialsiation
  openlog(_name.c_str(), _log_options, _log_facility);
}

fs::path core::work_directory() {return _core->work_directory();}
fs::path core::config_file() {return _core->config_file();}
void core::set_log_level(int level) {_core->set_log_level(level);}
int core::log_level() {return _core->log_level();}

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
    //TODO: Add debug level option
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

void core::core_::set_log_level(int level) {_log_level = level;}
int core::core_::log_level() {return _log_level;}

int core::core_::start_daemon()
{
  int maxfd, fd;
  _pid = fork();

  switch(_pid) // become a background process at first
  {
    case -1:                        // fork is failed, just return
      return -1;
    case 0:                         // child falls through ...
      break;
    default:
      syslog(LOG_INFO, "Parent is out of business");
      _exit(EXIT_SUCCESS);          // while parent terminates
  }

  // Become a leader of new session
  if (-1 == setsid())
  {
    return -1;
  }

  // Second fork ensures that the daemon process is
  // not session leader
  _pid = fork();
  switch(_pid)
  {
    case -1:                      // fork is failed, just return
      return -1;
    case 0:                       // child falls through ...
      syslog(LOG_INFO, "Starting daemon with PID");
      break;
    default:
      syslog(LOG_INFO, "Parent is out of business");
      _exit(EXIT_SUCCESS);        // while parent terminates
  }

  // Clear file mode creation mask
  syslog(LOG_INFO, "Cleairing file mode creation mask");
  umask(0);

  // Change to work directory
  syslog(LOG_INFO, "Changing to workdir");
  chdir(_work_directory.c_str());

  // Close all open files
  syslog(LOG_INFO, "Closing all open files");
  maxfd = sysconf(_SC_OPEN_MAX);
  if (maxfd == -1)
    maxfd = 8192;

  for (fd = 0; fd < maxfd; fd++)
  {
    close(fd);
  }

  // reopen standard fd's to /dev/null
  syslog(LOG_INFO, "Reopening standard fd's to /dev/null");
  close(STDIN_FILENO);

  fd = open("/dev/null", O_RDWR);
  if (fd != STDIN_FILENO)          // fd should be 0
    return -1;
  if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
    return -1;
  if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
    return -1;

  //sleep(0.5);
  _pid = getpid();
  syslog(LOG_INFO, "Now daemon is fully functional, PID = %ld", (long)_pid);

  while(true)
  {
    syslog(LOG_INFO, "I'm still alive, my PID = %ld", (long)getpid());
    sleep(5);
  }

  return 0;
}

//} // namespace daemon

