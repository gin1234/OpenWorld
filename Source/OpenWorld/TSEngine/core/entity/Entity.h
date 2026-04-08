#pragma once
#include <string>
#include <unordered_map>
#include "Engine/Object.h"
#include "Engine/Actor.h"

namespace TSEngine {

enum class EntityLifeStatus : int {
    PreLoad,
    Loading,
    FailLoad,
    Rendering,
    PreDestroy,
    FailDestroy
};

class Entity {
public:
    int ID;
    std::string EntityType;
    std::string LastState;
    std::string CurrentState;
    std::string ResStateId;
    EntityLifeStatus Status;
    AActor* Actor;  // 关联的虚幻 Actor

    Entity() : Actor(nullptr), Status(EntityLifeStatus::PreLoad) {}

    void SetResource(const std::string& Type, UObject* Resource) {
        Resources[Type] = Resource;
    }

    UObject* GetResource(const std::string& Type) {
        auto it = Resources.find(Type);
        return (it != Resources.end()) ? it->second : nullptr;
    }

    void ClearResources() {
        Resources.clear();
    }

    void OnMessage();

private:
    std::unordered_map<std::string, UObject*> Resources;
};

} // namespace TSEngine
