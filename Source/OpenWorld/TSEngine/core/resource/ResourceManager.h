#pragma once
#include "IResourceHandler.h"
#include <unordered_map>
#include "Engine/StreamableManager.h"
#include "../config/PrefabCfg.h"
#include "PropertyParser.h"

namespace TSEngine {

class ResourceManager {
public:
    static ResourceManager* GetInstance();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    void RegisterHandler(const std::string& Type, IResourceHandler* Handler);

    void RegisterPlayableHandler(const std::string& Type, IPlayableResourceHandler* Handler);

    bool Load(Entity* TsEntity, const Prefab& Prefab);

    // 统一 Update - 根据资源类型分发到对应处理器
    // SkeletalMesh: Params = {"Operation": "Play", "AnimationName": "Run", ...}
    // Sound: Params = {"Operation": "Play", "VolumeMultiplier": "1.0", ...} 或 Stop/Pause
    // ParticleSystem: Params = {"Operation": "Play"} 或 Stop
    // Widget: Params = {"Visibility": "Hidden", "RenderOpacity": "0.5"}
    bool Update(const std::string& Type, Entity* TsEntity,
               std::map<std::string, std::string> Property);

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
