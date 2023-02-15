//
// Created by developer on 15.02.23.
//

#ifndef COMMUNICATION_ZMQINSTANCEDCONTEXT_H
#define COMMUNICATION_ZMQINSTANCEDCONTEXT_H
#include "ZMQBaseContext.h"
#include "ZMQHelpers.h"

namespace scf {
    namespace zmq {
        class ZMQInstancedContext : public scf::zmq::ZMQBaseContext {
        public:
            ZMQInstancedContext()
            :ZMQBaseContext(ZMQContextType::INSTANCE),context(zmq_ctx_new()){}
            virtual ~ZMQInstancedContext(){
                if(context!=nullptr){
                    zmq_ctx_destroy(context);
                }
            }
            virtual ZMQContext getContext() final {
                return context;
            }
        private:
            ZMQContext context;
        };
    }
}


#endif //COMMUNICATION_ZMQINSTANCEDCONTEXT_H
