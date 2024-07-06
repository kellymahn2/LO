#pragma once

#include "Time.h"

namespace LinkedOut {
	class Like {
	public:
		Like() = default;

		const std::string& GetLikedBy()const { return m_LikedBy; }
		const std::string& GetLikeID()const { return m_LikeID; }
		const Time& GetLikedAt()const { return m_LikedAt; }

		void SetLikedBy(const std::string& likedBy) { m_LikedBy = likedBy; }
		void SetLikeID(const std::string& likeID) { m_LikeID = likeID; }
		void SetLikedAt(const Time& time) { m_LikedAt = time; }

	private:
		std::string m_LikedBy;
		std::string m_LikeID;
		Time m_LikedAt;
	};
}