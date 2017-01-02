//
// Created by defacto on 01.01.17.
//

#ifndef TINTORRENT_WORKINGDIRECTORYMANAGERTHREAD_H
#define TINTORRENT_WORKINGDIRECTORYMANAGERTHREAD_H


#include <Multithreading/ActionQueue.h>
#include <Kernel/Kernel.h>
#include "WorkingDirectoryManager.h"
#include "FileManager.h"

class FileManagerThread : ActionQueue<FileManagerThread> {
	FileManager fileManager;
	Kernel &kernel;
	std::thread sheduledCheckThread;
public:
	FileManagerThread( Kernel &kernel, std::string workingDirectory )
			: ActionQueue(this), kernel(kernel), fileManager(workingDirectory){
	}

	std::vector<FileInfo> initialCheck(){
		std::cout<<"FileManagerThread: initialCheck "<<std::endl;
		return fileManager.initialCheck();
	}

	void startScheduledCheckings(unsigned timeBetweenChecks ){
		std::cout<<"FileManagerThread: startScheduledCheckings "<<std::endl;
		sheduledCheckThread = std::thread([timeBetweenChecks, this](){
			while(true){ //todo do sth with it
				std::this_thread::sleep_for(std::chrono::seconds(timeBetweenChecks));
				add( [this]( FileManagerThread &th){ th.checkChanges();});
			}
		});
	}

	void createEmptyResource( Resource resource ){
		fileManager.createEmptyResource(resource);
	}

	void checkChanges(){
		std::cout<<"FileManagerThread: checkChanges "<<std::endl;
		UpdateInfo update = fileManager.update();
		std::cout<<"FileManagerThread: updateInfo " << update <<std::endl;
		if(!update.empty() ){
			kernel.add([update]( Kernel &k ){
				k.workingDirectoryChanged(update);
			});
		}
	}

	void requestSegments(int threadId, Resource &resource, SegmentInfo segmentInfo ){
		std::cout<<Help::Str("FileManagerThread: requestSegments threadId: ",threadId," resource ",resource, " segment info ",segmentInfo) <<std::endl;
		try{
			auto segmentIndex = segmentInfo.getSegmentIndex();
			SegmentsSet resultSet = fileManager.getSegments(resource,  SegmentRange(segmentIndex,segmentIndex+1 ));
			std::cout<<Help::Str("FileManagerThread: segment was present") << std::endl;
			kernel.add( [resultSet]( Kernel &k){
				k.requestedSegmentProvided( threadId, resultSet);
			});
		} catch( MissingRequiredFile ex ){
			std::cout<<Help::Str("FileManagerThread: segment was missing") << std::endl;
			kernel.add( [threadId, resource]( Kernel &k){ k.requestedResourceMissing(threadId, resource);});
		}


	}

};


#endif //TINTORRENT_WORKINGDIRECTORYMANAGERTHREAD_H
