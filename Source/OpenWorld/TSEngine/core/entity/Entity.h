#pragma once
#include <string>


namespace TSEngine {
	enum class EntityLifeStatus : int {
		PreLoad,
		Loading,
		FailLoad,
		Rendering,
		PreDestroy,
		FailDestroy
	};

	class Entity {
	public:
		int ID;
		std::string EntityType;
		std::string LastState;
		std::string CurrentState;
		std::string ResStateId;
		EntityLifeStatus Status;
		void OnMessage();

	};

	
}