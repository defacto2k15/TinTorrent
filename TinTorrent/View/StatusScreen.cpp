#include "StatusScreen.h"
#include <iostream>
#include <ncurses.h>

StatusScreen::StatusScreen(std::string name, Kernel *k) : Screen(name, k)
{
	choisePos = 0;
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	outConnections = infoProvider.getConnectionsToOtherServersInfo();
	inConnections = infoProvider.getConnectionsToUsInfo();
}

void StatusScreen::drawScreen()
{
	printw( "<- Q-powrót\tF5-odswiez\n" );
	printw( "\nStatus kernela:\n" );
	printw( "- polaczenia wychodzace: %d\n", outConnections.size() );
	for(std::vector<OutClientConnectionInfo>::iterator it = outConnections.begin();
		 it != outConnections.end(); ++it)
	{
		printw("  - conn\n");
	}
	printw( "- polaczenia przychodzace: %d\n", outConnections.size() );
	for(std::vector<OutServerConnectionInfo>::iterator it = inConnections.begin();
		 it != inConnections.end(); ++it)
	{
		printw("  - conn\n");
	}
}

std::string StatusScreen::inputHandle() 
{
	int input = getch();
	switch(input) {
		case KEY_DOWN:
			break;
		case KEY_UP:
			break;
		case 10:
		{
			break;
		}
		case KEY_F(5):
		{
			choisePos = 0;
			ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
			outConnections = infoProvider.getConnectionsToOtherServersInfo();
			inConnections = infoProvider.getConnectionsToUsInfo();
			break;
		}
		case 113:	// KEY_Q
			return "main_menu";
		default:
			break;
	}
	return "";
}

void StatusScreen::refresh()
{
	ProgramInfoProvider infoProvider = kernel->getProgramInfoProvider();
	outConnections = infoProvider.getConnectionsToOtherServersInfo();
	inConnections = infoProvider.getConnectionsToUsInfo();
}