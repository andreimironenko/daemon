//
// Created by amironenko on 05/11/2020.
//
#include<sstream>

#include "mq.h"
#include "mq_.h"

mq::mq_::mq_(std::string name, attr_uqp_t attr, int oflag, mode_t mode) :
        _name(name),
        _desc(mq_open(_name.c_str(), oflag, mode , attr.get()))
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

mq::mq_::mq_(std::string name, attr_wkp_t attr, int oflag, mode_t mode) :
        _name(name)
{

    auto sp = attr.lock(); //get the shared pointer
    _desc = mq_open(_name.c_str(), oflag, mode , sp ? sp.get() : NULL );
    if (_desc == static_cast<mqd_t>(-1)) {
        std::stringstream ss;
        ss << "Creation of ";
        ss << _name;
        ss << " daemon's message queue has failed";

        throw std::runtime_error(ss.str());
    }
}


mq::mq_::~mq_() {
    mq_close(_desc);
}

mq::attr_uqp_t mq::mq_::get_attr() const {
    int ret = -1;
    mq::attr_uqp_t attr = std::make_unique<struct mq_attr>();
    if (0 != (ret = mq_getattr(_desc, attr.get() )))
    {
        std::stringstream ss;
        ss << "Call of mq_getattr has failed, returning ";
        ss << ret;
        throw std::runtime_error(ss.str());
    }
    return attr;
}

mq::attr_uqp_t mq::mq_::set_attr(mq::attr_wkp_t attr) {
    int ret = -1;
    auto attr_sp = attr.lock();
    if (!attr_sp) {
        std::stringstream ss;
        ss << "The nullptr is passed as a parameter to set_attr function";
        ss << ret;
        throw std::runtime_error(ss.str());
    }

    attr_uqp_t prev_attr = std::make_unique<mq_attr>();

    if (0 != (ret = mq_setattr(_desc, attr_sp.get(), prev_attr.get() )))
    {
        std::stringstream ss;
        ss << "Call of mq_setattr has failed, returning ";
        ss << ret;
        throw std::runtime_error(ss.str());
    }

    return prev_attr;
}

void mq::mq_::send(std::weak_ptr<char[]> msg, unsigned int priority) {
    auto msg_spt = msg.lock();

    if (!msg_spt) {
        std::stringstream ss;
        ss << "The nullptr is passed as a parameter to send function";
        throw std::runtime_error(ss.str());
    }

    if (0 != mq_send(_desc, msg_spt.get(),sizeof(msg_spt.get()), priority)) {
        std::stringstream ss;
        ss << "Call of mq_send has failed";
        throw std::runtime_error(ss.str());
    }
}


ssize_t mq::mq_::receive(std::weak_ptr<char[]> msg, std::weak_ptr<unsigned int> priority) {
    ssize_t ret = -1;
    auto msg_spt = msg.lock();

    if(!msg_spt) {
        std::stringstream ss;
        ss << "The nullptr is passed as msg parameter to receive function";
        throw std::runtime_error(ss.str());
    }

    auto priority_spt = priority.lock();

    if(!priority_spt) {
        std::stringstream ss;
        ss << "The nullptr is passed as priority parameter to receive function";
        throw std::runtime_error(ss.str());
    }

    ret = mq_receive(_desc, msg_spt.get(),sizeof(msg_spt.get()), priority_spt.get());
    if(-1 == ret) {
        std::stringstream ss;
        ss << "Call of mq_send has failed";
        throw std::runtime_error(ss.str());
    }
    return ret;
}



