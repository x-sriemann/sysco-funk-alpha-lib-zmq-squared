//
// Created by developer on 13.02.23.
//

#ifndef COMMUNICATION_ZMQIPCURI_H
#define COMMUNICATION_ZMQIPCURI_H
#include "../ZMQBaseUri.h"

namespace scf {
    namespace zmq{
        class ZMQIpcUri :public ZMQBaseUri{
        public:
            explicit ZMQIpcUri(const std::string &aSocketName)
                    : ZMQBaseUri("ipc://"),socketName(aSocketName){
                uri=getBaseUri()+aSocketName+".ipc";
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
#endif //COMMUNICATION_ZMQIPCURI_H
