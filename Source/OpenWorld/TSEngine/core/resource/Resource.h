#pragma once
#include "../config/PrefabCfg.h"
#include "../entity/Entity.h"
#include "Engine/StreamableManager.h"

namespace TSEngine {
	class ResourceBase {
	public:

		virtual bool Load(Entity* tsEntity,Prefab prefab, FStreamableManager* StreamableManager) = 0;

		virtual bool Update(Entity* tsEntity, std::map<std::string,std::string> property) = 0;
	};

	struct ResourceConst {
		const std::string Canvas = "Canvas";
		const std::string Mesh = "Mesh";
		const std::string Effect = "Effect";

	};
}
