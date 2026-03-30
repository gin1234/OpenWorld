#include <MessageWrap.h>

#include "Binding.hpp"
using namespace TSEngine;
UsingCppType(Message);
UsingCppType(MessageManager);
struct AutoRegisterForMessage
{
    AutoRegisterForMessage()
    {
        puerts::DefineClass<Message>()
            .Property("mailId", MakeProperty(&Message::mailId))
            .Method("sendMessage", MakeFunction(&MessageManager::sendMessage))
            .Function("instance", MakeFunction(&MessageManager::getInstance))
            .Register();
    }
};

AutoRegisterForMessage _AutoRegisterForMessage__;