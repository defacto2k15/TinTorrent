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
	std::shared_ptr<BlockingElement> operator[]( std::string elementName ){
		if( !blockingElements.count(elementName)){
			throw std::invalid_argument(std::string("There is no element of name ")+elementName);
		}
		return blockingElements[elementName];
	}

	void initBlockingElement(std::string elementName){
		if( blockingElements.count(elementName)!= 0){
			throw std::invalid_argument(std::string("There is element of name ")+elementName);
		}
		blockingElements[elementName] = std::make_shared<BlockingElement>();
	}
};


#endif //TINTORRENT_MULTITHREADINGSEQUENCE_H
