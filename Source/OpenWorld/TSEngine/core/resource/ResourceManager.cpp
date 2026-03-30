#include "ResourceManager.h"

namespace TSEngine {
	bool ResourceManager::Load(Entity* TsEntity, Prefab& Prefab)
	{
		auto it = resourceHandler.find(Prefab.prefabType);
		if (it!=resourceHandler.end()) {
			it->second->Load(TsEntity,Prefab);
			return true;
		}
		return false;
	}
	bool ResourceManager::Update(const std::string& Type, Entity* TsEntity, std::map<std::string, std::string> Property)
	{
		auto it = resourceHandler.find(Type);
		if (it != resourceHandler.end()) {
			it->second->update(TsEntity, Property);
			return true;
		}
		return false;
	}

	ResourceManager::~ResourceManager()
	{
		if (StreamableManager)
		{
			delete StreamableManager;
			StreamableManager = nullptr;
		}
	}

	ResourceManager::ResourceManager()
	{
		StreamableManager = new FStreamableManager();
	}
}
