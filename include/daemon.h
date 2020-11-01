//
// Created by amironenko on 31/10/2020.
//

#pragma once
#include <memory>
#include <string>
namespace daemon {
    // Forward declarations;
    class core {

    class core_;
    std::unique_ptr<core_> _impl;

    public:
        core(std::string name);
        ~core() = default;
        void start();
        void stop();
        void restart();
        std::string status();
    };
} //namespace daemon

