#include "StateManager.h"
#include <TypeScriptEntityManager.h>
#include <ResourceCfg.h>
#include <ResourceStateCfg.h>
#include <PrefabCfg.h>
#include <resource/ResourceManager.h>
#include <transport/MessageManager.h>

namespace TSEngine {

void StateManager::SetTickCallback(std::function<void()> callback)
{
    TickCallback = callback;
}

void StateManager::Update()
{
    OnClose();
    OnLoad();
    OnRender();

    // 通知 TS 进行消息处理
    // TS 会在 callback 内部调用 MessageManager.Instance().FetchMessage() 获取消息
    // 然后调用 rootEntity.tick(messages)
    if (TickCallback) {
        TickCallback();
    }
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

        Resource res = ResourceCfg::GetInstance()->Get(entityType);
        Prefab prefab = PrefabCfg::GetInstance()->Get(res.prefabId);

        // 传入回调，加载完成后自动加入渲染队列
        bool ok = ResourceManager::GetInstance()->Load(item, prefab,
            [this](Entity* loadedEntity) {
                if (loadedEntity && loadedEntity->Status == EntityLifeStatus::Rendering) {
                    EntityManager::GetInstance()->AddRenderEntity(loadedEntity);
                }
            });

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
        // 检查实体是否已加载完成
        if (item->Status != EntityLifeStatus::Rendering) {
            continue;
        }

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

        ResourceState resState = ResourceStateCfg::GetInstance()->Get(resStateId);
        Resource res = ResourceCfg::GetInstance().Get(entityType);
        Prefab prefab = PrefabCfg::GetInstance().Get(res.prefabId);
        bool ok = ResourceManager::GetInstance()->Update(prefab.prefabType, item, resState.property);
        if (!ok) {
            continue;
        }
    }
}

void StateManager::OnClose()
{
    std::vector<Entity*> entities = EntityManager::GetInstance()->FetchDestroyEntity();

    for (Entity* item : entities) {
        // 释放虚幻资源
        ResourceManager::GetInstance()->ReleaseEntity(item);

        // 从 EntityManager 中移除
        EntityManager::GetInstance()->RemoveEntity(item->ID);

        UE_LOG(LogTemp, Log, TEXT("StateManager::OnClose - Destroyed entity %d"), item->ID);
    }
}

} // namespace TSEngine
