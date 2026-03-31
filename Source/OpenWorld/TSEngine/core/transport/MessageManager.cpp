#include <core/transport/MessageManager.h>

namespace TSEngine {
	MessageManager* MessageManager::getInstance()
	{
		static MessageManager instance;
		return &instance;
	}
	bool MessageManager::sendMessage(Message msg)
	{	
		mailBox.push(msg);
		return true;
	}

	std::queue<Message> MessageManager::fetchMessage()
	{	
		return mailBox.swap();
	}

}