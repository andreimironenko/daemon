//
// Created by amironenko on 05/11/2020.
//
#include<sstream>
#include<string>

#include "mq.h"
#include "mq_.h"

mq::mq_::mq_(std::string name, attr_uqp_t attr, int oflag, mode_t mode) :
        _name(name),
        _desc(mq_open(_name.c_str(), oflag, mode , attr.get())),
        _recv_buffer_size(attr ? attr->mq_msgsize: 2048),
        _recv_buffer(std::make_shared<char[]>(_recv_buffer_size))
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
    _recv_buffer_size = sp ? sp->mq_msgsize: 2048;
    _recv_buffer = std::make_shared<char[]>(_recv_buffer_size);
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

void mq::mq_::send(msg_t msg) {

    auto buff_sp = msg.ptr.lock();

    if (!buff_sp) {
        std::stringstream ss;
        ss << "The buffer pointer is null passed in msg_t";
        throw std::runtime_error(ss.str());
    }

    if (0 != mq_send(_desc, buff_sp.get(), msg.size, msg.priority)) {
        std::stringstream ss;
        ss << "Call of mq_send has failed,";
        ss << "size = " << msg.size << "bytes, ";
        ss << "message: " << buff_sp.get();
        throw std::runtime_error(ss.str());
    }
}


mq::msg_t mq::mq_::receive() {
    ssize_t ret = -1;
    mq::msg_t msg{_recv_buffer, 0, 0};

    msg.size = mq_receive(_desc, _recv_buffer.get(), _recv_buffer_size, &msg.priority);
    if(-1 == msg.size) {
        std::stringstream ss;
        ss << "Call of mq_receive has failed, ";
        ss << "max message size: " << sizeof(_recv_buffer.get()) << ", ";
        ss << "priority = " << msg.priority;
        throw std::runtime_error(ss.str());
    }

    return msg;
}



