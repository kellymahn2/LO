#pragma once
#include "Core/Layer.h"
#include "SignupLayer.h"
#include "LoginLayer.h"
#include "SplashLayer.h"
#include "ErrorLayer.h"
#include "NavigationMenu.h"
#include "UserInformationLayer.h"
#include "HomeLayer.h"
#include "Core/Account.h"
#include "Core/Person.h"
#include "Core/Company.h"

class QLayout;

class QWidget;
class QFrame;
class QLabel;
class QLabel;
class QPushButton;
class QPushButton;
class QValidator;
class QLineEdit;
class QVBoxLayout;

namespace LinkedOut {
	

	struct SuggestedUserPosts {
		std::vector<Ref<Post>> FollowingPosts;
		std::vector<Ref<Post>> SuggestedPosts;
		std::vector<Ref<Post>> RandomPosts;
	};


	//TODO: change layers to super static classes.

	struct Message {
		std::string Error;
	};

	class MainLayer{
	public:
		MainLayer();
		virtual void OnAttach() ;
		virtual void OnDetach() ;
		virtual void OnUpdate() ;

		static MainLayer& Get() { return *s_Instance; }

	private:
		UserInternalData GetUserDataStoredLocally();
	private:
		void SwitchToSignup(bool cleanAll);
		void SwitchToLogin(bool cleanAll);
		void SwitchToUserInformation();
		void SwitchToHome();
	private:

		Ref<Person> GetPerson(const std::string& id);
		Ref<Company> GetCompany(const std::string& id);
		Ref<Job> GetJob(const std::string& id);
		Ref<Post> GetPost(const std::string& id);

		Ref<Person> QueryPerson(const std::string& id, Ref<Person> person);
		Ref<Company> QueryCompany(const std::string& id, Ref<Company> company);
		Ref<Job> QueryJob(const std::string& id, Ref<Job> job);
		Ref<Post> QueryPost(const std::string& id, Ref<Post> post);

		void LikePost(Ref<Post> post);
		void RepostPost(Ref<Post> post);
		void CommentOnPost(Ref<Post> post, const std::string& text, const std::string& picture, const std::string& video);
		void Follow(const std::string& id);

		std::vector<Ref<Person>> GetUserSuggestions(Ref<Person> person);
		std::vector<Ref<Person>> GetUserRequests(Ref<Person> person);
		std::vector<Ref<Person>> GetCompanyFollowers(Ref<Company> company);
		
		std::vector<Ref<Job>> GetUserJobs(Ref<Person> person);

		std::vector<Ref<Post>> GetSuggestedUserPostsFromFollowed(Ref<Person> person);
		std::vector<Ref<Post>> GetSuggestedUserPostsFromSameCompany(Ref<Person> person);
		std::vector<Ref<Post>> GetSuggestedUserPostsFromRandom(Ref<Person> person);

		SuggestedUserPosts GetSuggestedUserPosts(Ref<Person> person);

		//std::vector<CommentSpecification> GetPostComments(Post& post);

		SignupErrorCodes Signup(const UserInternalData& userData);
		LoginErrorCodes Login(const UserInternalData& userData,bool storeLocally);
		UserInformationStoreErrorCodes StoreUserInformation(const UserData& userData);

		void StoreUserDataLocally(const UserInternalData& userData);

		bool UserExists(const std::string& username);
		UserInternalData GetUserDataFromUsername(const std::string& userName);
		UserData GetUserInformation(uint32_t userID);
		uint32_t GetUniqueUserID(const std::string& userName);
		void SetCurrentLayer(Layer* layer);
	private:
		UserInternalData m_UserData;

		QWidget* m_WindowCentralWidget;
		QVBoxLayout* m_WindowCentralLayout;

		Layer* m_CurrentLayer = nullptr;
		
		uint32_t m_LastCaptchaNumber;

		QVBoxLayout* m_LayersLayout;
		QWidget* m_LayersWidget;

		SignupLayer* m_SignupLayer;
		LoginLayer* m_LoginLayer;
		SplashLayer* m_SplashLayer;
		MessageLayer* m_MessageLayer;
		NavigationMenu* m_NavigationMenu;
		UserInformationLayer* m_UserInformationLayer;
		HomeLayer* m_HomeLayer;

		std::unordered_map<std::string, Ref<Person>> m_IDToPersonMap;
		std::unordered_map<std::string, Ref<Company>> m_IDToCompanyMap;
		std::unordered_map<std::string, Ref<Job>> m_IDToJobMap;
		std::unordered_map<std::string, Ref<Post>> m_IDToPostMap;

		Ref<Account> m_CurrentUser;
		
		static MainLayer* s_Instance;
		friend class SignupLayer;
		friend class LoginLayer;
		friend class SplashLayer;
		friend class MessageLayer;
		friend class UserInformationLayer;
		friend class NavigationMenu;
		friend class HomeLayer;
		friend class NetworkUser;
		friend class NetworkComany;
		friend class JobsUser;
		friend class JobsCompany;
		friend class PostUI;
		friend CommentsLayer;
	};
}