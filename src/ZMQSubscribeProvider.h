//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//
#ifndef COMMUNICATION_ZMQSUBSCRIBEPROVIDER_H
#define COMMUNICATION_ZMQSUBSCRIBEPROVIDER_H

#include "ZMQHelpers.h"
#include "ZMQConnector.h"
#include "ZMQBaseUri.h"
#include "ZMQUriFactory.h"
#include "ZMQMessage.h"

#include <vector>
#include <string>
#include <memory>

namespace scf{
    namespace zmq{
        class ZMQSubscribeProvider {
        public:
            ZMQSubscribeProvider(ZMQConnector &aConnector, std::unique_ptr<ZMQBaseUri> uri, const std::string &defaultTopic="default");
            ~ZMQSubscribeProvider();
            bool receive();

            const std::string &getReplyTo() const;
            const ZMQMessage &getMessage() const;
            ZMQSubscribeProvider & setTopic(const std::string &aTopic);

        private:
            std::string topic;
            std::string replyTo;
            ZMQMessage message;
            ZMQSubscriber subscriber;
        };
    }
}


#endif //COMMUNICATION_ZMQSUBSCRIBEPROVIDER_H
