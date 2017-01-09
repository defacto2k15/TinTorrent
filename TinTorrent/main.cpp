#include <iostream>
#include <Kernel/Kernel.h>
#include <View/View.h>

int main(int argc, const char **argv) {
	if( argc != 2 ){
		std::cout << "Arguments: [workingDirecory]" <<std::endl;
	}
	Kernel kernel;
	kernel.startApp(std::string(argv[1]), std::string());

	View view(&kernel);
	view.start();
	kernel.closeKernel();
	kernel.join();
	return 0; 
}