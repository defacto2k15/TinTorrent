//
// Created by defacto on 01.01.17.
//

#include "FileManagerThread.h"
#include <Kernel/Kernel.h>

FileManagerThread::FileManagerThread(Kernel &kernel, std::string workingDirectory)
		: ActionQueue(this), kernel(kernel), fileManager(workingDirectory){
}

std::vector<FileInfo> FileManagerThread::initialCheck() {
	std::cout<<"FileManagerThread: initialCheck "<<std::endl;
	return fileManager.initialCheck();
}

void FileManagerThread::startScheduledCheckings(unsigned timeBetweenChecks) {
	std::cout<<"FileManagerThread: startScheduledCheckings "<<std::endl;
	sheduledCheckThread = std::thread([timeBetweenChecks, this](){
		while(true){ //todo do sth with it
			std::this_thread::sleep_for(std::chrono::seconds(timeBetweenChecks));
			add( [this]( FileManagerThread &th){ th.checkChanges();});
		}
	});
}

void FileManagerThread::createEmptyResource(Resource resource) {
	fileManager.createEmptyResource(resource);
}

void FileManagerThread::checkChanges() {
	std::cout<<"FileManagerThread: checkChanges "<<std::endl;
	UpdateInfo update = fileManager.update();
	std::cout<<"FileManagerThread: updateInfo " << update <<std::endl;
	if(!update.empty() ){
		kernel.add([update]( Kernel &k ){
			k.workingDirectoryChanged(update);
		});
	}
}

void FileManagerThread::requestSegments(int threadId, Resource &resource, SegmentInfo segmentInfo) {
	std::cout<<Help::Str("FileManagerThread: requestSegments threadId: ",threadId," resource ",resource, " segment info ",segmentInfo) <<std::endl;
	try{
		auto segmentIndex = segmentInfo.getSegmentIndex();
		SegmentsSet resultSet = fileManager.getSegments(resource,  SegmentRange(segmentIndex,segmentIndex+1 ));
		std::cout<<Help::Str("FileManagerThread: segment was present") << std::endl;
		kernel.add( [resultSet, threadId]( Kernel &k){
			k.requestedSegmentProvided( threadId, resultSet);
		});
	} catch( MissingRequiredFile ex ){
		std::cout<<Help::Str("FileManagerThread: segment was missing") << std::endl;
		kernel.add( [threadId, resource]( Kernel &k){ k.requestedResourceMissing(threadId, resource);});
	}
}

void FileManagerThread::setSegments(Resource &resource, SegmentsSet segmentsSet) {
	std::cout<<Help::Str("FileManagerThread: setSegments  resource: ",resource, " segmentsSet ",segmentsSet) <<std::endl;
	try{
		fileManager.setSegments(resource, segmentsSet);
	} catch (MissingRequiredFile ex ){
		std::cout<<Help::Str("FileManagerThread: segment was missing") << std::endl;
		kernel.add( [ resource]( Kernel &k){ k.resourceMissing( resource);});
	}
}

void FileManagerThread::removeResource(Resource resource) {
	fileManager.removeResource(resource);
}
