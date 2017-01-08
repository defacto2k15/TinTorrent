//
// Created by defacto on 08.01.17.
//

#ifndef TINTORRENT_LOGGINGMAIN_H
#define TINTORRENT_LOGGINGMAIN_H

#include <Common/Constants.h>
#include "spdlog/spdlog.h"

namespace spd = spdlog;

class LoggingMain {
public:
	void init(){
		if( Constants::advancedLoggingActive) {
			spd::stdout_color_mt("console");
			spd::set_level(spd::level::debug); //Set global log level to info
		}
	}

	void closeLogger(){
		if( Constants::advancedLoggingActive) {
			spdlog::drop_all();
		}
	}

	~LoggingMain(){
		closeLogger();
	}
};


#endif //TINTORRENT_LOGGINGMAIN_H
