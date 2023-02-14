//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//

#include "ZMQConnector.h"
#include "ZMQHelpers.h"


namespace scf {
    namespace zmq {

        ZMQConnector::ZMQConnector(ZMQSharedContext &aContext)
        :context(aContext)
        {}

    } // scf
} // zmq