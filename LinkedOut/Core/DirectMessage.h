#pragma once

#include "Content.h"

namespace LinkedOut {
	class DirectMessage : public Content {
	public:
		DirectMessage() = default;

		const std::string& GetMessageID()const { return m_MessageID; }
		const std::string& GetToWho()const { return m_ToWho; }

		void SetMessageID(const std::string& messageID) { m_MessageID = messageID; }
		void SetToWho(const std::string& toWho) { m_ToWho = toWho; }
	private:
		std::string m_MessageID;
		std::string m_ToWho;
	};
}