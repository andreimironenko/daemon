//
// Created by amironenko on 02/11/2020.
//
#include <sstream>

#include "mq.h"

class mq::mq_{

    // PRIVATE INSTANCE VARIABLES
    std::string _name;
    mqd_t _desc;

public:
    // PUBLIC INSTANCE MEMBERS
    explicit mq_(const std::string &name, int oflag, mode_t mode, attr_ptr_t attr);
    ~mq_();
    mq_(mq_&&) = delete;
    mq_(const mq_&) = delete;
    mq_& operator=(mq_&&) = delete;
    mq_& operator=(const mq&) = delete;

    [[nodiscard]] attr_ptr_t get_attr() const;
    void set_attr(attr_ptr_t attr_ptr);
    void send(std::string_view msg, unsigned int priority=0);
    ssize_t receive(std::string_view& msg, unsigned int &priority);
};



mq::mq_::mq_(const std::string &name, int oflag, mode_t mode, mq::attr_ptr_t attr):
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

mq::mq_::~mq_() {
    mq_close(_desc);
}

mq::attr_ptr_t mq::mq_::get_attr() const {
    int ret = -1;
    mq::attr_ptr_t attr = std::make_unique<struct mq_attr>();
    if(0 != (ret = mq_getattr(_desc, attr.get() )))
    {
        std::stringstream ss;
        ss << "Call of mq_getattr has failed, returning ";
        ss << ret;
        throw std::runtime_error(ss.str());
    }
    return attr;
}

void mq::mq_::set_attr(mq::attr_ptr_t attr) {
    int ret = -1;
    mq::attr_ptr_t old_attr = std::make_unique<struct mq_attr>();
    if(0 != (ret = mq_setattr(_desc, attr.get(), old_attr.get() )))
    {
        std::stringstream ss;
        ss << "Call of mq_setattr has failed, returning ";
        ss << ret;
        throw std::runtime_error(ss.str());
    }
}

void mq::mq_::send(std::string_view msg, unsigned int priority) {
    int ret = -1;
    if (0 != (ret = mq_send(_desc, msg.data(), msg.size(), priority)))
    {
        std::stringstream ss;
        ss << "Call of mq_send has failed, returning ";
        ss << ret;
        throw std::runtime_error(ss.str());
    }
}

ssize_t mq::mq_::receive(std::string_view &msg, unsigned int &priority) {
    ssize_t count = 0;
    if (-1 == (count = mq_receive(_desc, const_cast<char*>(msg.data()), msg.max_size(), &priority)))
    {
        std::stringstream ss;
        ss << "Call of mq_receive has failed";
        throw std::runtime_error(ss.str());
    }
    return count;
}


mq::mq(const std::string &name, int oflag, mode_t mode, mq::attr_ptr_t attr):
_mq(std::make_unique<mq::mq_>(name, oflag, mode, std::move(attr)))
{}

mq::attr_ptr_t mq::get_attr() const{
    return _mq->get_attr();
}

void mq::set_attr(mq::attr_ptr_t attr) {
    _mq->set_attr(std::move(attr));
}

void mq::send(std::string_view msg, unsigned int priority) {
    _mq->send(msg, priority);

}

ssize_t mq::receive(std::string_view& msg, unsigned int &priority) {
    return _mq->receive(msg, priority);
}

int mq::unlink(const std::string& queue) {
    return mq_unlink(queue.c_str());
}