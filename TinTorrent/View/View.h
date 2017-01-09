//
// Created by Ransho 08.01.2017
//

#include <Kernel/Kernel.h>
#include <list>

class Screen;

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