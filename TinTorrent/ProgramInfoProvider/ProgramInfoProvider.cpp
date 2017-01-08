//
// Created by defacto on 07.01.17.
//

#include "ProgramInfoProvider.h"
#include "Kernel/Kernel.h"

ProgramInfoProvider::ProgramInfoProvider(Kernel &kernel) : kernel(kernel){}

std::vector<OutLocalResource> ProgramInfoProvider::getLocalResources() {
	BlockingElement blockingElement;
	std::promise< std::vector<OutLocalResource>> elementPromise;
	kernel.add( [&]( Kernel &k) { k.provideLocalResources( elementPromise ); });
	return (elementPromise.get_future().get());
}
