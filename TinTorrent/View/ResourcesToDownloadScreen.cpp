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
	printw( "<- Q-powrót\tENTER - rozpoczecie pobierania wybranego zasobu\n" );
	printw( "\nZasoby do pobrania: %d [Strona %d/%d]\n", remoteResources.size(),
			  pageNumber, remoteResources.size()/PAGE_SIZE+1 );
	printw( "%20s%20s%20s\n", "Nazwa:", "Segmenty:", "Rozmiar:" );
	std::vector<Resource>::iterator resEnd = (pageNumber * PAGE_SIZE < remoteResources.size() ? remoteResources.begin() + (pageNumber-1)*PAGE_SIZE + PAGE_SIZE : remoteResources.end());  
	int i = 0;	
	for(std::vector<Resource>::iterator it = remoteResources.begin()+(pageNumber-1)*PAGE_SIZE;
		 it != resEnd; ++it)
	{
		std::string resourceName = StringHelp::toUtf8((*it).getResourceName());
		if(i == choisePos) { attron( A_UNDERLINE); attron(COLOR_PAIR(2)); }
		printw( "%20.20s%20d%20d\n", resourceName.c_str(),
											  (*it).getSegmentCount(),
											  (*it).getResourceSize());
		attroff( A_UNDERLINE); attron(COLOR_PAIR(3));
		i++;
	}
}

std::string ResourcesToDownloadScreen::inputHandle() 
{
	int input = getch();
	switch(input) {
		case KEY_DOWN:
			if((unsigned)++choisePos >= (pageNumber*PAGE_SIZE < remoteResources.size() ? PAGE_SIZE : remoteResources.size()-(pageNumber-1)*PAGE_SIZE)) choisePos = 0;
			break;
		case KEY_UP:
			choisePos--;
			if(choisePos < 0) choisePos = (pageNumber*PAGE_SIZE < remoteResources.size() ? PAGE_SIZE-1 : remoteResources.size()-(pageNumber-1)*PAGE_SIZE-1);
			break;
		case 10:	// ENTER
		{
			ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
			infoProvider.orderResourceDownload(remoteResources[pageNumber*PAGE_SIZE-PAGE_SIZE+choisePos]);
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

void ResourcesToDownloadScreen::refresh()
{
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	remoteResources = infoProvider.getResourcesThatCanBeDownloaded();
	if((unsigned)choisePos > (pageNumber*PAGE_SIZE < remoteResources.size() ? PAGE_SIZE : remoteResources.size()-(pageNumber-1)*PAGE_SIZE)) choisePos = (pageNumber*PAGE_SIZE < remoteResources.size() ? PAGE_SIZE-1 : remoteResources.size()-(pageNumber-1)*PAGE_SIZE-1);
}