//
// Created by Ransho 08.01.2017
//

#pragma once
#include <View/Screen.h>
#include <Kernel/Kernel.h>
#include <list>

#define CYCLES_TO_REFRESH 10

class View {
private:
	Kernel *kernel;
	std::list<Screen*> screens;
	Screen* activeScreen;
public:
	View(Kernel*);
	~View();
	void start();
	void drawActiveScreen();
	void changeActiveScreen(std::string screenName);
};