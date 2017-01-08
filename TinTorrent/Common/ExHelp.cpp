//
// Created by defacto on 09.12.16.
//

#include "ExHelp.h"

void ExHelp::writeException(std::exception &e, std::string method) {
	std::cerr << "Exception Caught: "<<method<<": "<<e.what()<<std::endl;
}
