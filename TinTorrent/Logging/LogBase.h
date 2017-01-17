//
// Created by defacto on 08.01.17.
//

#ifndef TINTORRENT_LOGBASE_H
#define TINTORRENT_LOGBASE_H

#include <string>
#include <Utils/MyString.h>
#include <Common/Constants.h>
#include "spdlog/spdlog.h"

namespace spd = spdlog;

class LogBase {
	std::string prefix;
	std::shared_ptr<spd::logger> logger;
public:
	LogBase( std::string prefix );

	template <class ... TArgs>
	void debug(TArgs ... args ){
		initializeLogger();
		std::string payload = prefix+": "+Help::Str(args...);
		if( Constants::advancedLoggingActive ){
			logger->debug( payload);
		} else {
			std::cerr << payload << std::endl;
		}

	}

	template <class ... TArgs>
	void warn(TArgs ... args ){
		initializeLogger();
		std::string payload = prefix+": "+Help::Str(args...);
		if( Constants::advancedLoggingActive ){
			logger->warn( payload);
		} else {
			std::cerr << payload << std::endl;
		}
	}

	template <class ... TArgs>
	void info(TArgs ... args ){
		initializeLogger();
		std::string payload = prefix+": "+Help::Str(args...);
		if( Constants::advancedLoggingActive ){
			logger->info( payload);
		} else {
			std::cerr << payload << std::endl;
		}
	}

private:
	void initializeLogger();
};


#endif //TINTORRENT_LOGBASE_H
