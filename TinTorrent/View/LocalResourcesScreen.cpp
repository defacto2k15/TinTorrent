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
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	std::vector<Resource> v =  infoProvider.getResourcesThatCanBeAnnounced();

	printw( "<- Q-powrót | Strzalki - wybor zasobu/strony | F5 - reczne odswiezenie\n" );
	printw( "   ENTER - zakazanie pobierania wybranego zasobu\n" );
	printw( "\n\nLokalne zasoby: %d [Strona %d/%d]\n", localResources.size(),
			  pageNumber, localResources.size()/PAGE_SIZE+1 );
	printw( "%20s%12s%20s\n", "Nazwa:", "Rozmiar:", "Zakazany:");
	std::vector<OutLocalResource>::iterator resEnd = (pageNumber * PAGE_SIZE < localResources.size() ? localResources.begin() + (pageNumber-1)*PAGE_SIZE + PAGE_SIZE : localResources.end());  
	int i = 0;	
	for(std::vector<OutLocalResource>::iterator it = localResources.begin()+(pageNumber-1)*PAGE_SIZE;
		 it != resEnd; ++it)
	{
		std::string resourceName = StringHelp::toUtf8((*it).resource.getResourceName());
		if(i == choisePos) { attron( A_UNDERLINE); attron(COLOR_PAIR(2)); }
		printw( "%20.20s%12d%20s\n", resourceName.c_str(), 
										 	  (*it).resource.getResourceSize(),
										 	  (std::find(v.begin(), v.end(), (*it).resource) != v.end()) ? "true" : "false");
		attroff( A_UNDERLINE); attron(COLOR_PAIR(3));
		i++;
	}
}

std::string LocalResourcesScreen::inputHandle() 
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
			ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
			infoProvider.changeResourceAnnouncementState(localResources.at(pageNumber*PAGE_SIZE-PAGE_SIZE+choisePos).resource);
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

void LocalResourcesScreen::refresh()
{
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	localResources = infoProvider.getLocalResources();
	if((unsigned)choisePos > (pageNumber*PAGE_SIZE < localResources.size() ? PAGE_SIZE : localResources.size()-(pageNumber-1)*PAGE_SIZE)) choisePos = (pageNumber*PAGE_SIZE < localResources.size() ? PAGE_SIZE-1 : localResources.size()-(pageNumber-1)*PAGE_SIZE-1);
}