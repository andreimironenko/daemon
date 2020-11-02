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
        explicit core(std::string name);

        virtual ~core() = default;
        core(core&&) = delete;
        core(const core&) = delete;
        core& operator=(core&&) = delete;
        core& operator=(const core&) = delete;
        void start();
        void stop();
        void restart();
        std::string status();
    };
} //namespace daemon

