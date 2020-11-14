//
// Created by amironenko on 31/10/2020.
//

// Local headers
#include "daemon.h"
#include "daemon_base_.h"

// STL C++ headers
#include <memory>



daemon_base::daemon_base(std::string name, int argc, char** argv):
        _daemon_base(std::make_unique<daemon_base_>(name, argc, argv))
{
}

int daemon_base::start_daemon()
{
  return _daemon_base->start_daemon();
}

daemon_base::~daemon_base() {}



fs::path daemon_base::work_directory() {return _daemon_base->work_directory();}
fs::path daemon_base::config_file() {return _daemon_base->config_file();}
void daemon_base::set_log_level(int level) {_daemon_base->set_log_level(level);}
int daemon_base::log_level() {return _daemon_base->log_level();}


//} // namespace daemon

