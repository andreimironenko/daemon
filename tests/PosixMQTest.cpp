//
// Created by amironenko on 03/11/2020.
//
#include "gtest/gtest.h"
#include "mq.h"

#include <memory>
#include <string>

class PosixMQTest: public ::testing::Test {
protected:
    std::string _mq_name;

public:
    PosixMQTest()
    : _mq_name("/mq_test")
    {
        //mq mq_obj("/mymq");
        // initialization;
    }

    void SetUp( ) override {
        // initialization or some code to run before each test
        mq::unlink(_mq_name);
    }

    void TearDown( ) override {
        // code to run after each test;
        // can be used instead of a destructor,
        // but exceptions can be handled in this function only
    }

    ~PosixMQTest( )  override {
        //resources cleanup, no exceptions allowed
        mq::unlink(_mq_name);
    }

    // shared user datad
};

TEST_F(PosixMQTest, CreateExclusiveMQ ) {
    std::unique_ptr<mq> mqd = std::make_unique<mq>(_mq_name);
    mq::attr_uqp_t attr(mqd->get_attr());
    EXPECT_EQ(attr->mq_maxmsg, 100);
    EXPECT_EQ(attr->mq_msgsize, 2048);
}

TEST_F(PosixMQTest, OpenExistingMQ ) {
    std::unique_ptr<mq> mqd_excl = std::make_unique<mq>(_mq_name);
    std::unique_ptr<mq> mqd = std::make_unique<mq>(_mq_name, nullptr, O_RDWR);
    mq::attr_uqp_t attr(mqd->get_attr());
    EXPECT_EQ(attr->mq_maxmsg, 100);
    EXPECT_EQ(attr->mq_msgsize, 2048);
}

TEST_F(PosixMQTest, ExistingMQChangeAttr) {
    std::unique_ptr<mq> mqd_excl = std::make_unique<mq>(_mq_name);
    std::unique_ptr<mq> mqd = std::make_unique<mq>(_mq_name, nullptr, O_RDWR);
    mq::attr_uqp_t attr(mqd->get_attr());
    EXPECT_EQ(attr->mq_maxmsg, 100);
    EXPECT_EQ(attr->mq_msgsize, 2048);
    mq::attr_shp_t new_attr_sp = std::move(attr);
    new_attr_sp->mq_msgsize = 50;
    mq::attr_uqp_t prev_attr(mqd->set_attr(new_attr_sp));
}

TEST_F(PosixMQTest, OpenExistingMQNullAttr ) {
    std::unique_ptr<mq> mqd_excl = std::make_unique<mq>(_mq_name);
    std::unique_ptr<mq> mqd = std::make_unique<mq>(_mq_name, nullptr, O_RDWR, 0700);
    mq::attr_uqp_t attr(mqd->get_attr());
    EXPECT_EQ(attr->mq_maxmsg, 100);
    EXPECT_EQ(attr->mq_msgsize, 2048);
}

TEST_F(PosixMQTest, MQSendReceive ) {
    std::unique_ptr<mq> mqd_recv = std::make_unique<mq>(_mq_name);
    std::unique_ptr<mq> mqd_send = std::make_unique<mq>(_mq_name, nullptr, O_RDWR);
    auto hello = "Hello";
    auto msg_size = strlen(hello);
    EXPECT_EQ(msg_size, 5);
    auto buff = std::make_shared<char []>(msg_size);
    mq::msg_t msg;
    msg.ptr = buff;
    msg.size = msg_size;
    msg.priority = 0;

    memcpy((void*) buff.get(), (void *)hello, msg.size);

    mqd_send->send(msg);

    // receiving message
    auto [received_msg, received_bytes, priority] = mqd_recv->receive();

    EXPECT_EQ(priority, 0);
    EXPECT_EQ(received_bytes, msg_size);
    EXPECT_TRUE(0 == memcmp(buff.get(), received_msg.lock().get(), msg_size));
}

