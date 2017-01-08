//
// Created by defacto on 11.12.16.
//

#ifndef TINTORRENT_MULTITHREADINGSEQUENCE_H
#define TINTORRENT_MULTITHREADINGSEQUENCE_H

#include <map>
#include "BlockingElement.h"

class MultithreadingSequence{
	std::map< std::string, std::shared_ptr<BlockingElement>> blockingElements;
public:
	std::shared_ptr<BlockingElement> operator[]( std::string elementName );

	void initBlockingElement(std::string elementName);
};


#endif //TINTORRENT_MULTITHREADINGSEQUENCE_H
