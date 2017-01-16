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
	attron(COLOR_PAIR(3));
   printw( "TinTorrent - menu:\n\n" ); //tekst
	if(choisePos == 0) { attron( A_UNDERLINE); attron(COLOR_PAIR(2)); }
   printw( "1. Lokalne zasoby\n" );
	attroff( A_UNDERLINE); attron(COLOR_PAIR(3));
	
	if(choisePos == 1) { attron( A_UNDERLINE); attron(COLOR_PAIR(2)); }
   printw( "2. Zakazane zasoby\n" );
	attroff( A_UNDERLINE); attron(COLOR_PAIR(3));

	if(choisePos == 2) { attron( A_UNDERLINE); attron(COLOR_PAIR(2)); }
	printw( "3. Wyswietlenie wezlow\n" );
	attroff( A_UNDERLINE); attron(COLOR_PAIR(3));
	
	if(choisePos == 3) { attron( A_UNDERLINE); attron(COLOR_PAIR(2)); }
	printw( "4. Zasoby do pobrania\n" );
	attroff( A_UNDERLINE); attron(COLOR_PAIR(3));

	if(choisePos == 4) { attron( A_UNDERLINE); attron(COLOR_PAIR(2)); }
	printw( "5. Status\n" );
	attroff( A_UNDERLINE); attron(COLOR_PAIR(3));
	
	if(choisePos == 5) { attron( A_UNDERLINE); attron(COLOR_PAIR(2)); }
	printw( "6. Wyjscie\n" );
	attroff( A_UNDERLINE); attron(COLOR_PAIR(3));
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
				return "stopped_resources";
			}
			else if(choisePos == 2)
			{
				return "other_clients";
			}
			else if(choisePos == 3)
			{
				return "remote_resources";
			}
			else if(choisePos == 4)
			{
				return "status";
			}
			else if(choisePos == 5)
			{
				return "exit";
			}
			break;
		}
	}
	return "";
}