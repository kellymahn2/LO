#pragma once

#include <string>
#include "Time.h"

namespace LinkedOut {
	class Content {
	public:
		Content() = default;
		virtual ~Content() = default;

		Content(const std::string& SenderID, const Time& TimeSent, const std::string& ContentText, const std::string& ContentPicture, const std::string& ContentVideo)
			: m_SenderID(SenderID), m_TimeSent(TimeSent), m_ContentText(ContentText), m_ContentPicture(ContentPicture), m_ContentVideo(ContentVideo)
		{
		}
		const std::string& GetSenderID() const{ return m_SenderID;}
		const Time& GetTimeSent() const{ return m_TimeSent;}
		const std::string& GetContentText() const{ return m_ContentText;}
		const std::string& GetContentPicture() const{ return m_ContentPicture;}
		const std::string& GetContentVideo() const {  return m_ContentVideo; }


		void SetSenderID(const std::string& SenderID) { m_SenderID = SenderID; }
		void SetTimeSent(const Time& TimeSent){ m_TimeSent = TimeSent;}
		void SetContentText(const std::string& ContentText){ m_ContentText = ContentText;}
		void SetContentPicture(const std::string& ContentPicture){ m_ContentPicture = ContentPicture;}
		void SetContentVideo(const std::string& ContentVideo){ m_ContentVideo = ContentVideo;}

	protected:
		std::string m_SenderID;
		Time m_TimeSent;
		std::string m_ContentText;
		std::string m_ContentPicture;
		std::string m_ContentVideo;
	};

}