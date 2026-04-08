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
                                const std::map<std::string, std::string>& Properties) override;

    // 统一 Update 接口 - 根据 Operation 分发到 Play/Stop/Pause
    virtual bool Update(Entity* TsEntity, UObject* Target,
                       std::map<std::string, std::string> Params) override;

    virtual void ApplyProperties(UObject* Target,
                                const std::map<std::string, std::string>& Properties) override;

private:
    void OnSoundLoaded(FSoftObjectPath Path, FResourceLoadedCallback Callback);
    UAudioComponent* GetAudioComponent(AActor* Actor);

    std::unordered_map<FSoftObjectPath, FResourceLoadedCallback> PendingLoads;
};

} // namespace TSEngine
