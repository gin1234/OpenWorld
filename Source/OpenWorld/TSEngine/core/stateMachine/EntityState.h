#pragma once
#include <string>
#include "StateMachine.h"
#include "../entity/Entity.h"
namespace TSEngine {
	class EntityState {
	public:
		 EntityState(std::string entityType);

		 std::string MatchState(Entity* entity);
	private:
		StateMachine stateMachine;
	};
}