//
// Created by defacto on 07.01.17.
//

#include "ActionThread.h"

ActionThread::ActionThread(const std::function<void()> &funcToRunInLoop, unsigned secondsBetweenLoops, std::string threadName ) :
		ActionQueue(this ,Help::Str("AC",threadName)), funcToRunInLoop(funcToRunInLoop), secondsBetweenLoops(secondsBetweenLoops) {}
