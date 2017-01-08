//
// Created by defacto on 09.12.16.
//

#ifndef TINTORRENT_EXHELP_H
#define TINTORRENT_EXHELP_H

#include <string>
#include <exception>
#include <iostream>
class ExHelp {
public:
	static void writeException( std::exception &e, std::string method);
};


#endif //TINTORRENT_EXHELP_H
