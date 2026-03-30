#pragma once
#include "SwapQueue.h"
#include "Message.h"
#include "queue"

namespace TSEngine {
	class MessageManager {
	public:
		MessageManager(const MessageManager&) = delete;
		MessageManager& operator=(const MessageManager&) = delete;
		static MessageManager* getInstance();

		bool sendMessage(Message msg);

		std::queue<Message> fetchMessage();
	protected:
		MessageManager() = default;
		virtual ~MessageManager() = default;

	private:
		SwapQueue<Message> mailBox;
	};
}