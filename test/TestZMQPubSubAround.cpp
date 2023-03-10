//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//


#include <gtest/gtest.h>
#include <thread>

#define private public
#include <ZMQBaseUri.h>
#include <ZMQUriFactory.h>
#include <ZMQConnector.h>
#include <ZMQPublishProvider.h>
#include <ZMQSubscribeProvider.h>
#include <tracy/Tracy.hpp>

/**
 * Uri Creation Tests
 * by one
 */
TEST(ZMQ_URI_TEST,TCP_BUILDING){
    scf::zmq::ZMQUriFactory factory{};

    auto anUri=std::move(factory.buildUri({scf::zmq::ZMQUriType::TCP,""}));
    ASSERT_TRUE(anUri != nullptr);
    ASSERT_EQ(anUri->getUri(),"tcp://*:5556");
    anUri.reset();
    anUri=std::move(factory.buildUri({scf::zmq::ZMQUriType::TCP,""}));
    ASSERT_TRUE(anUri != nullptr);
    ASSERT_EQ(anUri->getUri(),"tcp://*:5557");
    anUri.reset();
    // retrieve TCP with specified port number
    anUri=std::move(factory.buildUri({scf::zmq::ZMQUriType::TCP,"5556"}));
    ASSERT_TRUE(anUri != nullptr);
    ASSERT_EQ(anUri->getUri(),"tcp://*:5556");
    anUri.reset();
}
TEST(ZMQ_URI_TEST,IPC_BUILDING){
    scf::zmq::ZMQUriFactory factory{};

    auto anUri=std::move(factory.buildUri({scf::zmq::ZMQUriType::IPC,""}));
    ASSERT_TRUE(anUri == nullptr);

    anUri=std::move(factory.buildUri({scf::zmq::ZMQUriType::IPC,"token"}));
    ASSERT_TRUE(anUri != nullptr);
    ASSERT_EQ(anUri->getUri(),"ipc://token.ipc");
}
TEST(ZMQ_URI_TEST,INPROC_BUILDING){
    scf::zmq::ZMQUriFactory factory{};

    auto anUri=std::move(factory.buildUri({scf::zmq::ZMQUriType::INPROC,""}));
    ASSERT_TRUE(anUri == nullptr);

    anUri=std::move(factory.buildUri({scf::zmq::ZMQUriType::INPROC,"token"}));
    ASSERT_TRUE(anUri != nullptr);
    ASSERT_EQ(anUri->getUri(),"inproc://token");
}


/**
 * Uri Creation Bindings
 * in group
 */
TEST(ZMQ_URI_TEST,BINDING_BUILDING){
    scf::zmq::ZMQUriFactory factory{};
    scf::zmq::ZMQBindings bindings;
    factory.createBindings(bindings,
                           {{scf::zmq::ZMQUriType::INPROC, "token"},
                            {scf::zmq::ZMQUriType::IPC,    "token"},
                            {scf::zmq::ZMQUriType::TCP,    "5556"}}
                            );
    ASSERT_EQ(bindings.size(),3);
    ASSERT_STREQ(bindings[0]->getUri().c_str(),"inproc://token");
    ASSERT_STREQ(bindings[1]->getUri().c_str(),"ipc://token.ipc");
    ASSERT_STREQ(bindings[2]->getUri().c_str(),"tcp://*:5556");
}

TEST(ZMQ_CONNECTOR_TEST, INIT)
{
    using namespace scf::zmq;
    ZMQConnector conn{SharedContext};
    ASSERT_EQ(SharedContext.isZMQType(ZMQContextType::SHARED), conn.isSharedConnection());
    auto same=(SharedContext.getContext())==(conn.getContext());
    ASSERT_TRUE(same);
}

TEST(ZMQPublishProvider, init)
{
    using namespace scf::zmq;
    ZMQUriFactory factory{};
    scf::zmq::ZMQBindings bindings;
    factory.createBindings(bindings,{{ZMQUriType::INPROC,"threads"}});
    ZMQConnector conn{SharedContext};
    ZMQPublishProvider pub{conn,bindings,"default"};
    ASSERT_STREQ(pub.topic.c_str(),"default");
    pub.setTopic("test");
    ASSERT_STREQ(pub.topic.c_str(),"test");
    ASSERT_STREQ(pub.replyTo.c_str(),"");
    pub.setReplyTo("me");
    ASSERT_STREQ(pub.replyTo.c_str(),"me");
    ASSERT_TRUE(pub.publisher != nullptr);
    ASSERT_TRUE(pub.connector.getContext()==SharedContext.getContext());
}
void publishPeriodic(scf::zmq::ZMQConnector *conn, bool *running, int *counter)
{
    tracy::SetThreadName("Publisher");
    using namespace scf::zmq;
    ZMQBindings bindings;
    ZMQUriFactory factory{};
    factory.createBindings(bindings, {{ZMQUriType::IPC, "threads"}});
    ZMQPublishProvider pub{*conn, bindings, "default"};
    pub.setReplyTo("pub");
    sleep(1);
    while(*running)
    {
        FrameMarkStart("Receiving Message");
        pub.sendMessage(ZMQMessage(std::to_string(++(*counter))));
        FrameMarkEnd("Receiving Message");
    }
    ASSERT_TRUE(true); // we are running, no need to test more
}

TEST(ZMQPublishProvider, start_threads) {
    using namespace scf::zmq;
    ZMQConnector conn{SharedContext};
    bool running=true;
    int t1_counter=0;
    std::thread t1(publishPeriodic, &conn, &running, &t1_counter);
    sleep(2);
    running=false;
    ASSERT_GT(t1_counter,0);
    std::cout << t1_counter << std::endl;
    t1.join();

}

TEST(ZMQSubscribeProvider, init)
{
    using namespace scf::zmq;
    ZMQConnector conn{SharedContext};
    ZMQUriFactory factory{};
    ZMQSubscribeProvider sub{conn,factory.buildUri({ZMQUriType::INPROC, "threads"}),"default"};

    ASSERT_STREQ(sub.topic.c_str(),"default");

}

void SubscriberLoop(scf::zmq::ZMQConnector *conn, bool *running, int *counter)
{
    using namespace scf::zmq;
    tracy::SetThreadName("SubscriberLoop");
    ZMQUriFactory factory{};
    ZMQSubscribeProvider sub{*conn,factory.buildUri({ZMQUriType::IPC, "threads"}),"default"};
    while(*running)
    {
        FrameMarkStart("Receiving Message");
        if(sub.receive()){
            FrameMarkEnd("Receiving Message");
            ASSERT_STREQ(sub.getReplyTo().c_str(),"pub");
            *counter= atol(sub.getMessage().value);
        }

    }
    ASSERT_TRUE(true); // we are running, no need to test more
}

TEST(ZMQPublishSubscriber, start_both_threads) {
    using namespace scf::zmq;

    tracy::SetThreadName("Subscriber");
    ZMQConnector conn{SharedContext};
    bool runningT1=true;
    bool runningT2=true;

    int t1_counter=0;
    int t2_counter=0;
    std::thread t1(SubscriberLoop,&conn, &runningT1, &t1_counter);
    std::thread t2(publishPeriodic, &conn, &runningT2, &t2_counter);
    sleep(2);
    runningT2=false;
    sleep(1);
    runningT1=false;
    t2.join();
    t1.join();
    std::cout << "t1: " << t1_counter << std::endl;
    std::cout << "t2: " << t2_counter << std::endl;
    ASSERT_GT(t2_counter,0);

}
void PublishTopicOne(scf::zmq::ZMQConnector *conn, bool *running)
{

    sleep(1);
    using namespace scf::zmq;
    tracy::SetThreadName("PublishTopicOne");
    ZMQBindings bindings;
    ZMQUriFactory factory{};
    factory.createBindings(bindings, {{ZMQUriType::IPC, "thread"}});
    ZMQPublishProvider pub{*conn, bindings, "TopicOne"};
    pub.setReplyTo("one_pub");
    while(*running)
    {
        FrameMarkStart("PublishTopicOne Message");
        pub.sendMessage(ZMQMessage("one"));
        FrameMarkEnd("PublishTopicOne Message");
    }
    ASSERT_TRUE(true); // we are running, no need to test more
}
void PublishTopicTwo(scf::zmq::ZMQConnector *conn, bool *running)
{
    sleep(1);
    using namespace scf::zmq;
    tracy::SetThreadName("PublishTopicTwo");
    ZMQBindings bindings;
    ZMQUriFactory factory{};
    factory.createBindings(bindings, {{ZMQUriType::IPC, "thread"}});
    ZMQPublishProvider pub{*conn, bindings, "TopicTwo"};
    pub.setReplyTo("two_pub");
    while(*running)
    {
        FrameMarkStart("PublishTopicTwo Message");
        pub.sendMessage(ZMQMessage("two"));
        FrameMarkEnd("PublishTopicTwo Message");
    }
    ASSERT_TRUE(true); // we are running, no need to test more
}
void SubscriberOne(scf::zmq::ZMQConnector *conn, bool *running, int *counter)
{
    using namespace scf::zmq;
    tracy::SetThreadName("SubscriberOne");
    ZMQUriFactory factory{};
    ZMQSubscribeProvider sub{*conn,factory.buildUri({ZMQUriType::IPC, "thread"}),"TopicOne"};
    //sub.setTopic("TopicOne");
    while(*running)
    {
        FrameMarkStart("SubscriberOne Message");
        if(sub.receive("TopicOne")){
            FrameMarkEnd("SubscriberOne Message");
            ++(*counter);
            ASSERT_STREQ(sub.getReplyTo().c_str(),"one_pub");
        }

    }
    ASSERT_TRUE(true); // we are running, no need to test more
}
void SubscriberTwo(scf::zmq::ZMQConnector *conn, bool *running, int *counter)
{
    using namespace scf::zmq;
    tracy::SetThreadName("SubscriberTwo");
    ZMQUriFactory factory{};
    ZMQSubscribeProvider sub{*conn,factory.buildUri({ZMQUriType::IPC, "thread"}),"TopicTwo"};
    //sub.setTopic("TopicTwo");
    while(*running)
    {
        FrameMarkStart("SubscriberTwo Message");
        if(sub.receive("TopicTwo")){
            FrameMarkEnd("SubscriberTwo Message");
            ++(*counter);
            ASSERT_STREQ(sub.getReplyTo().c_str(),"two_pub");
        }

    }
    ASSERT_TRUE(true); // we are running, no need to test more
}

TEST(ZMQPublishPublishSubscriberSubscriber, start_four_threads) {

    using namespace scf::zmq;

    ZMQConnector conn{SharedContext};
    ZMQConnector connSUB{};
    ZMQConnector connPUB{};
    bool runningTS=true;
    bool runningTP=true;
    int counterTS1=0;
    int counterTS2=0;
    FrameMarkStart("Subscriber-start");
    std::thread ts1(SubscriberOne,&connSUB, &runningTS, &counterTS1);
    std::thread ts2(SubscriberTwo,&connPUB, &runningTS, &counterTS2);
    sleep(1);
    FrameMarkStart("Publisher-start");
    std::thread tp1(PublishTopicOne, &connSUB, &runningTP);
    std::thread tp2(PublishTopicTwo, &connPUB, &runningTP);
    sleep(2);
    runningTP=false;
    sleep(1);
    runningTS=false;
    tp1.join();
    tp2.join();
    FrameMarkEnd("Subscriber-start");
    ts1.join();
    ts2.join();
    std::cout << counterTS1 << std::endl;
    std::cout << counterTS2 << std::endl;
    FrameMarkEnd("Publisher-start");
}