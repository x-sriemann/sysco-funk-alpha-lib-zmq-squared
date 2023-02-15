//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//

#include "ZMQConnector.h"
#include "ZMQHelpers.h"
#include "ZMQInstancedContext.h"

namespace scf {
    namespace zmq {

        ZMQConnector::ZMQConnector(ZMQSharedContext &aContext)
        :context(aContext)
        {}

        ZMQConnector::ZMQConnector()
        :context(*(new ZMQInstancedContext()))
        {}

        ZMQConnector::~ZMQConnector() {
            if(context.isZMQType(ZMQContextType::INSTANCE)){
                delete &context;
            }
        }

    } // scf
} // zmq