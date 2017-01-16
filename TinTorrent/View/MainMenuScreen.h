#pragma once
#include <View/Screen.h>
#include <Kernel/Kernel.h>
#include <ProgramInfoProvider/outStructures/OutServerConnectionInfo.h>
#include <Common/StringHelp.h>

#define NO_OF_OPTIONS 6

class MainMenuScreen: virtual public Screen {
private:
	int choisePos;
	int activeOption;
	std::vector<OutClientConnectionInfo> outConnections;
	std::vector<OutServerConnectionInfo> inConnections;
public:
	MainMenuScreen(std::string name, Kernel *k);
	void drawScreen();
	std::string inputHandle();
	void refresh() {};
};