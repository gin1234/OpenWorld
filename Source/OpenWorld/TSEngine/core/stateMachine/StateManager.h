#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <queue>
#include "EntityState.h"
#include "../entity/Entity.h"
#include "../transport/Message.h"

namespace TSEngine {
    class StateManager {
    public:
        // 设置 TS RootEntity 的 tick 回调
        // TS 调用: StateManager.Instance().SetTickCallback((messages) => { rootEntity.tick(messages); })
        void SetTickCallback(std::function<void()> callback);

        void Update();

    private:
        int loadCount;
        int renderCount;
        int closeCount;
        std::vector<Entity*> loadingEntities;

        std::unordered_map<std::string, EntityState> entityStateMap;
        std::unordered_map<std::string, std::unordered_map<std::string, int>> entityResMap;

        // TS RootEntity 的 tick 回调
        std::function<void()> TickCallback;

        void OnLoad();
        void OnRender();
        void OnClose();
    };
}
