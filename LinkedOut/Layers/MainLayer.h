#pragma once
#include "Core/Layer.h"
#include "SignupLayer.h"
#include "LoginLayer.h"
#include "SplashLayer.h"
#include "ErrorLayer.h"

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


		UserData GetUserDataStoredLocally();
	private:
		void SwitchToSignup();
		void SwitchToLogin();
	private:
		SignupErrorCodes Signup(const UserData& userData);
		LoginErrorCodes Login(const UserData& userData,bool storeLocally);

		void StoreUserDataLocally(const UserData& userData);


		bool UserExists(const std::string& username);
		UserData GetUserDataFromUsername(const std::string& userName);

		void SetCurrentLayer(Layer* layer);

	private:

		UserData m_UserData;

		QWidget* m_WindowCentralWidget;


		Layer* m_CurrentLayer;

		
		
		uint32_t m_LastCaptchaNumber;

		SignupLayer* m_SignupLayer;
		LoginLayer* m_LoginLayer;
		SplashLayer* m_SplashLayer;
		MessageLayer* m_MessageLayer;

		friend class SignupLayer;
		friend class LoginLayer;
		friend class SplashLayer;
		friend class MessageLayer;
	};
}