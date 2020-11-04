//
// Created by amironenko on 03/11/2020.
//
#include "gtest/gtest.h"
#include "mq.h"

#include <memory>
#include <string>

class myTestFixture: public ::testing::Test {
protected:
    std::string _mq_name;
    std::unique_ptr<mq> _mq;

public:
    myTestFixture()
    : _mq_name("/mq_test")
    {
        //mq mq_obj("/mymq");
        // initialization;
    }

    void SetUp( ) {
        // initialization or some code to run before each test
        mq::unlink(_mq_name.c_str());
    }

    void TearDown( ) {
        // code to run after each test;
        // can be used instead of a destructor,
        // but exceptions can be handled in this function only
    }

    ~myTestFixture( )  {
        //resources cleanup, no exceptions allowed
        mq::unlink(_mq_name.c_str());
    }

    // shared user data
};

TEST_F(myTestFixture, FirstTest) {
    _mq = std::make_unique<mq>(_mq_name.c_str());
    mq::attr_ptr_t attr(_mq->get_attr());
    EXPECT_EQ(attr->mq_maxmsg, 100);
    EXPECT_EQ(attr->mq_msgsize, 2048);
}