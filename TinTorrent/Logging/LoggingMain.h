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
	void init();

	void closeLogger();

	~LoggingMain();
};


#endif //TINTORRENT_LOGGINGMAIN_H
