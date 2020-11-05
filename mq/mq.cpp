//
// Created by amironenko on 02/11/2020.
//
#include <sstream>

#include "mq.h"
#include "mq_.h"




mq::mq(std::string name, mq::attr_uqp_t attr, int oflag, mode_t mode):
        _mq(new mq::mq_(name, std::move(attr), oflag, mode))
{}

mq::mq(std::string name, mq::attr_wkp_t attr, int oflag, mode_t mode):
        _mq(new mq::mq_(name, std::move(attr), oflag, mode))
{}

mq::mq(const mq &other):
        _mq(new mq::mq_(*other._mq)) {
}

mq::~mq() {}

mq::attr_uqp_t mq::get_attr() const{
    return _mq->get_attr();
}

mq::attr_uqp_t mq::set_attr(mq::attr_wkp_t attr) {
    return _mq->set_attr(std::move(attr));
}

void mq::send(std::weak_ptr<char[]> msg, unsigned int priority) {
    _mq->send(std::move(msg), priority);
}

ssize_t  mq::receive(std::weak_ptr<char[]> msg, std::weak_ptr<unsigned int> priority) {
    return _mq->receive(std::move(msg), std::move(priority));
}

int mq::unlink(std::string queue) {
    return mq_unlink(queue.c_str());
}

