#include <iostream>
#include <Kernel/Kernel.h>
#include <View/View.h>
#include <Common/ConfigLoader.h>
#include <Test/ScenarioLoader/ScenarioRunner.h>

int main(int argc, const char **argv) {
	if (argc != 2 && argc != 3 && argc != 4) {
		std::cout << "Arguments: [workingDirecory] (configuration file)" << std::endl;
	}

	std::string configurationPath;
	if( argc>= 3 ){
		configurationPath = std::string(argv[2]);
	}
	LogBase log("main");

	// set configuration
	log.debug(Help::Str("setting configuration. ConfigurationPath ",configurationPath));
	ConfigLoader loader;
	loader.loadConfigs(configurationPath);

	Kernel kernel;
	kernel.startApp(std::string(argv[1]));

	if( argc == 4 ){
		log.debug("Using scenario file ", argv[3]);
		ScenarioRunner runner(kernel);
		runner.runScenatio(argv[3]);
	} else if (Constants::guiActive) {
		View view(&kernel);
		view.start();
		//kernel.closeKernel();
	}

	kernel.join();
	return 0; 
}