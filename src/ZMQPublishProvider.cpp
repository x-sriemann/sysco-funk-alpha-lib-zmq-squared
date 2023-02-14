//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//
#include "ZMQHelpers.h"
#include "ZMQPublishProvider.h"
#include <iostream>

namespace scf {
    namespace zmq {
        ZMQPublishProvider::ZMQPublishProvider(ZMQConnector &aConnector, const ZMQBindings &bindings, const std::string &defaultTopic)
        : connector(aConnector), publisher(zmq_socket(connector.getContext(),ZMQ_PUB)), topic(defaultTopic), replyTo("")
        {
            for(auto &uri:bindings){
               auto rc= zmq_bind(publisher, uri->getUri().c_str());
               if(rc==0) {
                   bindings_used.push_back(uri->getUri());
               }else{
                   std::cout << uri->getUri() << std::endl;

               }
               assert(rc==0);
            }
        }
        ZMQPublishProvider::~ZMQPublishProvider() {
            for(auto &uri:bindings_used){
                zmq_unbind(publisher,uri.c_str());
            }
            if (publisher != nullptr) {
                zmq_close(publisher);
            }
        }
        ZMQPublishProvider &ZMQPublishProvider::sendMessage(const ZMQMessage &aMessage){
            if(not topic.empty()){
                auto size=s_sendmore(publisher,const_cast<char *>(topic.c_str()));
                assert(size==topic.size());
            }
            if(not replyTo.empty()){
                auto size=s_sendmore(publisher,const_cast<char *>(replyTo.c_str()));
                assert(size==replyTo.size());
            }
            auto size=zmq_send (publisher, aMessage.value, aMessage.size, ZMQ_DONTWAIT);
            assert(size==aMessage.size);

            return *this;
        }
    } // scf
} // zmq