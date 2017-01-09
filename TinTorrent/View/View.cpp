#include <View/View.h>
#include <View/MainMenuScreen.h>
#include <View/LocalResourcesScreen.h>
#include <View/OtherClientsScreen.h>
#include <View/StatusScreen.h>
#include <iostream>
#include <ncurses.h>

View::View(Kernel *k) : kernel(k)
{	
	initscr();
	keypad( stdscr, TRUE );
	noecho();

	MainMenuScreen *screen1 = new MainMenuScreen("main_menu", kernel);
	screens.push_back(screen1);
	LocalResourcesScreen *screen2 = new LocalResourcesScreen("local_resources", kernel);
	screens.push_back(screen2);
	OtherClientsScreen *screen3 = new OtherClientsScreen("other_clients", kernel);
	screens.push_back(screen3);
	StatusScreen *screen4 = new StatusScreen("status", kernel);
	screens.push_back(screen4);
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
			std::string empty = "";			
			
			if(submit.compare("exit") == 0) {
				// TODO stop kernel or not?
				break;
			}

			if(submit.compare(empty) != 0)
			{
				changeActiveScreen(submit);
			}
			sleep(0.1);
			
		}
		else
		{
			std::cout << "No active screen, abort!\n";
			break;
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