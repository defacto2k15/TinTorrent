//
// Created by defacto on 11.12.16.
//

#include "BlockingElement.h"

void BlockingElement::unlock() {
	{
		std::lock_guard<std::mutex> lk(m);
		ready = true;
	}
	cv.notify_one();
}

void BlockingElement::unlockAfter(unsigned int miliseconds) {
	if( unlockAfterActive ){
		throw std::logic_error("UnlockAfter was arleady caleld");
	}
	unlockAfterActive = true;

	unlockingThread = std::make_shared<std::thread>( [this, miliseconds](){
		std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
		unlock();
	});
}

void BlockingElement::waitForUnlock() {
	std::unique_lock<std::mutex> lk(m);
	cv.wait(lk, [this] { return ready; });
}

BlockingElement::~BlockingElement() {
	if( unlockingThread){
		unlockingThread->join();
	}
}
