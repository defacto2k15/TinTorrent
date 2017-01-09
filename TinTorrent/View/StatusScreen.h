#pragma once
#include <View/Screen.h>
#include <Kernel/Kernel.h>
#include <ProgramInfoProvider/outStructures/OutTinResourcesInOtherClients.h>
#include <ProgramInfoProvider/outStructures/OutServerConnectionInfo.h>
#include <Common/StringHelp.h>

#define PAGE_SIZE 20

class StatusScreen: virtual public Screen {
private:
	int choisePos;
	int activeOption;
	unsigned int pageNumber;
	std::vector<OutClientConnectionInfo> outConnections;
	std::vector<OutServerConnectionInfo> inConnections;
public:
	StatusScreen(std::string name, Kernel *k);
	void drawScreen();
	std::string inputHandle();
};