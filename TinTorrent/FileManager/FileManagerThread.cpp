//
// Created by defacto on 01.01.17.
//

#include "FileManagerThread.h"
#include <Kernel/Kernel.h>

FileManagerThread::FileManagerThread(Kernel &kernel, std::string workingDirectory)
		: ActionQueue(this, "FileManagerThread"), log("FileManagerThread"),  fileManager(workingDirectory), kernel(kernel){
}

std::vector<FileInfo> FileManagerThread::initialCheck() {
	log.debug(" initialCheck ");
	return fileManager.initialCheck();
}

void FileManagerThread::startScheduledCheckings(unsigned timeBetweenChecks) {
	log.debug(" startScheduledCheckings ");
	scheduledCheckThread = std::make_unique<ActionThread>( [timeBetweenChecks, this](){
			add( [this]( FileManagerThread &th){ th.checkChanges();});
		}, timeBetweenChecks, "FileManagerScheduledCheckingThread");
	scheduledCheckThread->start();
}

void FileManagerThread::createEmptyResource(Resource resource) {
	fileManager.createEmptyResource(resource);
}

void FileManagerThread::checkChanges() {
	log.debug(" checkChanges ");
	UpdateInfo update = fileManager.update();
	log.debug(" updateInfo ", update );
	if(!update.empty() ){
		kernel.add([update]( Kernel &k ){
			k.workingDirectoryChanged(update);
		});
	}
}

void FileManagerThread::requestSegments(int threadId, Resource &resource, SegmentInfo segmentInfo) {
	log.debug(" requestSegments threadId: ",threadId," resource ",resource, " segment info ",segmentInfo) ;
	try{
		auto segmentIndex = segmentInfo.getSegmentIndex();
		SegmentsSet resultSet = fileManager.getSegments(resource,  SegmentRange(segmentIndex,segmentIndex+1 ));
		log.debug(" segment was present");
		kernel.add( [resultSet, threadId]( Kernel &k){
			k.requestedSegmentProvided( threadId, resultSet);
		});
	} catch( MissingRequiredFile ex ){
		log.warn(" segment was missing");
		kernel.add( [threadId, resource]( Kernel &k){ k.requestedResourceMissing(threadId, resource);});
	}
}

void FileManagerThread::setSegments(Resource &resource, SegmentsSet segmentsSet) {
	log.debug(" setSegments  resource: ",resource, " segmentsSet ",segmentsSet) ;
	try{
		fileManager.setSegments(resource, segmentsSet);
	} catch (MissingRequiredFile ex ){
		log.warn("SetSegments: segment was missing");
		kernel.add( [ resource]( Kernel &k){ k.resourceMissing( resource);});
	}
}

void FileManagerThread::removeResource(Resource resource) {
	log.debug(Help::Str(" removeResource ",resource));
	fileManager.removeResource(resource);
}

void FileManagerThread::internalKillYourself() {
	log.debug(" Dying. killing scheduledCheckThread ");
	scheduledCheckThread->killYourself();

}

void FileManagerThread::internalJoin() {
	log.debug(" Joining scheduledCheckThread ");
	scheduledCheckThread->join();
}
