//
// Created by amironenko on 05/11/2020.
//

#pragma once

class mq::mq_{

    // PRIVATE INSTANCE VARIABLES
    std::string _name;
    mqd_t _desc;
    ssize_t _recv_buffer_size;
    std::shared_ptr<char[]> _recv_buffer;

public:
    // PUBLIC INSTANCE MEMBERS
    explicit mq_(std::string name, attr_uqp_t attr, int oflag, mode_t mode);
    explicit mq_(std::string name, attr_wkp_t attr, int oflag, mode_t mode);
    ~mq_();
    mq_(mq_&&) = delete;
    mq_(const mq_&) = delete;
    mq_& operator=(mq_&&) = delete;
    mq_& operator=(const mq&) = delete;

    [[nodiscard]] attr_uqp_t get_attr() const;
    attr_uqp_t set_attr(attr_wkp_t attr_ptr);

    void send(msg_t msg);
    mq::msg_t  receive();
};


