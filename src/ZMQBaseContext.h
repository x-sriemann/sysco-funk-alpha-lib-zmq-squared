//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//

#ifndef COMMUNICATION_ZMQBASECONTEXT_H
#define COMMUNICATION_ZMQBASECONTEXT_H
#include "ZMQHelpers.h"

namespace scf{
namespace zmq {
    enum struct ZMQContextType{
        SHARED,
        INSTANCE
    };
    class ZMQBaseContext {
    public:
        ZMQBaseContext(ZMQContextType aType);
        ZMQBaseContext():ZMQBaseContext(ZMQContextType::SHARED){}
        virtual ~ZMQBaseContext()=default;
        virtual void *getContext()=0;

        bool isZMQType(ZMQContextType aType){
            return aType==type;
        }
    private:
        const ZMQContextType type;
    };
}
}
#endif //COMMUNICATION_ZMQBASECONTEXT_H
