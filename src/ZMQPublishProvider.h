//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//

#ifndef COMMUNICATION_ZMQPUBLISHER_H
#define COMMUNICATION_ZMQPUBLISHER_H
#include "ZMQHelpers.h"
#include "ZMQConnector.h"
#include "ZMQBaseUri.h"
#include "ZMQUriFactory.h"
#include "ZMQMessage.h"

#include <vector>
#include <string>

namespace scf {
    namespace zmq {

        class ZMQPublishProvider {
        public:
            ZMQPublishProvider(ZMQConnector &aConnector, const ZMQBindings &bindings, const std::string &defaultTopic="");
            ~ZMQPublishProvider();
            ZMQPublishProvider &setTopic(const std::string &aTopic){
                topic=aTopic; return *this;
            }
            ZMQPublishProvider &sendMessage(const ZMQMessage &aMessage);
            ZMQPublishProvider &setReplyTo(const std::string &anAddress){
                replyTo=anAddress;return *this;
            }
        private:
            ZMQConnector &connector;
            ZMQPublisher publisher;
            std::string topic;
            std::string replyTo;
            std::vector<std::string> bindings_used;
        };

    } // scf
} // zmq

#endif //COMMUNICATION_ZMQPUBLISHER_H
