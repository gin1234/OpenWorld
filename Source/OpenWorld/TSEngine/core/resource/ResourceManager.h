#pragma once
#include "Resource.h"
#include <unordered_map>

#include "Engine/StreamableManager.h"

namespace TSEngine {
	class ResourceManager {
	public:
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		
		static ResourceManager* GetInstance() {
			static ResourceManager* instance;
			return instance;
		};

		bool Load(Entity* TsEntity, Prefab& Prefab);

		bool Update(const std::string& Type, Entity* TsEntity, std::map<std::string, std::string> Property);
		~ResourceManager();
	private:
		ResourceManager();
		std::unordered_map<std::string, Resource*> resourceHandler = std::unordered_map<std::string, Resource*>();
		FStreamableManager* StreamableManager;
	};
}
