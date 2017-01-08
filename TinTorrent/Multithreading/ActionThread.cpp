//
// Created by defacto on 07.01.17.
//

#include "ActionThread.h"

ActionThread::ActionThread(const std::function<void()> &funcToRunInLoop, unsigned secondsBetweenLoops ) :
		ActionQueue(this), funcToRunInLoop(funcToRunInLoop), secondsBetweenLoops(secondsBetweenLoops) {}
