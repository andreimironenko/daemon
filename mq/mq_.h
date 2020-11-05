//
// Created by amironenko on 05/11/2020.
//

#pragma once

class mq::mq_{

    // PRIVATE INSTANCE VARIABLES
    std::string _name;
    mqd_t _desc;

public:
    // PUBLIC INSTANCE MEMBERS
    explicit mq_(std::string name, attr_uqp_t attr, int oflag, mode_t mode);
    explicit mq_(std::string name, attr_wkp_t attr, int oflag, mode_t mode);
    ~mq_();
    mq_(mq_&&) = delete;
    mq_(const mq_&) = default;
    mq_& operator=(mq_&&) = delete;
    mq_& operator=(const mq&) = delete;

    [[nodiscard]] attr_uqp_t get_attr() const;
    attr_uqp_t set_attr(attr_wkp_t attr_ptr);

    void send(std::weak_ptr<char[]> msg, unsigned int priority = 0);
    ssize_t  receive(std::weak_ptr<char[]> msg, std::weak_ptr<unsigned int> priority);

};


