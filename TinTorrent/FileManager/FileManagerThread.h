//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_WORKINGDIRECTORYMANAGERTHREAD_H
#define TINTORRENT_WORKINGDIRECTORYMANAGERTHREAD_H


#include <Multithreading/ActionQueue.h>
#include <Multithreading/ActionThread.h>
#include "WorkingDirectoryManager.h"
#include "FileManager.h"
class Kernel;

class FileManagerThread : public ActionQueue<FileManagerThread> {
	FileManager fileManager;
	Kernel &kernel;
	std::unique_ptr<ActionThread> scheduledCheckThread;
public:
	FileManagerThread( Kernel &kernel, std::string workingDirectory );

	std::vector<FileInfo> initialCheck();

	void startScheduledCheckings(unsigned timeBetweenChecks );

	void createEmptyResource( Resource resource );

	void checkChanges();

	void requestSegments(int threadId, Resource &resource, SegmentInfo segmentInfo );

	void setSegments(Resource &resource, SegmentsSet segmentsSet );

	void removeResource( Resource resource );

	virtual void internalKillYourself() override {
		std::cout << "FileManagerThread. Dying. killing scheduledCheckThread " << std::endl;
		scheduledCheckThread->killYourself();

	}

	virtual void internalJoin() override {
		std::cout << "FileManagerThread. Joining scheduledCheckThread " << std::endl;
		scheduledCheckThread->join();
	}
};


#endif //TINTORRENT_WORKINGDIRECTORYMANAGERTHREAD_H
