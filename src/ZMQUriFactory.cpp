//
// Created by developer on 13.02.23.
//

#include "ZMQUriFactory.h"


namespace scf{
    namespace zmq {

        int ZMQUriFactory::counter=0;
        ZMQUriFactory::ZMQUriFactory()
        {

        }
        std::unique_ptr<ZMQBaseUri> ZMQUriFactory::buildUri(const ZMQUriPair &aUriPair)
        {
            switch(aUriPair.first){
                case ZMQUriType::INPROC:{
                    if(aUriPair.second.empty()){
                        return nullptr;
                    }
                    return  std::move(std::unique_ptr<ZMQBaseUri>(new ZMQInProcUri(aUriPair.second)));
                }
                case ZMQUriType::IPC:{
                    if(aUriPair.second.empty()){
                        return nullptr;
                    }
                    return std::move(std::unique_ptr<ZMQBaseUri>( new ZMQIpcUri(aUriPair.second)));
                }
                case ZMQUriType::TCP:
                default:{
                    auto port=aUriPair.second;
                    if(port.empty()){
                        port=std::to_string(5555 + (++counter));
                    }
                    return std::move(std::unique_ptr<ZMQBaseUri>( new ZMQTcpUri(port)));
                }
            };
        }

    }
}