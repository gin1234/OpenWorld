#include "StateManagerWrap.h"
#include "Binding.hpp"
#include <queue>

using namespace TSEngine;
UsingCppType(StateManager);

struct AutoRegisterForStateManager {
    AutoRegisterForStateManager() {
        puerts::DefineClass<StateManager>()
            .Function("instance", MakeFunction(&StateManager::GetInstance))
            .Method("setTickCallback", MakeFunction(&StateManager::SetTickCallback))
            .Register();
    }
};

AutoRegisterForStateManager _AutoRegisterForStateManager__;
