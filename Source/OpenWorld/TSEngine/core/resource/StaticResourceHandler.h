#pragma once
#include "IResourceHandler.h"

namespace TSEngine {

class ResourceManager;

template<typename T>
class TStaticResourceHandler {
public:
    TStaticResourceHandler(const std::string& TypeName) {
        ResourceManager::GetInstance()->RegisterHandler(TypeName, new T());
    }
};

} // namespace TSEngine
