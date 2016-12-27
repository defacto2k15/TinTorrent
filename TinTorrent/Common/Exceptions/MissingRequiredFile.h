//
// Created by defacto on 27.12.16.
//

#ifndef TINTORRENT_MISSINGREQUIREDFILE_H
#define TINTORRENT_MISSINGREQUIREDFILE_H


#include <stdexcept>

class MissingRequiredFile : std::runtime_error {
public:
	MissingRequiredFile(std::string message ) : std::runtime_error(message){
	}
};


#endif //TINTORRENT_MISSINGREQUIREDFILE_H
