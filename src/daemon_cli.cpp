//
// Created by amironenko on 14/11/2020.
//

// Loccal headers
#include "daemon_cli.h"
#include "daemon_cli_.h"
#include "nlohmann/json.hpp"

// C++ STL headers
daemon_cli::daemon_cli(std::string daemon_name):
  //_daemon_cli(std::make_unique<daemon_cli_>(daemon_name))
  _daemon_cli(std::make_unique<daemon_cli_>(daemon_name))
{}

daemon_cli::~daemon_cli() {}

template <typename S, typename R>
std::unique_ptr<R> daemon_cli::send(const S& msg) {

}


