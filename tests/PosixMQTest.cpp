//
// Created by amironenko on 03/11/2020.
//
#include "gtest/gtest.h"
#include "mq.h"
#include "nlohmann/json.hpp"

#include <memory>
#include <string>


using json = nlohmann::json;
namespace ns {
    // a simple struct to model a person
    struct person {
        std::string name;
        std::string address;
        int age;

        friend bool operator==(const person& lhs, const person& rhs) {
            if (
                    lhs.name != rhs.name ||
                    lhs.address != rhs.address ||
                    lhs.age != rhs.age
                    )
            {
                return false;
            }

            return true;
        }

    };

    void to_json(json& j, const person& p) {
        j = json{{"name", p.name}, {"address", p.address}, {"age", p.age}};
    }

    void from_json(const json& j, person& p) {
        j.at("name").get_to(p.name);
        j.at("address").get_to(p.address);
        j.at("age").get_to(p.age);
    }
} //namespace ns

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
        mq::unlink(_mq_name);
    }

    ~PosixMQTest( )  override {
        //resources cleanup, no exceptions allowed
        mq::unlink(_mq_name);
    }

    // shared user data
};

// Creating exclusive message queue with default flags which O_CREAT|O_EXCL
TEST_F(PosixMQTest, CreateExclusiveMQ ) {
    auto mqd = std::make_unique<mq>(_mq_name);
    mq::attr_uqp_t attr(mqd->get_attr());
    EXPECT_EQ(attr->mq_maxmsg, 100);
    EXPECT_EQ(attr->mq_msgsize, 2048);
}


TEST_F(PosixMQTest, CreateExclusiveMQCustAttr ) {
    auto attr = std::make_shared<mq_attr>();
    attr->mq_flags = 0;
    attr->mq_msgsize = 1024;
    attr->mq_maxmsg = 100;
    auto mqd = std::make_unique<mq>(_mq_name, attr, O_CREAT|O_EXCL, 0744);
    auto attr_retrieved{mqd->get_attr()};
    EXPECT_EQ(attr_retrieved->mq_maxmsg, 100);
    EXPECT_EQ(attr_retrieved->mq_msgsize, 1024);
}

TEST_F(PosixMQTest, ExistingMQChangeAttr) {
    std::unique_ptr<mq> mqd_excl = std::make_unique<mq>(_mq_name);
    auto mqd = std::make_unique<mq>(_mq_name, nullptr, O_RDWR);
    auto attr(mqd->get_attr());
    auto new_attr = std::shared_ptr<mq_attr>(std::move(attr));
    new_attr->mq_flags |= O_NONBLOCK;
    auto prev_attr = mqd->set_attr(new_attr);
    auto curr_attr = mqd->get_attr();
    EXPECT_TRUE(!(prev_attr->mq_flags & O_NONBLOCK));
    EXPECT_TRUE(curr_attr->mq_flags & O_NONBLOCK);
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
    auto buff = std::shared_ptr<char>((char*)calloc(sizeof(char),msg_size), free);
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

TEST_F(PosixMQTest, MQSendReceive2 ) {
    std::unique_ptr<mq> mqd_recv = std::make_unique<mq>(_mq_name);
    std::unique_ptr<mq> mqd_send = std::make_unique<mq>(_mq_name, nullptr, O_RDWR);
    auto hello = "Hello";
    auto msg_size = strlen(hello);
    EXPECT_EQ(msg_size, 5);
    auto buff = std::shared_ptr<char>((char*)calloc(sizeof(char),msg_size), free);
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


TEST_F(PosixMQTest, MQSendReceive3 ) {
    std::unique_ptr<mq> mqd_recv = std::make_unique<mq>(_mq_name);
    std::unique_ptr<mq> mqd_send = std::make_unique<mq>(_mq_name, nullptr, O_RDWR);
    std::string hello = "Hello";
    auto buff = std::shared_ptr<char>((char*)calloc(sizeof(char),hello.size()), free);
    mq::msg_t msg;
    msg.ptr = buff;
    msg.size = hello.size();
    msg.priority = 0;

    memcpy((void*) buff.get(), (void *)hello.c_str(), hello.size());

    mqd_send->send(msg);

    // receiving message
    auto [received_msg, received_bytes, priority] = mqd_recv->receive();

    EXPECT_EQ(priority, 0);
    EXPECT_EQ(received_bytes, hello.size());
    EXPECT_TRUE(0 == memcmp(buff.get(), received_msg.lock().get(), hello.size()));
}

TEST_F(PosixMQTest, MQSendReceiveJSON ) {
    std::unique_ptr<mq> mqd_recv = std::make_unique<mq>(_mq_name);
    std::unique_ptr<mq> mqd_send = std::make_unique<mq>(_mq_name, nullptr, O_RDWR);

    ns::person p = {"Ned Flanders", "744 Evergreen Terrace", 60};

    // conversion: person -> json
    json j = p;
    auto str = j.dump();

    auto buff = std::shared_ptr<char>((char*)calloc(sizeof(char),str.size()), free);
    mq::msg_t msg;
    msg.ptr = buff;
    msg.size = str.size();
    msg.priority = 0;
    str.copy(buff.get(), str.size());
    mqd_send->send(msg);

    try {
        auto[received_msg, received_bytes, priority] = mqd_recv->receive();
        auto sp = received_msg.lock();
        if (!sp) {
            EXPECT_TRUE(false);
        }
        auto received_str = std::string(static_cast<const char *>(sp.get()), received_bytes);

        EXPECT_TRUE(str == received_str);
        json j_received = json::parse(received_str);
        auto p_received = j_received.get<ns::person>();
        EXPECT_TRUE(p == p_received);
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

TEST_F(PosixMQTest, MQSendReceiveJSON2 ) {
    std::unique_ptr<mq> mqd_recv = std::make_unique<mq>(_mq_name);
    std::unique_ptr<mq> mqd_send = std::make_unique<mq>(_mq_name, nullptr, O_RDWR);

    ns::person p = {"Ned Flanders", "744 Evergreen Terrace", 60};

    // conversion: person -> json
    json j = p;
    auto msg = j.dump();
    mqd_send->send(msg);

    try {
        auto[received_msg, received_bytes, priority] = mqd_recv->receive();
        auto sp = received_msg.lock();
        if (!sp) {
            EXPECT_TRUE(false);
        }
        auto received_str = std::string(static_cast<const char *>(sp.get()), received_bytes);

        EXPECT_TRUE(msg == received_str);
        json j_received = json::parse(received_str);
        auto p_received = j_received.get<ns::person>();
        EXPECT_TRUE(p == p_received);
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}



