//
// Created by amironenko on 14/11/2020.
//
#pragma once
#include <string>
#include "daemon_cli.h"

class daemon_cli::daemon_cli_ {
  std::string _daemon_name;

  public:
    daemon_cli_(std::string daemon_name);
    virtual ~daemon_cli_();
};


