//
// Created by defacto on 07.01.17.
//

#ifndef TINTORRENT_PROGRAMINFOPROVIDER_H
#define TINTORRENT_PROGRAMINFOPROVIDER_H

#include <ProgramInfoProvider/outStructures/OutLocalResource.h>
#include <Multithreading/BlockingElement.h>
#include <future>

class Kernel;

class ProgramInfoProvider{
	Kernel &kernel;
public:
	ProgramInfoProvider( Kernel &kernel );

	std::vector<OutLocalResource> getLocalResources();
};

#endif //TINTORRENT_PROGRAMINFOPROVIDER_H
