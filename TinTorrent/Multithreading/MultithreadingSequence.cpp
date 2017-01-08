//
// Created by defacto on 11.12.16.
//

#include "MultithreadingSequence.h"

std::shared_ptr<BlockingElement> MultithreadingSequence::operator[](std::string elementName) {
	if( !blockingElements.count(elementName)){
		throw std::invalid_argument(std::string("There is no element of name ")+elementName);
	}
	return blockingElements[elementName];
}

void MultithreadingSequence::initBlockingElement(std::string elementName) {
	if( blockingElements.count(elementName)!= 0){
		throw std::invalid_argument(std::string("There is element of name ")+elementName);
	}
	blockingElements[elementName] = std::make_shared<BlockingElement>();
}
