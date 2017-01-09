#include "MainMenuScreen.h"
#include <iostream>
#include <ncurses.h>

MainMenuScreen::MainMenuScreen(std::string name, Kernel *k) : Screen(name, k)
{
	choisePos = 0;
	activeOption = 0;
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
	
	if(choisePos == 3) attron( A_UNDERLINE);
	printw( "4. Wyjscie\n" );
	attroff( A_UNDERLINE);
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
			if(choisePos == 0)
			{
				return "local_resources";
			}
			else if(choisePos == 1)
			{
				return "other_clients";
			}
			else if(choisePos == 2)
			{
				return "status";
			}
			else if(choisePos == 3)
			{
				return "exit";
			}
			break;
		}
	}
	return "";
}