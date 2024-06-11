#pragma once
#include "Core/Layer.h"
#include "SignupLayer.h"
#include "LoginLayer.h"
#include "SplashLayer.h"
#include "ErrorLayer.h"
#include "UserInformationLayer.h"

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

	
	struct Message {
		std::string Error;

	};



	class MainLayer{
	public:
		virtual void OnAttach() ;
		virtual void OnDetach() ;
		virtual void OnUpdate() ;
	private:
		void SetupSplash();
		void SetupSplashEvents();


		UserInternalData GetUserDataStoredLocally();
	private:
		void SwitchToSignup(bool cleanAll);
		void SwitchToLogin(bool cleanAll);
		void SwitchToUserInformation();
	private:
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


		Layer* m_CurrentLayer;

		
		
		uint32_t m_LastCaptchaNumber;

		SignupLayer* m_SignupLayer;
		LoginLayer* m_LoginLayer;
		SplashLayer* m_SplashLayer;
		MessageLayer* m_MessageLayer;
		UserInformationLayer* m_UserInformationLayer;

		friend class SignupLayer;
		friend class LoginLayer;
		friend class SplashLayer;
		friend class MessageLayer;
		friend class UserInformationLayer;
	};
}