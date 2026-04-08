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
		static EntityManager instance;
		return &instance;
	}

	int EntityManager::CreateEntity(const std::string& EntityType)
	{
		Ids++;
		Entity *entity = new Entity();
		entity->EntityType = EntityType;
		entity->ID = Ids;
		entity->Status = EntityLifeStatus::PreLoad;
		MEntityMaps.emplace(Ids, entity);
		MLoadEntities.push_back(entity);
		return Ids;
	}

	void EntityManager::AddRenderEntity(Entity* entity)
	{
		if (entity) {
			MRenderEntities.push_back(entity);
		}
	}

	void EntityManager::PreDestroyEntity(const int EntityId)
	{
		if (MEntityMaps.contains(EntityId)) {
			Entity* entity = MEntityMaps[EntityId];
			entity->Status = EntityLifeStatus::PreDestroy;
			MDestroyEntities.push_back(entity);
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
		std::vector<Entity*> copyRender(MRenderEntities);
		MRenderEntities.clear();
		return copyRender;
	}

	std::vector<Entity*> EntityManager::FetchDestroyEntity()
	{
		std::vector<Entity*> copyDestroy(MDestroyEntities);
		MDestroyEntities.clear();
		return copyDestroy;
	}

	Entity* EntityManager::GetEntity(int EntityId)
	{
		if (MEntityMaps.contains(EntityId)) {
			return MEntityMaps[EntityId];
		}
		return nullptr;
	}

	void EntityManager::RemoveEntity(int EntityId)
	{
		if (MEntityMaps.contains(EntityId)) {
			Entity* entity = MEntityMaps[EntityId];
			MEntityMaps.erase(EntityId);
			delete entity;
		}
	}
}
