//
// Created by amironenko on 05/11/2020.
//
#include<sstream>
#include<string>
#include<iostream> // TODO: Remove!

#include "mq.h"
#include "mq_.h"

mq::mq_::mq_(std::string name, attr_uqp_t attr, int oflag, mode_t mode) :
        _name(name),
        _desc(mq_open(_name.c_str(), oflag, mode , attr.get())),
        _recv_size(attr ? attr->mq_msgsize : mq::MAX_MSG_SIZE),
        _recv_buffer(std::shared_ptr<char>((char*)calloc(sizeof(char), _recv_size), free))
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
    _recv_size = sp ? sp->mq_msgsize : mq::MAX_MSG_SIZE;
    _recv_buffer = std::shared_ptr<char>((char*)calloc(sizeof(char), _recv_size), free);
}


mq::mq_::~mq_() {
    mq_close(_desc);
}

mq::attr_uqp_t mq::mq_::get_attr() const {
    mq::attr_uqp_t attr = std::make_unique<struct mq_attr>();
    if (int ret = (ret = mq_getattr(_desc, attr.get() )); ret != 0)
    {
        std::stringstream ss;
        ss << "Call of mq_getattr has failed, returning ";
        ss << ret;
        throw std::runtime_error(ss.str());
    }
    return attr;
}

mq::attr_uqp_t mq::mq_::set_attr(mq::attr_wkp_t attr_orig) {

    auto attr = attr_orig.lock();
    if (!attr) {
        std::stringstream ss;
        ss << "The nullptr is passed as a parameter to set_attr function";
        throw std::runtime_error(ss.str());
    }

    attr_uqp_t prev_attr = std::make_unique<mq_attr>();

    if (int ret = mq_setattr(_desc, attr.get(), prev_attr.get()); ret != 0)
    {
        std::stringstream ss;
        ss << "Call of mq_setattr has failed, returning ";
        ss << ret;
        throw std::runtime_error(ss.str());
    }

    return prev_attr;
}

void mq::mq_::send(msg_t msg) {

    auto buff = msg.ptr.lock();

    if (!buff) {
        std::stringstream ss;
        ss << "The buffer pointer is null passed in msg_t";
        throw std::runtime_error(ss.str());
    }


    if (int ret = mq_send(_desc, buff.get(), msg.size, msg.priority); ret != 0) {
        std::stringstream ss;
        ss << "Call of mq_send has failed,";
        ss << "size = " << msg.size << "bytes, ";
        ss << "message: " << buff.get();
        throw std::runtime_error(ss.str());
    }
}

void mq::mq_::send(const std::string& msg, unsigned int priority) {

    if (int ret = mq_send(_desc, msg.c_str(), msg.size(), priority); ret != 0) {
        std::stringstream ss;
        ss << "Call of mq_send has failed,";
        ss << "size = " << msg.size() << "bytes, ";
        ss << "message: " << msg;
        throw std::runtime_error(ss.str());
    }
}


mq::msg_t mq::mq_::receive() {
    mq:msg_t msg;
    msg.ptr = _recv_buffer;

    msg.size = mq_receive(_desc, _recv_buffer.get(), _recv_size, &msg.priority);
    if(-1 == msg.size) {
        std::stringstream ss;
        ss << "Call of mq_receive has failed, ";
        ss << "max message size: " << _recv_size << ", ";
        ss << "priority = " << msg.priority;
        throw std::runtime_error(ss.str());
    }
    return msg;
}