#include "StoppedResourcesScreen.h"
#include <iostream>
#include <ncurses.h>

// (*it).resource.isBlocked() ? "true" : "false",

StoppedResourcesScreen::StoppedResourcesScreen(std::string name, Kernel *k) : Screen(name, k)
{
	choisePos = 0;
	pageNumber = 1;
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	localResources = infoProvider.getResourcesThatCanBeAnnounced();
}

void StoppedResourcesScreen::drawScreen()
{
	printw( "<- Q-powrót\tF5-odswiez\n" );
	printw( "Udostepniane zasoby: %d [Strona %d/%d]\n", localResources.size(),
			  pageNumber, localResources.size()/PAGE_SIZE+1 );
	printw( "%20s%12s%20s\n", "Nazwa:", "Rozmiar:", "Pobrany procent:" );
	std::vector<Resource>::iterator resEnd = (pageNumber * PAGE_SIZE < localResources.size() ? localResources.begin() + (pageNumber-1)*PAGE_SIZE + PAGE_SIZE : localResources.end());  
	int i = 0;	
	for(std::vector<Resource>::iterator it = localResources.begin()+(pageNumber-1)*PAGE_SIZE;
		 it != resEnd; ++it)
	{
		std::string resourceName = StringHelp::toUtf8((*it).getResourceName());
		if(i == choisePos) attron( A_UNDERLINE);
		printw( "%20.20s%12d\t[/100%]\n", resourceName.c_str(),
														(*it).getResourceSize());
		attroff( A_UNDERLINE);
		i++;
	}
}

std::string StoppedResourcesScreen::inputHandle() 
{
	int input = getch();
	switch(input) {
		case KEY_DOWN:
			if((unsigned)++choisePos >= localResources.size()) choisePos = 0;
			break;
		case KEY_UP:
			choisePos--;
			if(choisePos < 0) choisePos = localResources.size()-1;
			break;
		case 10:	// ENTER
		{
			if(localResources.size() > 0)
			{	
				ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
				infoProvider.changeResourceAnnouncementState(localResources.at(choisePos));
				localResources = infoProvider.getResourcesThatCanBeAnnounced();
			}
			break;
		}
		case KEY_RIGHT:
			if(pageNumber < localResources.size()/PAGE_SIZE+1) pageNumber++;
			break;
		case KEY_LEFT:
			if(pageNumber > 1) pageNumber--;
			break;
		case KEY_F(5):
		{
			choisePos = 0;
			ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
			localResources = infoProvider.getResourcesThatCanBeAnnounced();
			break;
		}
		case 113:	// KEY_Q
			return "main_menu";
	}
	return "";
}