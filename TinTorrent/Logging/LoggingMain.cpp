//
// Created by defacto on 08.01.17.
//

#include "LoggingMain.h"

void LoggingMain::init() {
	if( Constants::advancedLoggingActive) {
		spd::stdout_color_mt("console");
		spd::set_level(spd::level::debug); //Set global log level to info
	}
}

void LoggingMain::closeLogger() {
	if( Constants::advancedLoggingActive) {
		spdlog::drop_all();
	}
}

LoggingMain::~LoggingMain() {
	closeLogger();
}
