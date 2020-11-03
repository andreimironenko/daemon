//
// Created by amironenko on 02/11/2020.
//
#pragma once

#include <array>
#include <memory>
#include <string_view>

// Linux headers
extern "C" {
#include <mqueue.h>
#include <fcntl.h>
}

class mq {

    // Forward implementation class declaration
    class mq_;

//typedef struct mq_attr mq_attr_t;

    // Type declarations
    using attr_ptr_t = std::unique_ptr<mq_attr>;

// PRIVATE INSTANCE VARIABLES
    // Implementation class pointer
    std::unique_ptr <mq_> _mq;

public:
    // PUBLIC STATIC MEMBERS
#if defined (MQ_MAX_MSG_COUNT)
    static const long MAX_MSG_COUNT = MQ_MAX_MSG_COUNT;
#else
    static const long MAX_MSG_COUNT = 100;
#endif

#if defined (MQ_MAX_MSG_SIZE)
    static const long MAX_MSG_SIZE = MQ_MAX_MSG_SIZE;
#else
    static const long MAX_MSG_SIZE = 2048;
#endif

    static int unlink(const std::string &queue);


    // PUBLIC INSTANCE MEMBERS
    /**!
     * Message queue Ctor, only required parameter is the name, by default it will create an exclusive message queue
     * with given name. It can throw an exception if the queue with the same name is already exist and O_EXCEL is used,
     * or when one of these flags are used O_RDONLY, O_WRONLY or O_RDWR and there is no queue found.
     *
     * @param name of the message queue, usual pattern is "/mqname"
     * @param oflag
     * @param mq_flags Description flags: 0 or O_NONBLOCK
     * @param mq_maxmsg Maximum number of messages on queue
     * @param mq_msgsize Maximum message size in bytes
     * @param mq_curmsgs Number of messages currently in queue
    **/

    //struct mq_attr attr = {0, MAX_MSG_COUNT, MAX_MSG_SIZE,0,}
    explicit mq(const std::string &name,
                int oflag = O_CREAT | O_EXCL,
                mode_t mode = 0700,
                attr_ptr_t attr =
                //std::make_unique<mq_attr_t>(0, MAX_MSG_COUNT, MAX_MSG_SIZE, 0)
                std::make_unique<mq_attr>()
    );

    ~mq() = default;

    mq(mq &&) = delete;

    mq(const mq &) = delete;

    mq &operator=(mq &&) = delete;

    mq &operator=(const mq &) = delete;


    [[nodiscard]] attr_ptr_t get_attr() const;

    void set_attr(attr_ptr_t attr);

    void send(std::string_view msg, unsigned int priority = 0);

    ssize_t receive(std::string_view &msg, unsigned int &priority);

    // TODO: implement mq_teimedsend, mq_timedrecive and mq_notify
};

