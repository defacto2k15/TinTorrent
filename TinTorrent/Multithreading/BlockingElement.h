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
	void unlock(){
		{
			std::lock_guard<std::mutex> lk(m);
			ready = true;
		}
		cv.notify_one();
	}

	void unlockAfter(unsigned int miliseconds ){
		if( unlockAfterActive ){
			throw std::logic_error("UnlockAfter was arleady caleld");
		}
		unlockAfterActive = true;

		unlockingThread = std::make_shared<std::thread>( [this, miliseconds](){
			std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
			unlock();
		});
	}

	void waitForUnlock(){
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [this] { return ready; });
	}

	~BlockingElement(){
		if( unlockingThread){
			unlockingThread->join();
		}
	}

};

#endif //TINTORRENT_BLOCKINGELEMENT_H
