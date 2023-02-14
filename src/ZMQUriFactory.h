//
// Copyright (c) <2023> Vaillant Group International GmbH. All rights reserved.
//

#ifndef COMMUNICATION_ZMQURIFACTORY_H
#define COMMUNICATION_ZMQURIFACTORY_H

#include "ZMQUris/ZMQInProcUri.h"
#include "ZMQUris/ZMQIpcUri.h"
#include "ZMQUris/ZMQTcpUri.h"
#include <memory>
#include <vector>

namespace scf{
    namespace zmq{

        enum struct ZMQUriType{
            TCP,
            IPC,
            INPROC
        };

        using ZMQBindings=std::vector<std::unique_ptr<ZMQBaseUri> >;
        using ZMQUriPair=std::pair<ZMQUriType, const std::string>;

        class ZMQUriFactory {
        public:
            ZMQUriFactory();
            void createBindings(ZMQBindings & bindings, std::vector<ZMQUriPair> pairs){
                for(auto x:pairs)
                {
                    bindings.push_back(std::move(buildUri(x)));
                }
            }
            /**
             * This should look like that:
             * ZMQBindings bindings;
             * ZMQUriFactory.createBindings(bindings,
             * {{IPC,"test"},{INPROC,"test2"},{TCP,""}, {TCP,"5556"}});
             * This will build an create a vector of Uri-Objects
             * which can be processed later.
             */
            std::unique_ptr<ZMQBaseUri>  buildUri(const ZMQUriPair &aUriPair);
        private:
            static int counter;
        };

    }
}
#endif //COMMUNICATION_ZMQURIFACTORY_H
