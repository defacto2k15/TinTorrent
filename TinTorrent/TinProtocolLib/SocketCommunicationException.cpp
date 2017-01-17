//
// Created by defacto on 07.01.17.
//

#include "SocketCommunicationException.h"

SocketCommunicationException::SocketCommunicationException(std::string message) : std::runtime_error(message){
}

SocketCommunicationException::SocketCommunicationException(std::exception &e) : std::runtime_error(Help::Str("Caught exception ",e.what())){
}
