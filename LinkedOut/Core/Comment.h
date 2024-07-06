#pragma once

#include "Content.h"

namespace LinkedOut {
	class Comment : public Content {
	public:
		Comment() = default;

		const std::string& GetPostID()const { return m_PostID; }
		const std::string& GetCommentID()const { return m_CommentID; }
		const std::string& GetCommenterName()const { return m_CommenterName; }

		void SetPostID(const std::string& postID) { m_PostID = postID; }
		void SetCommentID(const std::string& commentID) { m_CommentID = commentID; }
		void SetCommenterName(const std::string& name) { m_CommenterName = name; }


	private:
		std::string m_PostID;
		std::string m_CommentID;
		std::string m_CommenterName;
	};
}