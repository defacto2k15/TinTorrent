//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_WORKINGDIRECTORYMANAGERTHREAD_H
#define TINTORRENT_WORKINGDIRECTORYMANAGERTHREAD_H


#include <Multithreading/ActionQueue.h>
#include "WorkingDirectoryManager.h"
#include "FileManager.h"
class Kernel;

class FileManagerThread : public ActionQueue<FileManagerThread> {
	FileManager fileManager;
	Kernel &kernel;
	std::thread sheduledCheckThread;
public:
	FileManagerThread( Kernel &kernel, std::string workingDirectory );

	std::vector<FileInfo> initialCheck();

	void startScheduledCheckings(unsigned timeBetweenChecks );

	void createEmptyResource( Resource resource );

	void checkChanges();

	void requestSegments(int threadId, Resource &resource, SegmentInfo segmentInfo );

	void setSegments(Resource &resource, SegmentsSet segmentsSet );

	void removeResource( Resource resource );

};


#endif //TINTORRENT_WORKINGDIRECTORYMANAGERTHREAD_H
