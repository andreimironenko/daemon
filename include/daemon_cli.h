//
// Created by amironenko on 31/10/2020.
//

#pragma once

#include <memory>
#include <string>

class daemon_cli {
    class daemon_cli_;
    std::unique_ptr<daemon_cli_> _daemon_cli;

public:
    explicit daemon_cli(std::string daemon_name);

    virtual ~daemon_cli();

    daemon_cli(daemon_cli&&) = delete;
    daemon_cli(const daemon_cli&) = delete;
    daemon_cli operator=(daemon_cli&&) = delete;
    daemon_cli& operator=(const daemon_cli&) = delete;


    template <typename S, typename R>
    std::unique_ptr<R> send(const S& msg);

};

