#pragma once
#include "IResourceHandler.h"
#include "Engine/StreamableManager.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"

namespace TSEngine {

class SoundHandler : public IPlayableResourceHandler {
public:
    SoundHandler();

    virtual std::string GetType() const override { return "Sound"; }

    virtual void LoadAsync(const FSoftObjectPath& Path,
                          FResourceLoadedCallback Callback) override;

    virtual AActor* CreateActor(Entity* TsEntity, UObject* LoadedSound,
                                const std::map<std::string, std::string>& Properties);

    virtual void Play(Entity* TsEntity, UObject* Target,
                     const std::map<std::string, std::string>& Params) override;

    virtual void Stop(Entity* TsEntity, UObject* Target) override;
    virtual void Pause(Entity* TsEntity, UObject* Target) override;

    virtual void ApplyProperties(UObject* Target,
                                const std::map<std::string, std::string>& Properties) override;

private:
    void OnSoundLoaded(FSoftObjectPath Path, FResourceLoadedCallback Callback);
    UAudioComponent* GetAudioComponent(AActor* Actor);

    std::unordered_map<FSoftObjectPath, FResourceLoadedCallback> PendingLoads;
};

} // namespace TSEngine
