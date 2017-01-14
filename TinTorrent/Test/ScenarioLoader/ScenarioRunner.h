//
// Created by defacto on 14.01.17.
//

#ifndef TINTORRENT_SCENARIORUNNER_H
#define TINTORRENT_SCENARIORUNNER_H


#include <Logging/LogBase.h>
#include <fstream>
#include <Kernel/Kernel.h>
#include "ScenarioAction.h"

class ScenarioRunner  {
	LogBase log;
	Kernel &kernel;
	std::map<std::string, std::function<void(Resource &res, ProgramInfoProvider &p, std::string pathToOutFile)>> actionsFunc;

public:
	ScenarioRunner(Kernel &kernel) : log("ScenarioRunner"), kernel(kernel){

		actionsFunc["getLocalResources"] = [this](Resource &res, ProgramInfoProvider &p, std::string pathToOutFile){
			writeToFile(p.getLocalResources(), pathToOutFile);
		};

		actionsFunc["getResourcesInOtherClients"] = [this](Resource &res, ProgramInfoProvider &p, std::string pathToOutFile){
			writeToFile(p.getResourcesInOtherClients(), pathToOutFile);
		};

		actionsFunc["getRevertedResources"] = [this](Resource &res, ProgramInfoProvider &p, std::string pathToOutFile){
			writeToFile(p.getRevertedResources(), pathToOutFile);
		};

		actionsFunc["getConnectionsToOtherServersInfo"] = [this](Resource &res, ProgramInfoProvider &p, std::string pathToOutFile){
			writeToFile(p.getConnectionsToOtherServersInfo(), pathToOutFile);
		};

		actionsFunc["getConnectionsToUsInfo"] = [this](Resource &res, ProgramInfoProvider &p, std::string pathToOutFile){
			writeToFile(p.getConnectionsToUsInfo(), pathToOutFile);
		};

		actionsFunc["changeResourceBlockState"] = [this](Resource &res, ProgramInfoProvider &p, std::string pathToOutFile){
			p.changeResourceBlockState(res);
		};

		actionsFunc["changeResourceAnnouncementState"] = [this](Resource &res, ProgramInfoProvider &p, std::string pathToOutFile){
			p.changeResourceAnnouncementState(res);
		};

		actionsFunc["getResourcesThatCanBeAnnounced"] = [this](Resource &res, ProgramInfoProvider &p, std::string pathToOutFile){
			writeToFile(p.getResourcesThatCanBeAnnounced(), pathToOutFile);
		};

		actionsFunc["getResourcesThatCanBeDownloaded"] = [this](Resource &res, ProgramInfoProvider &p, std::string pathToOutFile){
			writeToFile(p.getResourcesThatCanBeDownloaded(), pathToOutFile);
		};

		actionsFunc["orderResourceDownload"] = [this](Resource &res, ProgramInfoProvider &p, std::string pathToOutFile){
			p.orderResourceDownload(res);
		};
	}




	void runScenatio( std::string scenarioFilePath ){
		if (scenarioFilePath.empty()) {
			log.debug("Scenatio filepath is empty - wont load file ");
			return;
		}
		std::ifstream t(scenarioFilePath);
		if (!t) {
			log.debug("Cannot read scenatio file ", scenarioFilePath);
			return;
		}
		std::string filePayload((std::istreambuf_iterator<char>(t)),
		                        std::istreambuf_iterator<char>());

		std::vector<ScenarioAction> actions;
		try {
			actions = parseScenario(filePayload);
		} catch( std::exception &e){
			log.warn(Help::Str("Error during scenarion file parsing: ", e.what()));
			return;
		}
		log.debug("Loading configuration file ", scenarioFilePath, " OK");

		std::sort(actions.begin(), actions.end(),
		     [](const ScenarioAction & a, const ScenarioAction & b) -> bool
		     {
			     return a.getDelay() < b.getDelay();
		     });

		auto startTime = std::chrono::system_clock::now();
		for( auto &action : actions){
			auto now = std::chrono::system_clock::now();
			while ( now < startTime + std::chrono::seconds(action.getDelay())){
				now = std::chrono::system_clock::now();
				// ugly code but works
			}
			auto t =std::chrono::system_clock::to_time_t(now);

			log.debug(Help::Str("W ",std::ctime(&t) ," wykonujemy akcje ", action ));
			action.run(kernel.getProgramInfoProvider());
		}
	}

private:
	std::vector<ScenarioAction> parseScenario(std::string &scenarioFile){
		auto j = json::parse(scenarioFile);
		if( !j.is_array()){
			log.debug("Passed json should be array, but is not. Wont load");
			return {};
		}

		std::vector<ScenarioAction> outActionsVec;
		for( json oneActionJson : j){
			int delay = oneActionJson["delay"];
			std::string action  = oneActionJson["action"];
			std::string target;
			if( oneActionJson.count("target") != 0) {
				target = oneActionJson["target"];
			}
			Resource resource;
			if( oneActionJson.count("value") != 0 ){
				resource.parseJson(oneActionJson["value"]);
			}

			if( actionsFunc.count(action) == 0){
				log.warn(Help::Str("There is no action of name ",action," defined, will be ignored "));
			} else {
				outActionsVec.push_back(ScenarioAction(actionsFunc[action], action, delay, target, resource));
			}
		}
		return outActionsVec;
	}

	template <typename T>
	void writeToFile( std::vector<T> vec, std::string fileName ){
		if( fileName.empty()){
			return;
		}
		std::vector<json> outValues;
		for( auto &elem : vec ){
			outValues.push_back( elem.toJson());
		}
		json j(outValues);

		std::ofstream out(fileName);
		out << j.dump();
		out.close();
	}
};


#endif //TINTORRENT_SCENARIORUNNER_H
