#pragma once
#include "IResourceHandler.h"
#include <unordered_map>
#include "Engine/StreamableManager.h"
#include "../config/PrefabCfg.h"

namespace TSEngine {

class ResourceManager {
public:
    static ResourceManager* GetInstance();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    void RegisterHandler(const std::string& Type, IResourceHandler* Handler);

    void RegisterPlayableHandler(const std::string& Type, IPlayableResourceHandler* Handler);

    bool Load(Entity* TsEntity, const Prefab& Prefab);

    bool Update(const std::string& Type, Entity* TsEntity,
               std::map<std::string, std::string> Property);

    bool Play(const std::string& Type, Entity* TsEntity,
             std::map<std::string, std::string> Params);

    bool Stop(const std::string& Type, Entity* TsEntity);

    bool Pause(const std::string& Type, Entity* TsEntity);

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
    FStreamableManager* StreamableManager;

    template<typename T>
    friend class TStaticResourceHandler;
};

} // namespace TSEngine
