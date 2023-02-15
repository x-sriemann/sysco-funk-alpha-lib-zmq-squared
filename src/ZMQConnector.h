//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//

#ifndef SYSCO_FUNK_ALPHA_LIB_ZMQ_SQUARED_ZMQCONNECTOR_H
#define SYSCO_FUNK_ALPHA_LIB_ZMQ_SQUARED_ZMQCONNECTOR_H
#include <string>
#include "ZMQSharedContext.h"

namespace scf {
    namespace zmq {

        class ZMQConnector {
        public:
             explicit ZMQConnector(ZMQSharedContext &aContext);
             ZMQConnector();

             virtual ~ZMQConnector();
             ZMQContext getContext(){
                 return context.getContext();
             }
             bool isSharedConnection(){
                 return context.isZMQType(ZMQContextType::SHARED);
             }
        private:
            ZMQBaseContext &context;

        };

    } // scf
} // zmq

#endif //SYSCO_FUNK_ALPHA_LIB_ZMQ_SQUARED_ZMQCONNECTOR_H
