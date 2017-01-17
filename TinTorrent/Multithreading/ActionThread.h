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

	void runOneLoop();

	void start();
protected:
	virtual void internalKillYourself();
};


#endif //TINTORRENT_ACTIONTHREAD_H
