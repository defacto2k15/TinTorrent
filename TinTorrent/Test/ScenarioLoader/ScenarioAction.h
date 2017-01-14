//
// Created by defacto on 14.01.17.
//

#ifndef TINTORRENT_SCENARIOACTION_H
#define TINTORRENT_SCENARIOACTION_H


#include <ProgramInfoProvider/ProgramInfoProvider.h>
#include <ostream>

class ScenarioAction {
	std::function<void(Resource &res, ProgramInfoProvider &p, std::string pathToOutFile)> actionFunc;
	std::string actionName;
	int delay;
	std::string outputFileTarget;
	Resource resource;

public:
	ScenarioAction( std::function<void(Resource &res, ProgramInfoProvider &p, std::string pathToOutFile)> actionFunc,
	                std::string actionName, int delay, std::string outputFileTarget, Resource resource)
			: actionFunc(actionFunc), actionName(actionName), delay(delay), outputFileTarget(outputFileTarget), resource(resource){
	}

	int getDelay() const{
		return delay;
	}

	void run( ProgramInfoProvider &p ){
		actionFunc(resource, p, outputFileTarget);
	}

	friend std::ostream &operator<<(std::ostream &os, const ScenarioAction &action) {
		os << " delay: " << action.delay << " outputFileTarget: "
		   << action.outputFileTarget << " resource: " << action.resource.toJson().dump();
		return os;
	}
};


#endif //TINTORRENT_SCENARIOACTION_H
