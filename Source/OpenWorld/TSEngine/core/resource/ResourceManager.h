#pragma once
#include "IResourceHandler.h"
#include <unordered_map>
#include <functional>
#include "Engine/StreamableManager.h"
#include "../config/PrefabCfg.h"
#include "PropertyParser.h"

namespace TSEngine {

class Entity;

class ResourceManager {
public:
    static ResourceManager* GetInstance();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    void RegisterHandler(const std::string& Type, IResourceHandler* Handler);

    void RegisterPlayableHandler(const std::string& Type, IPlayableResourceHandler* Handler);

    // 加载资源，onLoaded 回调会在加载完成后被调用
    bool Load(Entity* TsEntity, const Prefab& Prefab,
             std::function<void(Entity*)> OnLoaded = nullptr);

    // 统一 Update - 根据资源类型分发到对应处理器
    bool Update(const std::string& Type, Entity* TsEntity,
               std::map<std::string, std::string> Property);

    // 释放实体资源，销毁关联的 Actor
    void ReleaseEntity(Entity* TsEntity);

    // 销毁指定类型的 Actor
    void DestroyActor(Entity* TsEntity, const std::string& Type);

    UObject* GetLoadedResource(int32 EntityId, const std::string& Type);

    void ReleaseEntityResources(int32 EntityId);

    FStreamableManager* GetStreamableManager() { return StreamableManager; }

    ~ResourceManager();

private:
    ResourceManager();
    void NotifyLoadSuccess(Entity* TsEntity, const Prefab& Prefab, UObject* LoadedObject);
    void NotifyLoadFailed(Entity* TsEntity);

    std::unordered_map<std::string, IResourceHandler*> Handlers;
    std::unordered_map<std::string, IPlayableResourceHandler*> PlayableHandlers;
    std::unordered_map<int32, std::unordered_map<std::string, UObject*>> LoadedResources;
    std::unordered_map<int32, std::unordered_map<std::string, AActor*>> EntityActors;
    FStreamableManager* StreamableManager;

    template<typename T>
    friend class TStaticResourceHandler;
};

} // namespace TSEngine
