#pragma once
#include "Entity.h"
#include "SwapQueue.h"
#include "unordered_map"
#include <string>
#include <vector>
namespace TSEngine {

	class EntityManager {
    private:
        uint8_t Ids = 0;
        std::unordered_map<int, Entity*> MEntityMaps;
        std::vector<Entity*> MRenderEntities;
        std::vector<Entity*> MLoadEntities;
        std::vector<Entity*> MDestroyEntities;
    protected:
        EntityManager() = default;
        ~EntityManager();

    public:
        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;
        static EntityManager* GetInstance();

        int CreateEntity(const std::string& EntityType);

        void PreDestroyEntity(int EntityId);

        std::vector<Entity*> FetchLoadEntity();

        std::vector<Entity*> FetchRenderEntity();
	};
}
