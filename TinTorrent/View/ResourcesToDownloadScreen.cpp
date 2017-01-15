#include "ResourcesToDownloadScreen.h"
#include <iostream>
#include <ncurses.h>

ResourcesToDownloadScreen::ResourcesToDownloadScreen(std::string name, Kernel *k) : Screen(name, k)
{
	choisePos = 0;
	pageNumber = 1;
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	remoteResources = infoProvider.getResourcesThatCanBeDownloaded();
}

void ResourcesToDownloadScreen::drawScreen()
{
	printw( "<- Q-powrót\tF5-odswiez\n" );
	printw( "\nZasoby do pobrania: %d [Strona %d/%d]\n", remoteResources.size(),
			  pageNumber, remoteResources.size()/PAGE_SIZE+1 );
	printw( "%20s%20s%20s\n", "Nazwa:", "Segmenty:", "Rozmiar:" );
	std::vector<Resource>::iterator resEnd = (pageNumber * PAGE_SIZE < remoteResources.size() ? remoteResources.begin() + (pageNumber-1)*PAGE_SIZE + PAGE_SIZE : remoteResources.end());  
	int i = 0;	
	for(std::vector<Resource>::iterator it = remoteResources.begin()+(pageNumber-1)*PAGE_SIZE;
		 it != resEnd; ++it)
	{
		std::string resourceName = StringHelp::toUtf8((*it).getResourceName());
		if(i == choisePos) attron( A_UNDERLINE);
		printw( "%20.20s%20d%20d\n", resourceName.c_str(),
															(*it).getSegmentCount(),
															(*it).getResourceSize());
		attroff( A_UNDERLINE);
		i++;
	}
}

std::string ResourcesToDownloadScreen::inputHandle() 
{
	int input = getch();
	switch(input) {
		case KEY_DOWN:
			if((unsigned)++choisePos >= remoteResources.size()) choisePos = 0;
			break;
		case KEY_UP:
			choisePos--;
			if(choisePos < 0) choisePos = remoteResources.size()-1;
			break;
		case 10:	// ENTER
		{
			ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
			infoProvider.orderResourceDownload(remoteResources[choisePos]);
			break;
		}
		case KEY_RIGHT:
			if(pageNumber < remoteResources.size()/PAGE_SIZE+1) pageNumber++;
			break;
		case KEY_LEFT:
			if(pageNumber > 1) pageNumber--;
			break;
		case KEY_F(5):
		{
			choisePos = 0;
			ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
			remoteResources = infoProvider.getResourcesThatCanBeDownloaded();
			break;
		}
		case 113:	// KEY_Q
			return "main_menu";
	}
	return "";
}