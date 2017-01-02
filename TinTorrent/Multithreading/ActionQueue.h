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

template<typename T>
class ActionQueue {
	std::deque<std::function<void(T& )> > actionQueue;
	std::mutex mutex;
	std::condition_variable notEmptyActionQueue;
	std::thread workingThread;
	T *castedThis;
protected:
	ActionQueue( T* castedThis ) : castedThis(castedThis){
	}
public:
	void add( std::function<void(T&)> action){
		{
			std::unique_lock<std::mutex> lock(this->mutex);
			actionQueue.push_front(action);
		}
		this->notEmptyActionQueue.notify_one();
	}

	void performAction( T& consumer){
		pop()(consumer);
	}

	void startThread(){
		workingThread = std::thread([this](){
			while(true){ //todo create some thread closing
				performAction(*castedThis);
			}
		});
	}

private:
	T pop() {
		std::unique_lock<std::mutex> lock(this->mutex);
		this->notEmptyActionQueue.wait(lock, [=]{ return !this->actionQueue.empty(); });
		T rc(std::move(this->actionQueue.back()));
		this->actionQueue.pop_back();
		return rc;
	}
};


#endif //TINTORRENT_ACTIONQUEUE_H
