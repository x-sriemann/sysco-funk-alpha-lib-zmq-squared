//
// Created by developer on 13.02.23.
//

#include "ZMQMessage.h"
#include <cstring>

namespace scf {
    namespace zmq {
        ZMQMessage::ZMQMessage(const std::string &aValue){
            size=aValue.size();
            if(!aValue.empty()){
                auto err=strncpy(value,aValue.c_str(),sizeof(value)-1);
            }
            if(size>(sizeof(value)-1))
            {
                size=sizeof(value)-1;
            }
        }
    } // scf
} // zmq