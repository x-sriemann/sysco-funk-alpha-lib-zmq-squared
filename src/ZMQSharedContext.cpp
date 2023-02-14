//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//
#include "ZMQSharedContext.h"

namespace scf{
    namespace zmq{

        void* ZMQSharedContext::sharedContext=nullptr;
        ZMQSharedContext SharedContext{};
    }
}