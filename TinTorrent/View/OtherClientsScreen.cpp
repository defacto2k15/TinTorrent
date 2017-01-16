#include "OtherClientsScreen.h"
#include <iostream>
#include <ncurses.h>

OtherClientsScreen::OtherClientsScreen(std::string name, Kernel *k) : Screen(name, k)
{
	choisePos = 0;
	pageNumber = 1;
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	remoteResources = infoProvider.getResourcesInOtherClients();
}

void OtherClientsScreen::drawScreen()
{
	printw( "<- Q-powrót\tF5-odswiez\n" );
	printw( "\nLiczba wezlow: %d [Strona %d/%d]\n", remoteResources.size(),
			  pageNumber, remoteResources.size()/PAGE_SIZE+1 );
	printw( "%20s%20s\n", "Adres:", "Liczba zasobow:" );
	std::vector<OutTinResourcesInOtherClients>::iterator resEnd = (pageNumber * PAGE_SIZE < remoteResources.size() ? remoteResources.begin() + (pageNumber-1)*PAGE_SIZE + PAGE_SIZE : remoteResources.end());  
	int i = 0;	
	for(std::vector<OutTinResourcesInOtherClients>::iterator it = remoteResources.begin()+(pageNumber-1)*PAGE_SIZE;
		 it != resEnd; ++it)
	{
		std::stringstream ss;
		ss << (*it).getAddress();
		if(i == choisePos) attron( A_UNDERLINE);
		printw( "%20.20s%20d\n", ss.str().c_str(), (*it).getResources().size());
		attroff( A_UNDERLINE);
	}
}

std::string OtherClientsScreen::inputHandle() 
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
		case 10:
		{
			// TODO pokaz zasoby wybranego wezla albo cos
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
			remoteResources = infoProvider.getResourcesInOtherClients();
			break;
		}
		case 113:	// KEY_Q
			return "main_menu";
	}
	return "";
}

void OtherClientsScreen::refresh()
{
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	remoteResources = infoProvider.getResourcesInOtherClients();
	if((unsigned)choisePos > (pageNumber*PAGE_SIZE < remoteResources.size() ? PAGE_SIZE : remoteResources.size()-(pageNumber-1)*PAGE_SIZE)) choisePos = (pageNumber*PAGE_SIZE < remoteResources.size() ? PAGE_SIZE-1 : remoteResources.size()-(pageNumber-1)*PAGE_SIZE-1);
}