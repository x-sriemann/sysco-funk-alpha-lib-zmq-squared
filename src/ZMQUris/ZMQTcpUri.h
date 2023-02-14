//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//

#ifndef COMMUNICATION_ZMQTCPURI_H
#define COMMUNICATION_ZMQTCPURI_H
#include "../ZMQBaseUri.h"

namespace scf {
    namespace zmq{
        class ZMQTcpUri :public ZMQBaseUri{
        public:
            explicit ZMQTcpUri(const std::string &aPort)
            : ZMQBaseUri("tcp://*:"),port(aPort){
                uri=getBaseUri()+port;
            }
            const std::string &getUri() override{
                return uri;
            }
        private:
            const std::string port;
            std::string uri;
        };
    }
}
#endif //COMMUNICATION_ZMQTCPURI_H
