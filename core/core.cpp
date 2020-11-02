//
// Created by amironenko on 31/10/2020.
//

#include "../include/daemon.h"
#include <iostream>
#include <sstream>
#include <array>

// Linux headers
#include <mqueue.h>
#include <fcntl.h>

using std::cout;
using std::endl;
using std::stringstream;

namespace daemon
{
    class core::core_ {

        // STATIC PRIVATE VARIABLES
#if defined (DAEMON_MQ_MAX_MSG_COUNT)
        static const long MQ_MAX_MSG_COUNT = DAEMON_MQ_MAX_MSG_COUNT;
#else
        static const long MQ_MAX_MSG_COUNT= 100;
#endif

#if defined (DAEMON_MQ_MAX_MSG_SIZE)
        static const long MQ_MAX_MSG_SIZE = DAEMON_MQ_MAX_MSG_SIZE;
#else
        static const long MQ_MAX_MSG_SIZE = 2048;
#endif

        static const int MQ_FLAGS = O_CREAT | O_EXCL;
        static const mode_t MQ_PERMS = 0700;



       // PRIVATE INSTANCE VARIABLES
        std::string _name; //daemon process name or maybe file name argv[0]?
        std::string _mq_name;
        struct mq_attr _mq_attr;
        mqd_t _mq_desc;
        std::array<char, MQ_MAX_MSG_SIZE> _mq_buffer = {};
        int _stop_flag = 0;

    public:
        explicit core_(std::string name);
        virtual ~core_() = default;

        void start();
        void stop();
        void restart();
        std::string status();
    };



    core::core_::core_(std::string name) :
            _name(name),
            _mq_name("/mq_" + _name),
            _mq_attr{MQ_FLAGS, MQ_MAX_MSG_COUNT, MQ_MAX_MSG_SIZE,0},
            _mq_desc(mq_open(_mq_name.c_str(), MQ_FLAGS, MQ_PERMS,&_mq_attr))
    {
        if (_mq_desc == static_cast<mqd_t>(-1))
        {
            stringstream ss;
            ss << "Creation of ";
            ss << _name;
            ss << " daemon's message queue has failed";

            throw std::runtime_error(ss.str());
        }
        start();
    }

    void core::core_::start() {
        cout << "daemon started" << endl;
        while(!_stop_flag) {
           auto nm_read = mq_receive(_mq_desc, _mq_buffer.data(), MQ_MAX_MSG_SIZE, 0);

        }
    }
    void core::core_::stop() {cout << "daemon stopped" << endl;}
    void core::core_::restart() {cout << "daemon restarted" << endl;}
    std::string core::core_::status() { return std::string("");}

    core::core(std::string name) : _impl(std::make_unique<core_>(name)) {}
    void core::start() {_impl->start();}
    void core::stop() {_impl->stop();}
    void core::restart() {_impl->restart();}
    std::string core::status() {return _impl->status();}
} // namespace daemon

