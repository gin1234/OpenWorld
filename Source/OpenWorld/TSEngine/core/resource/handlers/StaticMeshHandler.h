#pragma once
#include "IResourceHandler.h"
#include "Engine/StreamableManager.h"
#include "Engine/StaticMeshActor.h"

namespace TSEngine {

class StaticMeshHandler : public ILoadOnlyResourceHandler {
public:
    StaticMeshHandler();

    virtual std::string GetType() const override { return "StaticMesh"; }

    virtual void LoadAsync(const FSoftObjectPath& Path,
                          FResourceLoadedCallback Callback) override;

    virtual AActor* CreateActor(Entity* TsEntity, UObject* LoadedObject,
                                const std::map<std::string, std::string>& Properties) override;

    virtual void ApplyProperties(UObject* Target,
                                const std::map<std::string, std::string>& Properties) override;

private:
    void OnMeshLoaded(FSoftObjectPath Path, FResourceLoadedCallback Callback);

    std::unordered_map<FSoftObjectPath, FResourceLoadedCallback> PendingLoads;
};

} // namespace TSEngine
