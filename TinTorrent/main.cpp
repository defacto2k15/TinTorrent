#include <iostream>
#include <Kernel/Kernel.h>

int main(int argc, const char **argv) {
	if( argc != 2 ){
		std::cout << "Arguments: [workingDirecory]" <<std::endl;
	}
	Kernel kernel;
	kernel.startApp(std::string(argv[1]), std::string());
	kernel.join();
}