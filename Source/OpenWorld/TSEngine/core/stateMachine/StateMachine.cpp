#include "StateMachine.h"
#include "ResourceCfg.h";
namespace TSEngine{
	StateMachine::StateMachine(std::string entityType)
	{
		Resource res = ResourceCfg::GetInstance().Get(entityType);

		for (const State& state : res.states) {
			if (stateMap.contains(state.srcState)) {
				std::unordered_map<std::string, std::string> currentMap = std::unordered_map<std::string, std::string>();
				stateMap[state.srcState] = currentMap;
			}
			stateMap[state.srcState][state.dstState] = state.resourceStateId;
		}
	}
	std::string StateMachine::Get(std::string lastState, std::string currentState)
	{	
		auto it = stateMap.find(lastState);
		if (it == stateMap.end()) {
			return "";
		}
		auto currentIt = it->second.find(currentState);
		if (currentIt == it->second.end()) {
			return "";
		}
		return currentIt->second;
	}
}