#include <ranges>
#include <core/entity/TypeScriptEntityManager.h>


namespace TSEngine {
	EntityManager::~EntityManager()
	{
		for (const auto Val : MEntityMaps | std::views::values)
		{
			delete Val;
		}
	}

	EntityManager* EntityManager::GetInstance()
	{
		static EntityManager* instance;
		return instance;
	}
	int EntityManager::CreateEntity(const std::string& EntityType)
	{	
		Ids++;
		Entity *entity = new Entity();
		entity->EntityType = EntityType;
		entity->ID = Ids;
		MEntityMaps.emplace(Ids, entity);
		MLoadEntities.push_back(entity);
		return Ids;
	}
	void EntityManager::PreDestroyEntity(const int EntityId)
	{	
		if (MEntityMaps.contains(EntityId)) {
			Entity* entity = MEntityMaps[EntityId];
			entity->Status = EntityLifeStatus::PreLoad;
		}
	}
	std::vector<Entity*> EntityManager::FetchLoadEntity()
	{	
		std::vector<Entity*> copyLoad(MLoadEntities);
		MLoadEntities.clear();
		return copyLoad;
	}
	std::vector<Entity*> EntityManager::FetchRenderEntity()
	{	
		return MRenderEntities;
	}
}