//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_ACTIONQUEUE_H
#define TINTORRENT_ACTIONQUEUE_H

#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <thread>
#include <iostream>
#include <Logging/LogBase.h>

#define UNUSED(x) (void)(x)

template<typename T>
class ActionQueue  {
	LogBase log;
	std::deque<std::function<void(T& )> > actionQueue;
	std::mutex mutex;
	std::condition_variable notEmptyActionQueue;
	T *castedThis;
	std::thread workingThread;
protected:
	ActionQueue( T* castedThis, std::string ancestorPrefix ) : log(ancestorPrefix+":ActionQueue"), castedThis(castedThis){
	}

	bool threadShouldRun = true;
public:
	void add( std::function<void(T&)> action){
		{
			std::unique_lock<std::mutex> lock(this->mutex);
			actionQueue.push_front(action);
		}
		this->notEmptyActionQueue.notify_one();
	}

	void clearQueue(){
		std::unique_lock<std::mutex> lock(this->mutex);
		actionQueue.clear();
	}

	void performAction( T& consumer){
		std::function<void(T&)> a = pop();
		a(consumer);
	}

	void startThread(){
		threadShouldRun = true;
		workingThread = std::thread([this](){
			try {
				while (threadShouldRun) { //todo create some thread closing
					performAction(*castedThis);
				}
			} catch( std::exception &e){
				std::string descr = e.what();
				log.warn( " caught exception " , descr );
			}
			log.debug(" terminates thread " );
		});
	}

	void join(){
		internalJoin();
		if( workingThread.joinable() ){
			workingThread.join();
		}
	}

	void killYourself(){
		log.debug(" got kill yourself request");
		threadShouldRun = false;
		clearQueue();
		internalKillYourself();

		add( [](T &UNUSED_VAR){ UNUSED(UNUSED_VAR);});
	}

	virtual ~ActionQueue(){
		log.debug(" Destructor called ");
		if(threadShouldRun){
			log.debug(" threadShouldRun is still true. Should have told me to kill myself. Ending myself ");
			killYourself();
		}
	}

protected:
	virtual void internalKillYourself(){
		// do nothing - to be implemented by inheritance;
	}

	virtual void internalJoin(){
		// do nothing - to be implemented by inheritance;
	}

private:
	std::function<void(T&)> pop() {
		std::unique_lock<std::mutex> lock(this->mutex);
		this->notEmptyActionQueue.wait(lock, [=]{ return !this->actionQueue.empty(); });
		auto rc(std::move(this->actionQueue.back()));
		this->actionQueue.pop_back();
		return rc;
	}
};


#endif //TINTORRENT_ACTIONQUEUE_H
