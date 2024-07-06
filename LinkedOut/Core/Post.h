#pragma once
#include "Content.h"
#include "Like.h"
#include "Comment.h"
#include <vector>

namespace LinkedOut {
	class Post : public Content {
	public:
		Post() = default;

		const std::string& GetPostID()const { return m_PostID; }
		const uint32_t& GetRepostCounter()const { return m_RepostCounter;}
		const std::vector<Like>& GetLikes()const { return m_Likes;}
		const std::vector<Ref<Comment>>& GetComments()const { return m_Comments;}
		const std::string& GetPosterName()const { return m_PosterName; }

		void SetPostID(const std::string& PostID){m_PostID = PostID;}
		void SetRepostCounter(const uint32_t& RepostCounter){m_RepostCounter = RepostCounter;}
		void SetLikes(const std::vector<Like>& Likes){m_Likes = Likes;}
		void SetComments(const std::vector<Ref<Comment>>& Comments) { m_Comments = Comments; }
		void SetPosterName(const std::string& name) { m_PosterName = name; }

		void LikePost(Like& like) {
			m_Likes.push_back(like);
		}

	private:
		std::string m_PosterName;
		std::string m_PostID;
		uint32_t m_RepostCounter;
		std::vector<Like> m_Likes;
		std::vector<Ref<Comment>> m_Comments;
	};
}