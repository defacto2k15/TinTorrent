//
// Created by defacto on 11.12.16.
//

#ifndef TINTORRENT_BLOCKINGELEMENT_H
#define TINTORRENT_BLOCKINGELEMENT_H

#include <mutex>
#include <condition_variable>
#include <thread>

class BlockingElement{
	bool ready = false;
	std::mutex m;
	std::condition_variable cv;

	bool unlockAfterActive = false;
	std::shared_ptr<std::thread> unlockingThread;
public:
	void unlock();

	void unlockAfter(unsigned int miliseconds );

	void waitForUnlock();

	~BlockingElement();

};

#endif //TINTORRENT_BLOCKINGELEMENT_H
