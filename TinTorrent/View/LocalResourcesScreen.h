#pragma once
#include <View/Screen.h>
#include <Kernel/Kernel.h>
#include <ProgramInfoProvider/outStructures/OutLocalResource.h>
#include <Common/StringHelp.h>

#define PAGE_SIZE 20

class LocalResourcesScreen: virtual public Screen {
private:
	int choisePos;
	int activeOption;
	unsigned int pageNumber;
	std::vector<OutLocalResource> localResources;
public:
	LocalResourcesScreen(std::string name, Kernel *k);
	void drawScreen();
	std::string inputHandle();
};