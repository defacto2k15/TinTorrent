//
// Created by Ransho 08.01.2017
//
#pragma once
#include <Kernel/Kernel.h>

class Screen {
private:
	std::string screenName;
protected:
	Kernel *kernel;
public:
	Screen(std::string name, Kernel *k) : screenName(name), kernel(k) {}
	std::string getScreenName() { return screenName; }

	virtual void drawScreen()=0;
	virtual std::string inputHandle()=0;
};