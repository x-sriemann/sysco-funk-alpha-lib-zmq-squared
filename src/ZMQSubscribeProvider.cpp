//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//
#include "ZMQHelpers.h"
#include "ZMQSubscribeProvider.h"
#include <memory>

namespace scf{
    namespace zmq{

        ZMQSubscribeProvider::ZMQSubscribeProvider(ZMQConnector &aConnector, std::unique_ptr<ZMQBaseUri> uri, const std::string &defaultTopic)
        :topic(defaultTopic),subscriber(zmq_socket(aConnector.getContext(),ZMQ_SUB)){
            auto rc=zmq_connect(subscriber,uri->getUri().c_str());
            assert(rc==0);
            zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,topic.c_str(),topic.size());

        }
        ZMQSubscribeProvider::~ZMQSubscribeProvider(){
            zmq_close(subscriber);
        }
        bool ZMQSubscribeProvider::receive(const std::string &aTopic){

            if(not topic.empty()) setTopic(aTopic);

            auto env=s_recv(subscriber);
            if(env == nullptr){return false;}
            topic=std::string(env);
            free(env);

            env= s_recv(subscriber);
            if(env == nullptr){return false;}
            replyTo=std::string(env);
            free(env);

            env= s_recv(subscriber);
            if(env == nullptr){return false;}
            message=ZMQMessage(env);
            free(env);

            return true;
        }
         const std::string &ZMQSubscribeProvider::getReplyTo() const {
            return replyTo;
        }
        const ZMQMessage &ZMQSubscribeProvider::getMessage() const{
            return message;
        }

        ZMQSubscribeProvider &ZMQSubscribeProvider::setTopic(const std::string &aTopic) {
            topic=aTopic;
            zmq_setsockopt(subscriber,ZMQ_SUBSCRIBE,topic.c_str(),topic.size());
            return *this;
        }
    }
}