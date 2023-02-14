//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//

#ifndef COMMUNICATION_ZMQINPROCURI_H
#define COMMUNICATION_ZMQINPROCURI_H
#include "../ZMQBaseUri.h"

namespace scf {
    namespace zmq{
        class ZMQInProcUri :public ZMQBaseUri{
        public:
            explicit ZMQInProcUri(const std::string &aSocketName)
                    : ZMQBaseUri("inproc://"),socketName(aSocketName){
                uri=getBaseUri()+aSocketName;
            }
            const std::string &getUri() override{
                return uri;
            }
        private:
            const std::string socketName;
            std::string uri;
        };
    }
}
#endif //COMMUNICATION_ZMQINPROCURI_H
