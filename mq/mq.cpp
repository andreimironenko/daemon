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

mq::~mq() {}

mq::attr_uqp_t mq::get_attr() const{
    return _mq->get_attr();
}

mq::attr_uqp_t mq::set_attr(mq::attr_wkp_t attr) {
    return _mq->set_attr(std::move(attr));
}

void mq::send(msg_t msg) {
    _mq->send(std::move(msg));
}

mq::msg_t  mq::receive() {
    return _mq->receive();
}

int mq::unlink(std::string queue) {
    return mq_unlink(queue.c_str());
}

