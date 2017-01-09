#include "LocalResourcesScreen.h"
#include <iostream>
#include <ncurses.h>

LocalResourcesScreen::LocalResourcesScreen(std::string name, Kernel *k) : Screen(name, k)
{
	choisePos = 0;
	pageNumber = 1;
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	localResources = infoProvider.getLocalResources();
}

void LocalResourcesScreen::drawScreen()
{
	printw( "<- Q-powrót\tF5-odswiez\n" );
	printw( "\nLokalne zasoby: %d [Strona %d/%d]\n", localResources.size(),
			  pageNumber, localResources.size()/PAGE_SIZE+1 );
	printw( "%12s%20s%12s%20s\n", "Zablokowany:", "Nazwa:", "Rozmiar:", "Pobrany procent:" );
	std::vector<OutLocalResource>::iterator resEnd = (pageNumber * PAGE_SIZE < localResources.size() ? localResources.begin() + (pageNumber-1)*PAGE_SIZE + PAGE_SIZE : localResources.end());  
	int i = 0;	
	for(std::vector<OutLocalResource>::iterator it = localResources.begin()+(pageNumber-1)*PAGE_SIZE;
		 it != resEnd; ++it)
	{
		std::string resourceName = StringHelp::toUtf8((*it).resource.getResourceName());
		if(i == choisePos) attron( A_UNDERLINE);
		printw( "%12s%20.20s%12d\t[%d/100%]\n", (*it).resource.isBlocked() ? "true" : "false",
													resourceName.c_str(),
													(*it).resource.getResourceSize(),												      
													(*it).percentDownloaded);
		attroff( A_UNDERLINE);
		i++;
	}
}

std::string LocalResourcesScreen::inputHandle() 
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
			ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
			infoProvider.changeResourceBlockState(localResources.at(choisePos).resource);
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
			localResources = infoProvider.getLocalResources();
			break;
		}
		case 113:	// KEY_Q
			return "main_menu";
	}
	return "";
}