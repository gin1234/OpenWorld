#pragma once
#include <string>

namespace TSEngine {

class ResourceManager;

template<typename T>
class TStaticResourceHandler {
public:
    TStaticResourceHandler(const std::string& TypeName) {
        ResourceManager::GetInstance()->RegisterHandler(TypeName, new T());
    }
};

template<typename T>
class TStaticPlayableResourceHandler {
public:
    TStaticPlayableResourceHandler(const std::string& TypeName) {
        ResourceManager::GetInstance()->RegisterPlayableHandler(TypeName, new T());
    }
};

} // namespace TSEngine
