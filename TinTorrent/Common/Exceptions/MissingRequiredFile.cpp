//
// Created by defacto on 27.12.16.
//

#include "MissingRequiredFile.h"

MissingRequiredFile::MissingRequiredFile(std::string message) : std::runtime_error(message){
}
