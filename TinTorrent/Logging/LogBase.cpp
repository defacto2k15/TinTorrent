//
// Created by defacto on 08.01.17.
//

#include "LogBase.h"

LogBase::LogBase(std::string prefix) : prefix(prefix){
}

void LogBase::initializeLogger() {
	if( !logger){
		logger = spd::get("console");
	}
}
