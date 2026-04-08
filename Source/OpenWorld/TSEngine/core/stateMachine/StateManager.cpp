#include "StateManager.h"
#include <TypeScriptEntityManager.h>
#include <ResourceCfg.h>
#include <ResourceStateCfg.h>
#include <PrefabCfg.h>
#include <resource/ResourceManager.h>

namespace TSEngine {

void StateManager::Update()
{
    OnClose();
    OnLoad();
    OnRender();
}

void StateManager::OnLoad()
{
    std::vector<Entity*> entities = EntityManager::GetInstance()->FetchLoadEntity();

    for (Entity* item : entities) {
        std::string entityType = item->EntityType;

        if (entityStateMap.find(entityType) == entityStateMap.end()) {
            EntityState entityState(entityType);
            entityStateMap[entityType] = entityState;
        }

        Resource res = ResourceCfg::GetInstance().Get(entityType);
        Prefab prefab = PrefabCfg::GetInstance().Get(res.prefabId);

        bool ok = ResourceManager::GetInstance()->Load(item, prefab);
        if (!ok) {
            item->Status = EntityLifeStatus::FailLoad;
            continue;
        }

        item->Status = EntityLifeStatus::Loading;
        loadingEntities.push_back(item);
    }
}

void StateManager::OnRender()
{
    std::vector<Entity*> entities = EntityManager::GetInstance()->FetchRenderEntity();

    for (Entity* item : entities) {
        std::string entityType = item->EntityType;

        auto it = entityStateMap.find(entityType);
        if (it == entityStateMap.end()) {
            continue;
        }

        EntityState entityState = it->second;
        std::string resStateId = entityState.MatchState(item);
        if (resStateId.empty()) {
            continue;
        }

        ResourceState resState = ResourceStateCfg::GetInstance().Get(resStateId);

        // 获取资源类型而非实体类型
        Resource res = ResourceCfg::GetInstance().Get(entityType);
        bool ok = ResourceManager::GetInstance()->Update(res.type, item, resState.property);
        if (!ok) {
            continue;
        }
    }
}

void StateManager::OnClose()
{
}

} // namespace TSEngine
