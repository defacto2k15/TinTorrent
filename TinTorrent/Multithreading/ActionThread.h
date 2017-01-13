//
// Created by defacto on 07.01.17.
//

#ifndef TINTORRENT_ACTIONTHREAD_H
#define TINTORRENT_ACTIONTHREAD_H


#include <functional>
#include "ActionQueue.h"
#include <chrono>

class ActionThread : public ActionQueue<ActionThread>{
	std::function< void()> funcToRunInLoop;
	unsigned secondsBetweenLoops;
	std::mutex mutex;
	std::condition_variable  waitingConditionVariable;
public:
	ActionThread(const std::function<void()> &funcToRunInLoop, unsigned secondsBetweenLoops, std::string threadName );

	void runOneLoop(){
		std::unique_lock<std::mutex> lk(mutex);
		funcToRunInLoop();
		bool wasTimeouted = !waitingConditionVariable.wait_for(lk, std::chrono::milliseconds(secondsBetweenLoops * 1000),
		                                                       [this]{return !threadShouldRun;});
		if( wasTimeouted ){
			add( [](ActionThread &t ){
				t.runOneLoop();
			});
		}
	}

	void start(){
		add( [](ActionThread &t ){
			t.runOneLoop();
		});
		startThread();
	}
protected:
	virtual void internalKillYourself(){
		waitingConditionVariable.notify_all();
	}
};


#endif //TINTORRENT_ACTIONTHREAD_H
