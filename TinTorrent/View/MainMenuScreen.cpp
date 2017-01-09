#include "MainMenuScreen.h"
#include <iostream>
#include <ncurses.h>

MainMenuScreen::MainMenuScreen(std::string name, Kernel *k) : Screen(name, k)
{
	choisePos = 0;
	activeOption = 0;
	pageNumber = 1;
	remoteResources.clear();
	outConnections.clear();
	inConnections.clear();
}

void MainMenuScreen::drawScreen()
{
   printw( "TinTorrent - menu:\n\n" ); //tekst
	if(choisePos == 0) attron( A_UNDERLINE);
   printw( "1. Wyswietlenie lokalnych zasobow\n" );
	attroff( A_UNDERLINE);
	
	if(choisePos == 1) attron( A_UNDERLINE);
	printw( "2. Wyswietlenie wezlow\n" );
	attroff( A_UNDERLINE);

	if(choisePos == 2) attron( A_UNDERLINE);
	printw( "3. Status\n" );
	attroff( A_UNDERLINE);
	
	if(activeOption == 2)
	{
		printw( "\nLiczba wezlow: %d [Strona %d/%d]\n", remoteResources.size(),
				  pageNumber, remoteResources.size()/PAGE_SIZE+1 );
		printw( "%20s%20s\n", "Adres:", "Liczba zasobow:" );
		std::vector<OutTinResourcesInOtherClients>::iterator resEnd = (pageNumber * PAGE_SIZE < remoteResources.size() ? remoteResources.begin() + (pageNumber-1)*PAGE_SIZE + PAGE_SIZE : remoteResources.end());  
		for(std::vector<OutTinResourcesInOtherClients>::iterator it = remoteResources.begin()+(pageNumber-1)*PAGE_SIZE;
			 it != resEnd; ++it)
		{
			std::stringstream ss;
			ss << (*it).getAddress();
			printw( "%20.20s%20d\n", ss.str().c_str(), (*it).getResources().size());
		}
	}
	else if(activeOption == 3)
	{
		printw( "\nStatus kernela:\n" );
		printw( "- polaczenia wychodzace: %d\n", outConnections.size() );
		for(std::vector<OutClientConnectionInfo>::iterator it = outConnections.begin();
			 it != outConnections.end(); ++it)
		{
			printw("conn\n");
		}
	}
}

std::string MainMenuScreen::inputHandle() 
{
	int input = getch();
	switch(input) {
		case KEY_DOWN:
			choisePos = (choisePos+1)%NO_OF_OPTIONS;
			break;
		case KEY_UP:
			choisePos--;
			if(choisePos < 0) choisePos = NO_OF_OPTIONS-1;
			break;
		case 10:
		{
			ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
			if(choisePos == 0)
			{
				return "local_resources";
			}
			else if(choisePos == 1)
			{
				pageNumber = 1;
				activeOption = 2;
				remoteResources = infoProvider.getResourcesInOtherClients();
			}
			else if(choisePos == 2)
			{
				pageNumber = 1;
				activeOption = 3;
				outConnections = infoProvider.getConnectionsToOtherServersInfo();
				inConnections = infoProvider.getConnectionsToUsInfo();
			}
			break;
		}
		case KEY_RIGHT:
			if(pageNumber < remoteResources.size()/PAGE_SIZE+1) pageNumber++;
			break;
		case KEY_LEFT:
			if(pageNumber > 1) pageNumber--;
			break;
	}
	return "";
}