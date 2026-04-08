#include "ResourceManagerWrap.h"
#include "Binding.hpp"
#include <map>

using namespace TSEngine;
UsingCppType(ResourceManager);
UsingCppType(std::map<std::string, std::string>);

struct AutoRegisterForResourceManager {
    AutoRegisterForResourceManager() {
        puerts::DefineClass<ResourceManager>()
            .Function("instance", MakeFunction(&ResourceManager::GetInstance))
            .Function("load", MakeFunction(&ResourceManager::Load))
            .Function("update", MakeFunction(&ResourceManager::Update))
            .Register();
    }
};

AutoRegisterForResourceManager _AutoRegisterForResourceManager__;
