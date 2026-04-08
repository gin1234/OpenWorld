#pragma once
#include <string>
#include <map>
#include <functional>
#include "Engine/StreamableManager.h"
#include "Engine/Object.h"
#include "Engine/Actor.h"
#include "../entity/Entity.h"

namespace TSEngine {

struct FResourceLoadedCallback {
    std::function<void(UObject*)> OnLoaded;
    std::function<void()> OnFailed;
};

// 基础资源处理器接口
class IResourceHandler {
public:
    virtual ~IResourceHandler() = default;

    virtual std::string GetType() const = 0;

    virtual void LoadAsync(const FSoftObjectPath& Path, FResourceLoadedCallback Callback) = 0;

    virtual void ApplyProperties(UObject* Target, const std::map<std::string, std::string>& Properties) {}
};

// 仅加载类型接口 (StaticMesh) - 加载完成后创建 Actor
class ILoadOnlyResourceHandler : public IResourceHandler {
public:
    virtual AActor* CreateActor(Entity* TsEntity, UObject* LoadedObject,
                                const std::map<std::string, std::string>& Properties) = 0;
};

// 可播放资源接口 (骨骼动画、声音、粒子)
class IPlayableResourceHandler : public IResourceHandler {
public:
    virtual void Play(Entity* TsEntity, UObject* Target,
                     const std::map<std::string, std::string>& Params) = 0;
    virtual void Stop(Entity* TsEntity, UObject* Target) {}
    virtual void Pause(Entity* TsEntity, UObject* Target) {}
};

// 可更新类型接口 (UI) - 资源常驻内存，支持属性更新
class IUpdatableResourceHandler : public IResourceHandler {
public:
    virtual bool Update(Entity* TsEntity, UObject* Target,
                       std::map<std::string, std::string> Property) = 0;
};

} // namespace TSEngine
