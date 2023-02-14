//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//

#include "ZMQBaseContext.h"

namespace scf{
    namespace zmq {
        ZMQBaseContext::ZMQBaseContext(ZMQContextType aType)
        :type(aType){}
    }
}