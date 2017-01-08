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

#define UNUSED(x) (void)(x)

template<typename T>
class ActionQueue {
	std::deque<std::function<void(T& )> > actionQueue;
	std::mutex mutex;
	std::condition_variable notEmptyActionQueue;
	T *castedThis;
	std::thread workingThread;
protected:
	ActionQueue( T* castedThis ) : castedThis(castedThis){
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
			} catch( std::exception e){
				std::cout << "ActionQueue caught exception " << e.what() << std::endl;
			}
			std::cout << "ActionQueue terminates thread " << std::endl;
		});
	}

	void join(){
		internalJoin();
		if( workingThread.joinable() ){
			workingThread.join();
		}
	}

	void killYourself(){
		std::cout << "ActionQueue: got kill yourself request" << std::endl;
		threadShouldRun = false;
		clearQueue();
		internalKillYourself();

		add( [](T &UNUSED_VAR){ UNUSED(UNUSED_VAR);});
	}

	virtual ~ActionQueue(){
		std::cout <<"ActionQueue: Destructor called " << std::endl;
		if(threadShouldRun){
			std::cout <<"ActionQueue: threadShouldRun is still true. Should have told me to kill myself. Ending myself " << std::endl;
		}
		killYourself();
		join();
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
