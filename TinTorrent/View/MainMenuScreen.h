#pragma once
#include <View/Screen.h>
#include <Kernel/Kernel.h>
#include <ProgramInfoProvider/outStructures/OutTinResourcesInOtherClients.h>
#include <ProgramInfoProvider/outStructures/OutServerConnectionInfo.h>
#include <Common/StringHelp.h>

#define PAGE_SIZE 10
#define NO_OF_OPTIONS 4

class MainMenuScreen: virtual public Screen {
private:
	int choisePos;
	int activeOption;
	unsigned int pageNumber;
	std::vector<OutTinResourcesInOtherClients> remoteResources;
	std::vector<OutClientConnectionInfo> outConnections;
	std::vector<OutServerConnectionInfo> inConnections;
public:
	MainMenuScreen(std::string name, Kernel *k);
	void drawScreen();
	std::string inputHandle();
};