//
// Created by developer on 13.02.23.
//

#ifndef COMMUNICATION_ZMQMESSAGE_H
#define COMMUNICATION_ZMQMESSAGE_H
#include <string>

namespace scf {
    namespace zmq {

        struct ZMQMessage {
            int size;
            char value[1020];
            bool exhausted;
            explicit ZMQMessage(const std::string &aValue);
            ZMQMessage(): ZMQMessage(""){};
            bool isFull(){return (size>=sizeof(value));}
        };

    } // scf
} // zmq

#endif //COMMUNICATION_ZMQMESSAGE_H
