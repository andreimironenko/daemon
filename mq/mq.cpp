//
// Created by amironenko on 02/11/2020.
//
#include <sstream>

#include "mq.h"

class mq::mq_{

    // PRIVATE INSTANCE VARIABLES
    std::string _name; //daemon process name or maybe file name argv[0]?
    attr_ptr _attr;
    mqd_t _desc;
    std::array<char, mq::MAX_MSG_SIZE> _buffer = {};
    int _stop_flag = 0;

public:
    // PUBLIC INSTANCE MEMBERS
    mq_(std::string name, int oflag, mode_t mode, attr_ptr attr);
    ~mq_() = default;
    mq_(mq_&&) = delete;
    mq_(const mq_&) = delete;
    mq_& operator=(mq_&&) = delete;
    mq_& operator=(const mq&) = delete;

    attr_ptr get_attr();
    void set_attr(attr_ptr attr);
    void send();
    void receive();
};


mq::mq_::mq_(std::string name, int oflag, mode_t mode, mq::attr_ptr attr):
        _name(name),
        _attr(std::move(attr)),
        _desc(mq_open(_name.c_str(), oflag, mode ,&_attr))
{
    if (_desc == static_cast<mqd_t>(-1))
    {
        std::stringstream ss;
        ss << "Creation of ";
        ss << _name;
        ss << " daemon's message queue has failed";

        throw std::runtime_error(ss.str());
    }
}

mq::attr_ptr mq::mq_::get_attr() {
    return mq::attr_ptr();
}

void mq::mq_::set_attr(mq::attr_ptr attr) {

}

void mq::mq_::send() {

}

void mq::mq_::receive() {

}

mq::mq(const std::string name, int oflag, mode_t mode, mq::attr_ptr attr) {

}

mq::attr_ptr mq::get_attr() {
}

void mq::set_attr(mq::attr_ptr attr) {

}

void mq::send() {

}

void mq::receive() {

}

void mq::unlink(std::string name) {

}

