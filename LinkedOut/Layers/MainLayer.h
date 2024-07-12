#pragma once
#include "Core/Layer.h"
#include "SignupLayer.h"
#include "LoginLayer.h"
#include "SplashLayer.h"
#include "ErrorLayer.h"
#include "NavigationMenu.h"
#include "UserInformationLayer.h"
#include "HomeLayer.h"
#include "JobPanel.h"
#include "NetworkPanel.h"
#include "MessagePanel.h"
#include "ProfilePanel.h"
#include "OtherProfilePanel.h"

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

	struct CompanyCreatedJob {
		std::vector<Ref<Person>> PeopleRequested;
		Ref<Job> Job;
	};

	struct PersonJobs {
		Ref<Job> Job;
		enum class State{
			None = 0,
			Pending,
		};
		State JobState;
	};

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

		template<typename...Args>
		void Info(const std::string& format, Args&&...args) {
			m_MessageLayer->Info(format, std::forward<Args>(args)...);
		}

		template<typename...Args>
		void Warn(const std::string& format, Args&&...args) {
			m_MessageLayer->Warn(format, std::forward<Args>(args)...);
		}

		template<typename...Args>
		void Error(const std::string& format, Args&&...args) {
			m_MessageLayer->Error(format, std::forward<Args>(args)...);
		}

		template<typename...Args>
		void InfoTimed(float aliveDuration, const std::string& format, Args&&...args) {
			m_MessageLayer->InfoTimed(aliveDuration,format, std::forward<Args>(args)...);
		}

		template<typename...Args>
		void WarnTimed(float aliveDuration, const std::string& format, Args&&...args) {
			m_MessageLayer->WarnTimed(aliveDuration,format, std::forward<Args>(args)...);
		}
		template<typename...Args>
		void ErrorTimed(float aliveDuration, const std::string& format, Args&&...args) {
			m_MessageLayer->ErrorTimed(aliveDuration,format, std::forward<Args>(args)...);
		}

		template<typename...Args>
		void InfoInfinite(const std::string& format, Args&&...args) {
			m_MessageLayer->InfoInfinite(format, std::forward<Args>(args)...);
		}

		template<typename...Args>
		void WarnInfinite(const std::string& format, Args&&...args) {
			m_MessageLayer->WarnInfinite(format, std::forward<Args>(args)...);
		}

		template<typename...Args>
		void ErrorInfinite(const std::string& format, Args&&...args) {
			m_MessageLayer->ErrorInfinite(format, std::forward<Args>(args)...);
		}

		NavigationMenu* GetNavMenu() { return (NavigationMenu*)m_CurrentLayer; }


	private:
		UserInternalData GetUserDataStoredLocally();
	/*private:
		void SwitchToSignup(bool cleanAll);
		void SwitchToLogin(bool cleanAll);
		void SwitchToUserInformation();
		void SwitchToHome();*/
	public:
		
		Ref<Account> GetCurrentUser() {
			return m_CurrentUser;
		}

		Ref<Person> GetPerson(const std::string& id);
		Ref<Company> GetCompany(const std::string& id);
		Ref<Job> GetJob(const std::string& id);
		Ref<Post> GetPost(const std::string& id);
		//Ref<DirectMessage> GetDirectMessage(const std::string& id);

		Ref<Person> QueryPerson(const std::string& id, Ref<Person> person);
		Ref<Company> QueryCompany(const std::string& id, Ref<Company> company);
		Ref<Job> QueryJob(const std::string& id, Ref<Job> job);
		Ref<Post> QueryPost(const std::string& id, Ref<Post> post);
		Ref<DirectMessage> QueryDirectMessage(const std::string& id, Ref<DirectMessage> message);
		bool AccountExists(const std::string& username);
		bool AccountExists(const std::string& username, const std::string& password);
		bool PostExists(const std::string& id);
		std::string CompanyAccountExists(const std::string& username);
		std::string CompanyAccountExists(const std::string& username, const std::string& password);
		std::string PersonAccountExists(const std::string& username);
		std::string PersonAccountExists(const std::string& username, const std::string& password);

		void LikePost(Ref<Post> post);
		void RepostPost(Ref<Post> post);
		void CommentOnPost(Ref<Post> post, const std::string& text, const std::string& picture, const std::string& video);
		void Follow(const std::string& followerID, const std::string& followingID);
		void RequestFollow(const std::string& id);
		void MakePost(const std::string& text, const std::string& picture, const std::string& video);
		Ref<DirectMessage> SendMessage(Ref<Person> receiver, const std::string& text, const std::string& picture, const std::string& video);
		Ref<Job> CreateJob(Ref<Company> company, int32_t salary,const std::string& name, const std::string& location, JobType jobType, WorkspaceType workspaceType, const std::vector<std::string>& skills);
		void SignupCompany(const std::string& username,const std::string& password);
		void SignupPerson(const std::string& username,const std::string& password);
		void LoginCompany(const std::string& id);
		void LoginPerson(const std::string& id);
		void AcceptFollow(Ref<Person> person);
		void MakeJobRequest(const std::string& id);
		void AcceptJobRequest(Ref<Company> company, Ref<Job> job,Ref<Person> person);
		void DenyJobRequest(const std::string& id, Ref<Person> person);
		void UpdatePerson(Ref<Person> person,const std::string& imagepath, const std::string& firstname, const std::string& lastname, const std::string& email,const std::vector<std::string>& skills);
		void UpdateCompany(Ref<Company> company, const std::string& companyName, const std::string& imagepath, const std::string& phone, const std::string& email);


		std::vector<Ref<Person>> GetUserSuggestions(Ref<Person> person);
		std::vector<Ref<Person>> GetUserRequests(Ref<Person> person);
		std::vector<Ref<Person>> GetCompanyFollowers(Ref<Company> company);
		
		std::vector<PersonJobs> GetUserJobs(Ref<Person> person);
		std::vector<CompanyCreatedJob> GetCompanyCreatedJobs(Ref<Company> company);

		std::vector<Ref<Post>> GetSuggestedUserPostsFromFollowed(Ref<Person> person);
		std::vector<Ref<Post>> GetSuggestedUserPostsFromSameCompany(Ref<Person> person);
		std::vector<Ref<Post>> GetSuggestedUserPostsFromRandom(Ref<Person> person);
		SuggestedUserPosts GetSuggestedUserPosts(Ref<Person> person);
		std::vector<Ref<Post>> GetSuggestedCompanyPostsFromFollowed(Ref<Company> company);

		const std::unordered_map<std::string,std::string>& GetIDToUsername();

		//std::vector<CommentSpecification> GetPostComments(Post& post);

		void StoreUserDataLocally(const UserInternalData& userData);

		
		void SetCurrentLayer(Layer* layer);

		void SwitchToSignup();
		void SwitchToLogin();
		void SwitchToSplash();
		void SwitchToNavigation();

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
		//UserInformationLayer* m_UserInformationLayer;
		Ref<HomeLayer> m_HomeLayer;
		Ref<JobPanel> m_JobPanel;
		Ref<NetworkPanel> m_NetworkPanel;
		Ref<MessagePanel> m_MessagePanel;
		Ref<ProfilePanel> m_ProfilePanel;
		Ref<OtherProfilePanel> m_ViewProfilePanel;

		std::unordered_map<std::string, Ref<Person>> m_IDToPersonMap;
		std::unordered_map<std::string, Ref<Company>> m_IDToCompanyMap;
		std::unordered_map<std::string, Ref<Job>> m_IDToJobMap;
		std::unordered_map<std::string, Ref<Post>> m_IDToPostMap;
		std::unordered_map<std::string, std::string> m_IDToUsername;

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
		friend class CommentsLayer;
		friend class Person;
		friend class Company;
		friend class PersonHomeLayerUI;
	};
}