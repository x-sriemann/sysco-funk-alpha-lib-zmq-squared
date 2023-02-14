//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//

#ifndef COMMUNICATION_ZMQBASEURI_H
#define COMMUNICATION_ZMQBASEURI_H
#include <string>

namespace scf {
    namespace zmq {

        class ZMQBaseUri {
        public:
            explicit ZMQBaseUri(const std::string &uri);
            virtual ~ZMQBaseUri() =default ;
            const std::string &getBaseUri(){return baseUri;}
            virtual const std::string &getUri()=0;
        protected:
            const std::string baseUri;
        };

    } // scf
} // zmq

#endif //COMMUNICATION_ZMQBASEURI_H
