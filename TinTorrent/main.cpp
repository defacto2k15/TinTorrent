#include <iostream>
#include <Kernel/Kernel.h>
#include <View/View.h>
#include <Common/ConfigLoader.h>

int main(int argc, const char **argv) {
	if (argc != 2 && argc != 3) {
		std::cout << "Arguments: [workingDirecory] (configuration file)" << std::endl;
	}
	std::string configurationPath;
	if( argc== 3 ){
		configurationPath = std::string(argv[2]);
	}
	LogBase log("main");

	// set configuration
	log.debug("setting configuration");
	ConfigLoader loader;
	loader.loadConfigs(configurationPath);

	Kernel kernel;
	kernel.startApp(std::string(argv[1]));

	if (Constants::guiActive) {
		View view(&kernel);
		view.start();
		kernel.closeKernel();
	}

	kernel.join();
	return 0; 
}