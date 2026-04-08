#include "ResourceManager.h"
#include "../entity/Entity.h"

namespace TSEngine {

ResourceManager::ResourceManager() {
    StreamableManager = new FStreamableManager();
}

ResourceManager::~ResourceManager() {
    if (StreamableManager) {
        delete StreamableManager;
        StreamableManager = nullptr;
    }
}

void ResourceManager::RegisterHandler(const std::string& Type, IResourceHandler* Handler) {
    if (Handler) {
        Handlers[Type] = Handler;
    }
}

void ResourceManager::RegisterPlayableHandler(const std::string& Type, IPlayableResourceHandler* Handler) {
    if (Handler) {
        PlayableHandlers[Type] = Handler;
        Handlers[Type] = Handler; // Also register in base handlers map
    }
}

bool ResourceManager::Load(Entity* TsEntity, const Prefab& Prefab) {
    auto it = Handlers.find(Prefab.prefabType);
    if (it == Handlers.end()) {
        UE_LOG(LogTemp, Warning, TEXT("No handler for resource type: %s"),
               UTF8_TO_TCHAR(Prefab.prefabType.c_str()));
        return false;
    }

    IResourceHandler* Handler = it->second;
    FSoftObjectPath Path(UTF8_TO_TCHAR(Prefab.path.c_str()));

    FResourceLoadedCallback Callback;
    Callback.OnLoaded = [this, TsEntity, Prefab, Handler](UObject* LoadedObject) {
        NotifyLoadSuccess(TsEntity, Prefab, LoadedObject);
    };

    Callback.OnFailed = [this, TsEntity]() {
        NotifyLoadFailed(TsEntity);
    };

    Handler->LoadAsync(Path, Callback);
    TsEntity->Status = EntityLifeStatus::Loading;

    return true;
}

void ResourceManager::NotifyLoadSuccess(Entity* TsEntity, const Prefab& Prefab, UObject* LoadedObject) {
    if (!TsEntity || !LoadedObject) {
        return;
    }

    auto HandlerIt = Handlers.find(Prefab.prefabType);
    if (HandlerIt == Handlers.end()) {
        return;
    }

    IResourceHandler* Handler = HandlerIt->second;

    // Check if it's a playable resource handler first (SkeletalMesh, Sound, ParticleSystem)
    IPlayableResourceHandler* PlayableHandler = dynamic_cast<IPlayableResourceHandler*>(Handler);
    if (PlayableHandler) {
        AActor* Actor = PlayableHandler->CreateActor(TsEntity, LoadedObject, {});
        if (Actor) {
            TsEntity->Status = EntityLifeStatus::Rendering;
            LoadedResources[TsEntity->ID][Prefab.prefabType] = LoadedObject;
        } else {
            TsEntity->Status = EntityLifeStatus::FailLoad;
        }
        return;
    }

    // Check if it's a load-only handler (StaticMesh)
    ILoadOnlyResourceHandler* LoadOnlyHandler = dynamic_cast<ILoadOnlyResourceHandler*>(Handler);
    if (LoadOnlyHandler) {
        AActor* Actor = LoadOnlyHandler->CreateActor(TsEntity, LoadedObject, {});
        if (Actor) {
            TsEntity->Status = EntityLifeStatus::Rendering;
            LoadedResources[TsEntity->ID][Prefab.prefabType] = LoadedObject;
        } else {
            TsEntity->Status = EntityLifeStatus::FailLoad;
        }
        return;
    }

    // For other handlers (like Widget), just store the resource
    LoadedResources[TsEntity->ID][Prefab.prefabType] = LoadedObject;
    TsEntity->Status = EntityLifeStatus::Rendering;
}

void ResourceManager::NotifyLoadFailed(Entity* TsEntity) {
    if (TsEntity) {
        TsEntity->Status = EntityLifeStatus::FailLoad;
    }
}

bool ResourceManager::Update(const std::string& Type, Entity* TsEntity,
                            std::map<std::string, std::string> Property) {
    auto it = Handlers.find(Type);
    if (it == Handlers.end()) {
        return false;
    }

    IUpdatableResourceHandler* UpdatableHandler =
        dynamic_cast<IUpdatableResourceHandler*>(it->second);

    if (!UpdatableHandler) {
        return false;
    }

    UObject* Resource = GetLoadedResource(TsEntity->ID, Type);
    if (!Resource) {
        return false;
    }

    return UpdatableHandler->Update(TsEntity, Resource, Property);
}

bool ResourceManager::Play(const std::string& Type, Entity* TsEntity,
                          std::map<std::string, std::string> Params) {
    auto it = PlayableHandlers.find(Type);
    if (it == PlayableHandlers.end()) {
        return false;
    }

    UObject* Resource = GetLoadedResource(TsEntity->ID, Type);
    if (!Resource) {
        return false;
    }

    it->second->Play(TsEntity, Resource, Params);
    return true;
}

bool ResourceManager::Stop(const std::string& Type, Entity* TsEntity) {
    auto it = PlayableHandlers.find(Type);
    if (it == PlayableHandlers.end()) {
        return false;
    }

    UObject* Resource = GetLoadedResource(TsEntity->ID, Type);
    if (!Resource) {
        return false;
    }

    it->second->Stop(TsEntity, Resource);
    return true;
}

bool ResourceManager::Pause(const std::string& Type, Entity* TsEntity) {
    auto it = PlayableHandlers.find(Type);
    if (it == PlayableHandlers.end()) {
        return false;
    }

    UObject* Resource = GetLoadedResource(TsEntity->ID, Type);
    if (!Resource) {
        return false;
    }

    it->second->Pause(TsEntity, Resource);
    return true;
}

UObject* ResourceManager::GetLoadedResource(int32 EntityId, const std::string& Type) {
    auto entityIt = LoadedResources.find(EntityId);
    if (entityIt == LoadedResources.end()) {
        return nullptr;
    }

    auto typeIt = entityIt->second.find(Type);
    if (typeIt == entityIt->second.end()) {
        return nullptr;
    }

    return typeIt->second;
}

void ResourceManager::ReleaseEntityResources(int32 EntityId) {
    auto it = LoadedResources.find(EntityId);
    if (it != LoadedResources.end()) {
        for (auto& [Type, Resource] : it->second) {
            if (Resource) {
                Resource->RemoveFromRoot();
            }
        }
        LoadedResources.erase(it);
    }
}

} // namespace TSEngine
