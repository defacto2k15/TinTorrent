#include "MainMenuScreen.h"
#include <iostream>
#include <ncurses.h>

MainMenuScreen::MainMenuScreen(std::string name, Kernel *k) : Screen(name, k)
{
	choisePos = 0;
	activeOption = 0;
	localResources.clear();
}

void MainMenuScreen::drawScreen()
{
   printw( "TinTorrent - menu:\n\n" ); //tekst
	if(choisePos == 0) attron( A_UNDERLINE);
   printw( "1. Wyswietlenie lokalnych zasobow\n" );
	attroff( A_UNDERLINE);
	
	if(choisePos == 1) attron( A_UNDERLINE);
	printw( "2. Jakas opcja\n" );
	attroff( A_UNDERLINE);
	
	if(activeOption == 1)
	{
		printw( "\nLokalne zasoby: %d\n", localResources.size() );
		printw( " Nazwa:\tRozmiar:\tPobrany procent:\n" );
		for(std::vector<OutLocalResource>::iterator it = localResources.begin(); it != localResources.end(); ++it)
		{
			std::string resourceName = StringHelp::toUtf8((*it).resource.getResourceName());
			printw( "\n%s\t%d\t[%d/100%]\n", resourceName.c_str(),
														(*it).resource.getResourceSize(),												      
														(*it).percentDownloaded);
		}
	}
}

std::string MainMenuScreen::inputHandle() 
{
	int input = getch();
	switch(input) {
		case KEY_DOWN:
			choisePos = (choisePos+1)%3;
			break;
		case KEY_UP:
			choisePos--;
			if(choisePos < 0) choisePos = 2;
			break;
		case KEY_RIGHT:
			ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
			if(choisePos == 0)
			{
				activeOption = 1;
				localResources = infoProvider.getLocalResources();
			}
			break;
	}
	return "";
}