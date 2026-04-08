#pragma once
#include "IResourceHandler.h"
#include "Engine/StreamableManager.h"
#include "Engine/SkeletalMeshActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

namespace TSEngine {

class SkeletalMeshHandler : public IPlayableResourceHandler {
public:
    SkeletalMeshHandler();

    virtual std::string GetType() const override { return "SkeletalMesh"; }

    virtual void LoadAsync(const FSoftObjectPath& Path,
                          FResourceLoadedCallback Callback) override;

    virtual AActor* CreateActor(Entity* TsEntity, UObject* LoadedMesh,
                                const std::map<std::string, std::string>& Properties) override;

    // 统一 Update 接口 - 根据 Operation 分发到 Play/Stop
    virtual bool Update(Entity* TsEntity, UObject* Target,
                       std::map<std::string, std::string> Params) override;

    virtual void ApplyProperties(UObject* Target,
                                const std::map<std::string, std::string>& Properties) override;

private:
    USkeletalMeshComponent* GetMeshComponent(AActor* Actor);
    UAnimInstance* GetAnimInstance(AActor* Actor);

    void OnMeshLoaded(FSoftObjectPath Path, FResourceLoadedCallback Callback);

    std::unordered_map<FSoftObjectPath, FResourceLoadedCallback> PendingLoads;
};

} // namespace TSEngine
