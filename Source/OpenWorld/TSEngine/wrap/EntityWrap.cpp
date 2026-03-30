#include "EntityWrap.h"
#include "Binding.hpp"
#include "string"
using namespace TSEngine;
UsingCppType(EntityManager);
struct AutoRegisterForEntityManager
{
    AutoRegisterForEntityManager()
    {
        puerts::DefineClass<EntityManager>()
            .Method("createEntity", MakeFunction(&EntityManager::CreateEntity))
            .Method("destroyEntity", MakeFunction(&EntityManager::PreDestroyEntity))
            .Function("instance", MakeFunction(&EntityManager::GetInstance))
            .Register();
    }
};

AutoRegisterForEntityManager _AutoRegisterForEntityManager__;