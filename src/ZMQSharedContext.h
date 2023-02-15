//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//

#ifndef COMMUNICATION_ZMQSHAREDCONTEXT_H
#define COMMUNICATION_ZMQSHAREDCONTEXT_H
#include "ZMQHelpers.h"
#include "ZMQBaseContext.h"

namespace scf {
    namespace zmq {

        class ZMQSharedContext :public ZMQBaseContext {
        public:
            ZMQSharedContext()
            {
                if(sharedContext== nullptr) {
                    sharedContext = zmq_ctx_new();
                }
            }
            virtual ~ZMQSharedContext(){
                if(sharedContext!= nullptr){
                    zmq_ctx_destroy(sharedContext);
                }
            }
            virtual ZMQContext getContext() final {
                return sharedContext;
            }

            static ZMQContext sharedContext;
        };

        extern ZMQSharedContext SharedContext;
    } // scf
} // zmq

#endif //COMMUNICATION_ZMQSHAREDCONTEXT_H
