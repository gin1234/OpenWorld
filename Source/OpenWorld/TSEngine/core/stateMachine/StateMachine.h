#pragma once
#include <string>
#include <unordered_map>

namespace TSEngine {
	class StateMachine {
	public:
		StateMachine() = default;
		explicit StateMachine(std::string entityType);

		std::string Get(std::string lastState, std::string currentState);

	private:
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> stateMap = 
			std::unordered_map<std::string, std::unordered_map<std::string, std::string>>();
	};
}