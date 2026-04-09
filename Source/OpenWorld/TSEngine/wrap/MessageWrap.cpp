#include <MessageWrap.h>

#include "Binding.hpp"
#include <queue>

using namespace TSEngine;
UsingCppType(Message);
UsingCppType(MessageManager);
UsingCppType(std::queue<Message>);

struct AutoRegisterForMessage
{
    AutoRegisterForMessage()
    {
        puerts::DefineClass<Message>()
            .Property("mailId", MakeProperty(&Message::mailId))
            .Property("param", MakeProperty(&Message::param))
            .Method("sendMessage", MakeFunction(&MessageManager::sendMessage))
            .Function("instance", MakeFunction(&MessageManager::getInstance))
            .Method("fetchMessage", MakeFunction(&MessageManager::fetchMessage))
            .Register();
    }
};

AutoRegisterForMessage _AutoRegisterForMessage__;
