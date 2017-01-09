#include <View/View.h>
#include <View/MainMenuScreen.h>
#include <iostream>
#include <ncurses.h>

View::View(Kernel *k) : kernel(k)
{	
	initscr();
	keypad( stdscr, TRUE );
	noecho();

	MainMenuScreen *screen1 = new MainMenuScreen("main_menu", kernel);
	screens.push_back(screen1);
	activeScreen = screens.front();
}

View::~View()
{
	endwin();
}

void View::start()
{
	while(1)
	{
		if(activeScreen != nullptr)
		{
			clear();
			activeScreen->drawScreen();
			fflush(stdout);
			std::string submit = activeScreen->inputHandle();
			sleep(0.1);
			// TODO change screen
		}
		else
		{
			std::cout << "No active screen, abort!\n";
			//break;
		}
	}
}

void View::drawActiveScreen()
{
	if(activeScreen != nullptr) activeScreen->drawScreen();
}

void View::changeActiveScreen(std::string screenName)
{
	for(std::list<Screen*>::iterator it = screens.begin(); it != screens.end(); ++it)
	{
		if ((**it).getScreenName().compare(screenName) == 0) {
			activeScreen = &**it;
			break;
		}
	}
}