//
// Created by defacto on 07.01.17.
//

#include "ActionThread.h"

ActionThread::ActionThread(const std::function<void()> &funcToRunInLoop, unsigned secondsBetweenLoops, std::string threadName ) :
		ActionQueue(this ,Help::Str("AC",threadName)), funcToRunInLoop(funcToRunInLoop), secondsBetweenLoops(secondsBetweenLoops) {}

void ActionThread::runOneLoop() {
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

void ActionThread::start() {
	add( [](ActionThread &t ){
		t.runOneLoop();
	});
	startThread();
}

void ActionThread::internalKillYourself() {
	waitingConditionVariable.notify_all();
}
