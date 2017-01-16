#include "StoppedResourcesScreen.h"
#include <iostream>
#include <ncurses.h>

StoppedResourcesScreen::StoppedResourcesScreen(std::string name, Kernel *k) : Screen(name, k)
{
	choisePos = 0;
	pageNumber = 1;
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	localResources = infoProvider.getResourcesThatCanBeAnnounced();
}

void StoppedResourcesScreen::drawScreen()
{
	printw( "<- Q-powrót | Strzalki - wybor zasobu/strony\n" );
	printw( "   ENTER - usuniecie z listy zakazanych\n" );
	printw( "Zakazane zasoby: %d [Strona %d/%d]\n", localResources.size(),
			  pageNumber, localResources.size()/PAGE_SIZE+1 );
	printw( "%20s%12s\n", "Nazwa:", "Rozmiar:" );
	std::vector<Resource>::iterator resEnd = (pageNumber * PAGE_SIZE < localResources.size() ? localResources.begin() + (pageNumber-1)*PAGE_SIZE + PAGE_SIZE : localResources.end());  
	int i = 0;	
	for(std::vector<Resource>::iterator it = localResources.begin()+(pageNumber-1)*PAGE_SIZE;
		 it != resEnd; ++it)
	{
		std::string resourceName = StringHelp::toUtf8((*it).getResourceName());
		if(i == choisePos) attron( A_UNDERLINE);
		printw( "%20.20s%12d\n", resourceName.c_str(),
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
			if((unsigned)++choisePos >= (pageNumber*PAGE_SIZE < localResources.size() ? PAGE_SIZE : localResources.size()-(pageNumber-1)*PAGE_SIZE)) choisePos = 0;
			break;
		case KEY_UP:
			choisePos--;
			if(choisePos < 0) choisePos = (pageNumber*PAGE_SIZE < localResources.size() ? PAGE_SIZE-1 : localResources.size()-(pageNumber-1)*PAGE_SIZE-1);
			break;
		case 10:	// ENTER
		{
			if(localResources.size() > 0)
			{	
				ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
				infoProvider.changeResourceAnnouncementState(localResources.at(pageNumber*PAGE_SIZE-PAGE_SIZE+choisePos));
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

void StoppedResourcesScreen::refresh()
{
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	localResources = infoProvider.getResourcesThatCanBeAnnounced();
	if((unsigned)choisePos > (pageNumber*PAGE_SIZE < localResources.size() ? PAGE_SIZE : localResources.size()-(pageNumber-1)*PAGE_SIZE)) choisePos = (pageNumber*PAGE_SIZE < localResources.size() ? PAGE_SIZE-1 : localResources.size()-(pageNumber-1)*PAGE_SIZE-1);
}