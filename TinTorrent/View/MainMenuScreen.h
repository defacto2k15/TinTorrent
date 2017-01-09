//
// Created by Ransho 08.01.2017
//

#include <View/Screen.h>
#include <Kernel/Kernel.h>
#include <ProgramInfoProvider/outStructures/OutLocalResource.h>
#include <Common/StringHelp.h>

class MainMenuScreen: virtual public Screen {
private:
	int choisePos;
	int activeOption;
	std::vector<OutLocalResource> localResources;
public:
	MainMenuScreen(std::string name, Kernel *k);
	void drawScreen();
	std::string inputHandle();
};