#pragma once

#include "Core/Base.h"

#include "Post.h"

#include "DirectMessage.h"

#include <string>
#include <vector>

namespace LinkedOut {
	class Account {
	public:

        Account() = default;

        virtual ~Account() = default;

        std::string GetAccountID() const {
            return m_AccountID;
        }

        void SetAccountID(const std::string& accountID) {
            m_AccountID = accountID;
        }

        std::string GetPhoneNumber() const {
            return m_PhoneNumber;
        }

        void SetPhoneNumber(const std::string& phoneNumber) {
            m_PhoneNumber = phoneNumber;
        }

        std::string GetEmail() const {
            return Email;
        }

        void SetEmail(const std::string& email) {
            Email = email;
        }

        std::vector<std::string> GetFollowingID() const {
            return m_FollowingID;
        }

        void SetFollowingID(const std::vector<std::string>& followingID) {
            m_FollowingID = followingID;
        }

        std::vector<Ref<Post>> GetPosts() const {
            return m_Posts;
        }

        void SetPosts(const std::vector<Ref<Post>>& posts) {
            m_Posts = posts;
        }

        std::vector<Ref<DirectMessage>> GetDirectMessages() const {
            return m_DirectMessages;
        }

        void SetDirectMessages(const std::vector<Ref<DirectMessage>>& directMessages) {
            m_DirectMessages = directMessages;
        }
        const std::string& GetUsername() const { return m_Username; }
        const std::string& GetPassword() const{return m_Password;}
        void SetUsername(const std::string& Username){ m_Username = Username;}
        void SetPassword(const std::string& Password) { m_Password = Password; }

        virtual void Follow(const std::string& id) {}

	protected:
        std::string m_Username;
        std::string m_Password;
		std::string m_AccountID;
		std::string m_PhoneNumber;
		std::string Email;
		std::vector<std::string> m_FollowingID;
		std::vector<Ref<Post>> m_Posts;
		std::vector<Ref<DirectMessage>> m_DirectMessages;
	};
}