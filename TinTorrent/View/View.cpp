#include <View/View.h>
#include <View/MainMenuScreen.h>
#include <View/LocalResourcesScreen.h>
#include <View/OtherClientsScreen.h>
#include <View/ResourcesToDownloadScreen.h>
#include <View/StatusScreen.h>
#include <View/StoppedResourcesScreen.h>
#include <iostream>
#include <ncurses.h>

View::View(Kernel *k) : kernel(k)
{	
	// ncurses stuff
	initscr();
	start_color();
	keypad( stdscr, TRUE );
	noecho();

	// colors
	init_color(COLOR_RED, 1000, 750, 0);
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
   init_pair(3, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(3));

	// screens
	MainMenuScreen *screen1 = new MainMenuScreen("main_menu", kernel);
	screens.push_back(screen1);
	LocalResourcesScreen *screen2 = new LocalResourcesScreen("local_resources", kernel);
	screens.push_back(screen2);
	OtherClientsScreen *screen3 = new OtherClientsScreen("other_clients", kernel);
	screens.push_back(screen3);
	StatusScreen *screen4 = new StatusScreen("status", kernel);
	screens.push_back(screen4);
	ResourcesToDownloadScreen *screen5 = new ResourcesToDownloadScreen("remote_resources", kernel);
	screens.push_back(screen5);
	StoppedResourcesScreen *screen6 = new StoppedResourcesScreen("stopped_resources", kernel);
	screens.push_back(screen6);
	activeScreen = screens.front();
}

View::~View()
{
	endwin();
}

void View::start()
{
	int cycles = 0;
	while(1)
	{
		if(activeScreen != nullptr)
		{
			clear();
			if ( ++cycles > CYCLES_TO_REFRESH )
			{
				activeScreen->refresh();
				cycles = 0;
			}
			activeScreen->drawScreen();
			fflush(stdout);
			std::string submit = activeScreen->inputHandle();
			std::string empty = "";			
			
			if(submit.compare("exit") == 0) {
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