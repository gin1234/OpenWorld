#include "EntityState.h"

namespace TSEngine {
	EntityState::EntityState(std::string entityType):stateMachine(entityType)
	{
		
	}
	std::string EntityState::MatchState(Entity* entity)
	{
		if (entity->LastState != entity->CurrentState) {
			std::string resStateId = stateMachine.Get(entity->LastState, entity->CurrentState);
			if (resStateId.empty()) {
				return "";
			}
			entity->LastState = entity->CurrentState;
			entity->ResStateId = resStateId;
			return resStateId;
		}
		return "";
	}
}