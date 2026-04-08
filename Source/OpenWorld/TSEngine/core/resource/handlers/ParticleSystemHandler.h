#pragma once
#include "IResourceHandler.h"
#include "Engine/StreamableManager.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"

namespace TSEngine {

class ParticleSystemHandler : public IPlayableResourceHandler {
public:
    ParticleSystemHandler();

    virtual std::string GetType() const override { return "ParticleSystem"; }

    virtual void LoadAsync(const FSoftObjectPath& Path,
                          FResourceLoadedCallback Callback) override;

    virtual AActor* CreateActor(Entity* TsEntity, UObject* LoadedPS,
                                const std::map<std::string, std::string>& Properties) override;

    // 统一 Update 接口 - 根据 Operation 分发到 Play/Stop
    virtual bool Update(Entity* TsEntity, UObject* Target,
                       std::map<std::string, std::string> Params) override;

    virtual void ApplyProperties(UObject* Target,
                                const std::map<std::string, std::string>& Properties) override;

private:
    void OnParticleLoaded(FSoftObjectPath Path, FResourceLoadedCallback Callback);
    UParticleSystemComponent* GetParticleComponent(AActor* Actor);

    std::unordered_map<FSoftObjectPath, FResourceLoadedCallback> PendingLoads;
};

} // namespace TSEngine
