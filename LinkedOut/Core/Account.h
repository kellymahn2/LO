#pragma once

#include "Core/Base.h"

#include "Post.h"

#include "DirectMessage.h"


#include <string>
#include <vector>
#include <unordered_map>

class QWidget;

namespace LinkedOut {
    class HomeLayerUI;
    class JobPanelUI;
    class NetworkPanelUI;
    class MessagePanelUI;
    class ProfilePanelUI;
    class SignupLayerUI;
    class LoginLayerUI;
    class OtherProfilePanelUI;

    enum class AccountType {
        None = 0,
        Person,
        Company
    };

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

        std::string GetImagePath()const { return m_ImagePath; }

        void SetImagePath(const std::string& path) { m_ImagePath = path; }


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

        const std::unordered_map<std::string, std::vector<Ref<DirectMessage>>>& GetDirectMessages() const {
            return m_DirectMessages;
        }

        void SetDirectMessages(const  std::unordered_map<std::string, std::vector<Ref<DirectMessage>>>& directMessages) {
            m_DirectMessages = directMessages;
        }
        const std::string& GetUsername() const { return m_Username; }
        const std::string& GetPassword() const{return m_Password;}
        void SetUsername(const std::string& Username){ m_Username = Username;}
        void SetPassword(const std::string& Password) { m_Password = Password; }

        virtual void Follow(const std::string& id) {}

        virtual AccountType GetAccountType()const = 0;
        virtual void Serialize(QWidget* parent) {}

        virtual HomeLayerUI* MakeHomeLayer(QWidget* parent) = 0;
        virtual JobPanelUI* MakeJobLayer(QWidget* parent) = 0;
        virtual NetworkPanelUI* MakeNetworkLayer(QWidget* parent) = 0;
        virtual MessagePanelUI* MakeMessagePanel(QWidget* parent) { return nullptr; }
        virtual ProfilePanelUI* MakeProfilePanelUI(bool currentUser, QWidget* parent) = 0;
        virtual OtherProfilePanelUI* MakeOtherProfilePanelUI(QWidget* parent) = 0;
        virtual LoginLayerUI* MakeLoginUI(QWidget* parent) = 0;
        virtual SignupLayerUI* MakeSignupUI(QWidget* parent) = 0;

        virtual void Post(Ref<LinkedOut::Post> post) { m_Posts.push_back(post); }
        virtual void DM(Ref<DirectMessage> dm, const std::string& id) { m_DirectMessages[id].push_back(dm); }
    
	protected:
        std::string m_Username;
        std::string m_Password;
		std::string m_AccountID;
		std::string m_PhoneNumber;
		std::string Email;
		std::vector<std::string> m_FollowingID;
		std::vector<Ref<LinkedOut::Post>> m_Posts;
        std::unordered_map<std::string, std::vector<Ref<DirectMessage>>> m_DirectMessages;

        std::string m_ImagePath;
	};
}