#pragma once
#include <string>
#include <unordered_map>
namespace TSEngine {
	class Message {
	public:
		int mailId;
		std::unordered_map<std::string, std::string> param;
	};
}