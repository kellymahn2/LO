#include "MainLayer.h"
#include "Core/Window.h"
#include "Core/Application.h"
#include "Core/Random.h"



#include <QMainWindow>
#include <qpushbutton.h>
#include "ui_LinkedOut.h"
#include <QStyle>
#include <QStandardPaths>
#include <qlabel.h>
#include <qlayout.h>
#include <QValidator>
#include <qvalidator.h>

#include <iostream>


namespace LinkedOut {


	static constexpr const char* TempFileName = "LinkedOutUD.txt";

	UserData MainLayer::GetUserDataStoredLocally() {
		FileSystem::path tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation).toStdString();

		FileSystem::path tempFilePath = tempPath / TempFileName;
		if (FileSystem::directory_entry(tempPath).exists()) {

			std::ifstream tempFile(tempFilePath);

			if (tempFile.is_open() == false)
				return {};

			std::string username;
			std::string password;

			{
				char buff[10000];
				tempFile.getline(buff, 10000);
				username = buff;
			}

			{
				char buff[10000];
				tempFile.getline(buff, 10000);
				password = buff;
			}

			TrimInplace(username);
			TrimInplace(password);

			if (!IsValidUsername(username))
				return {};
			if (!IsValidPassword(password))
				return {};



			return { std::move(username), std::move(password) };
		}
		return {};
	}

	void MainLayer::StoreUserDataLocally(const UserData& userData) {
		FileSystem::path tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation).toStdString();
		FileSystem::path tempFilePath = tempPath / TempFileName;
		std::ofstream file(tempFilePath);
		if (!file.is_open())
			return;
		file.clear();
		file << userData.Username;
		file << "\n";
		file << userData.Password;
		file.close();
		return;
	}



	Ui_LinkedOutClass* cl;

	void MainLayer::SwitchToLogin(bool cleanAll) {
		SetCurrentLayer(m_LoginLayer);
		m_LastCaptchaNumber = Random::GenerateUInt32(1000, 10000);
		m_LoginLayer->m_CaptchaLabel->setText(QString("Captcha %1").arg(m_LastCaptchaNumber));
		if (cleanAll)
			m_LoginLayer->CleanAllInputs();
		else
			m_LoginLayer->CleanupInputs();
	}

	void MainLayer::SwitchToSignup(bool cleanAll) {

		SetCurrentLayer(m_SignupLayer);

		m_LastCaptchaNumber= Random::GenerateUInt32(1000, 10000);

		m_SignupLayer->m_CaptchaLabel->setText(QString("Captcha %1").arg(m_LastCaptchaNumber));
		if (cleanAll)
			m_SignupLayer->CleanAllInputs();
		else
			m_SignupLayer->CleanupInputs();
	}

	void MainLayer::OnAttach() {
		QMainWindow* window = Window::GetMainWindow()->GetNativeHandle();

		m_WindowCentralWidget = new QWidget(window);

		window->setCentralWidget(m_WindowCentralWidget);

		window->resize(1280, 720);

		window->setWindowTitle(QApplication::translate("window", "LinkedOut", nullptr));


		m_SplashLayer = new SplashLayer(this);
		m_SignupLayer = new SignupLayer(this);
		m_LoginLayer = new LoginLayer(this);
		m_MessageLayer = new MessageLayer(this);

		m_MessageLayer->InfoInfinite("Hello, Info");
		m_MessageLayer->WarnInfinite("Hello, Warn");

		m_MessageLayer->InfoTimed(10, "Hello, Info Timed");

		m_MessageLayer->ErrorInfinite("Hello, Error");




		m_SplashLayer->Hide();
		m_SignupLayer->Hide();
		m_LoginLayer->Hide();

		m_UserData = GetUserDataStoredLocally();
		

		SetCurrentLayer(m_SplashLayer);

		if (!m_UserData.IsValid()) {
			SetCurrentLayer(m_SplashLayer);
		}

		QMetaObject::connectSlotsByName(window);
	}
	void MainLayer::OnDetach(){
		delete m_LoginLayer;
		delete m_SignupLayer;
		delete m_SplashLayer;
		delete m_WindowCentralWidget;
	}


	void MainLayer::OnUpdate(){
		m_CurrentLayer->OnUpdate();
		m_MessageLayer->OnUpdate();
	}
	


	void MainLayer::SetCurrentLayer(Layer* layer) {
		if (m_CurrentLayer)
			m_CurrentLayer->Hide();
		layer->Show();
		m_CurrentLayer = layer;
		m_MessageLayer->Show();
	}

	bool MainLayer::UserExists(const std::string& username) {
		auto db = Application::Get()->GetDatabase();
		std::string statement = fmt::format("SELECT * FROM USER WHERE (USERNAME == '{}');", username);
		auto res = db->ExecuteAndReturn(statement);
		return !res.empty();

	}

	UserData MainLayer::GetUserDataFromUsername(const std::string& userName) {
		return {};
	}

	SignupErrorCodes MainLayer::Signup(const UserData& userData) {
		auto db= Application::Get()->GetDatabase();
		
		{
			std::string statement = fmt::format("SELECT * FROM USER WHERE (USERNAME == '{}');", userData.Username);
			auto res = db->ExecuteAndReturn(statement);
			if (!res.empty())
				return SignupErrorCodes::UserExists;
		}



		std::string statement = fmt::format("INSERT INTO USER(USERNAME,PASSWORD) VALUES ('{}','{}');", userData.Username, userData.Password);
		try {
			db->Execute(statement);
		}
		catch (QueryException& exception) {
			return SignupErrorCodes::UserExists;
		}
		return SignupErrorCodes::None;
	}




	LoginErrorCodes MainLayer::Login(const UserData& userData,bool storeLocally) {
		auto db = Application::Get()->GetDatabase();

		std::string statement = fmt::format("SELECT * FROM USER WHERE (USERNAME == '{}' AND PASSWORD == '{}');",userData.Username,userData.Password);

		try {
			auto res = db->ExecuteAndReturn(statement);
			if (res.empty())
				return LoginErrorCodes_IncorrectUsername | LoginErrorCodes_IncorrectPassword;
			m_UserData = userData;

			if (storeLocally)
				StoreUserDataLocally(userData);

			return LoginErrorCodes_None;
		}
		catch (...) {
			return LoginErrorCodes_DBError;
		}

		return LoginErrorCodes_None;
	}

	
	

}