#include "MainLayer.h"
#include "Core/Window.h"
#include "Core/Application.h"
#include "Core/Random.h"
#include "CustomUI/PopupWindow.h"

#include <QMainWindow>
#include <qpushbutton.h>
#include "ui_LinkedOut.h"
#include <QStyle>
#include <QStandardPaths>
#include <qlabel.h>
#include <qlayout.h>
#include <QValidator>
#include <qvalidator.h>
#include <QTreeWidget>
#include <QDate>

#include <iostream>


namespace LinkedOut {


	static constexpr const char* TempFileName = "LinkedOutUD.txt";

	UserInternalData MainLayer::GetUserDataStoredLocally() {
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

	void MainLayer::StoreUserDataLocally(const UserInternalData& userData) {
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

	void MainLayer::SwitchToUserInformation()
	{
		
		if (m_CurrentLayer == m_LoginLayer) {
			UserData data = GetUserInformation(m_UserData.UniqueUserID);

			m_UserInformationLayer->SetContext(data);
		}
		
		SetCurrentLayer(m_UserInformationLayer);
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

	// Function to create a custom widget with a button
	QWidget* createButtonWidget(const QString& buttonText, const QString& message) {
		QWidget* widget = new QWidget();
		QVBoxLayout* layout = new QVBoxLayout(widget);
		QPushButton* button = new QPushButton(buttonText, widget);

		widget->setAutoFillBackground(true);
		// Connect the button's clicked signal to a lambda function that prints the message
		QObject::connect(button, &QPushButton::clicked, [message]() {
			std::cout << message.toStdString() << std::endl;
			});

		layout->addWidget(button);
		widget->setLayout(layout);
		return widget;
	}

#define TEST 0


	void MainLayer::OnAttach() {
		#if TEST==0
		QMainWindow* window = Window::GetMainWindow()->GetNativeHandle();

		m_WindowCentralWidget = new QWidget(window);

		window->setCentralWidget(m_WindowCentralWidget);

		window->resize(1280, 720);

		window->setWindowTitle(QApplication::translate("window", "LinkedOut", nullptr));


		m_SplashLayer = new SplashLayer(this);
		m_SignupLayer = new SignupLayer(this);
		m_LoginLayer = new LoginLayer(this);
		m_MessageLayer = new MessageLayer(this);
		m_UserInformationLayer = new UserInformationLayer(this);

		m_SplashLayer->Hide();
		m_SignupLayer->Hide();
		m_LoginLayer->Hide();
		m_UserInformationLayer->Hide();


		m_UserData = GetUserDataStoredLocally();
		

		SetCurrentLayer(m_SplashLayer);

		if (!m_UserData.IsValid()) {
			SetCurrentLayer(m_SplashLayer);
		}

		QMetaObject::connectSlotsByName(window);
		#else
		treeWidget = new QTreeWidget();
		treeWidget->setColumnCount(1);
		treeWidget->setHeaderLabels(QStringList() << "Column 1");

		// Create first tree node with a button
		QTreeWidgetItem* item1 = new QTreeWidgetItem(treeWidget);
		item1->setText(0, "Hello");
		treeWidget->setItemWidget(item1, 0, createButtonWidget("Hello button", "Hello"));

		// Create second tree node with a button
		QTreeWidgetItem* item2 = new QTreeWidgetItem(treeWidget);
		item2->setText(0, "Hello2");
		treeWidget->setItemWidget(item2, 0, createButtonWidget("Hello2 button", "Hello2"));

		// Expand all nodes
		treeWidget->expandAll();
		#endif	
	}
	void MainLayer::OnDetach(){
		delete m_LoginLayer;
		delete m_SignupLayer;
		delete m_SplashLayer;
		delete m_WindowCentralWidget;
	}


	void MainLayer::OnUpdate(){
#if TEST ==0 
		m_CurrentLayer->OnUpdate();
		m_MessageLayer->OnUpdate();
#else
		treeWidget->show();

#endif
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

	UserInternalData MainLayer::GetUserDataFromUsername(const std::string& userName) {
		return {};
	}

	UserData MainLayer::GetUserInformation(uint32_t userID)
	{
		auto db = Application::Get()->GetDatabase();

		UserData userData{};

		//Jobs
		{
			std::string statement = fmt::format("SELECT * FROM WANTEDUSERJOB WHERE (UNIQUE_USERID = '{}');",userID);

			auto res = db->ExecuteAndReturn(statement);

			for (auto& row : res) {
				userData.WantedJobPositions.push_back(row.field("JOB_TITLE").value().toString().toStdString());
			}

		}
		//Company
		{
			std::string statement = fmt::format("SELECT * FROM WANTEDCOMPANYJOB WHERE (UNIQUE_USERID = '{}');", userID);

			auto res = db->ExecuteAndReturn(statement);

			for (auto& row : res) {
				userData.WantedJobPositions.push_back(row.field("COMPANY_NAME").value().toString().toStdString());
			}

		}

		std::string statement = fmt::format("SELECT * FROM USER WHERE (UNIQUE_USERID = '{}');", userID);
		auto res = db->ExecuteAndReturn(statement);
		assert(res.size() == 1);

		auto& record = res[0];

		auto firstName = record.field("FIRSTNAME").value().toString().toStdString();
		auto lastName = record.field("LASTNAME").value().toString().toStdString();

		auto birthDate = record.field("BIRTHDATE").value().toString().toStdString();

		auto placeOfStudy = record.field("PLACEOFSTUDY").value().toString().toStdString();

		userData.FirstName = firstName;
		userData.LastName = lastName;
		userData.PlaceOfEducation = placeOfStudy;
		userData.BirthDate = birthDate;

		return userData;
	}

	uint32_t MainLayer::GetUniqueUserID(const std::string& userName)
	{
		auto db = Application::Get()->GetDatabase();



		std::string statement = fmt::format("SELECT * FROM USER WHERE (USERNAME = '{}');", userName);
		try {
			auto res = db->ExecuteAndReturn(statement);
			if (res.size() != 1)
				return -1;

			return res[0].field("UNIQUE_USERID").value().toUInt();
		}
		catch (...) {
			return -1;
		}
		return -1;

	}

	SignupErrorCodes MainLayer::Signup(const UserInternalData& userData) {
		auto db= Application::Get()->GetDatabase();
		
		{
			std::string statement = fmt::format("SELECT * FROM USER WHERE (USERNAME = '{}');", userData.Username);
			auto res = db->ExecuteAndReturn(statement);
			if (!res.empty())
				return SignupErrorCodes::UserExists;
		}



		std::string statement = fmt::format("INSERT INTO USER(USERNAME,PASSWORD) VALUES ('{}','{}');", userData.Username, userData.Password);
		try {
			db->Execute(statement);
			m_UserData = userData;
			m_UserData.UniqueUserID = GetUniqueUserID(userData.Username);
		}
		catch (QueryException& exception) {
			return SignupErrorCodes::UserExists;
		}
		return SignupErrorCodes::None;
	}




	LoginErrorCodes MainLayer::Login(const UserInternalData& userData,bool storeLocally) {
		auto db = Application::Get()->GetDatabase();

		std::string statement = fmt::format("SELECT * FROM USER WHERE (USERNAME = '{}' AND PASSWORD = '{}');",userData.Username,userData.Password);

		try {
			auto res = db->ExecuteAndReturn(statement);
			if (res.empty())
				return LoginErrorCodes_IncorrectUsername | LoginErrorCodes_IncorrectPassword;

			m_UserData = userData;
			m_UserData.UniqueUserID = res[0].field("UNIQUE_USERID").value().toUInt();
			if (storeLocally)
				StoreUserDataLocally(userData);

			return LoginErrorCodes_None;
		}
		catch (...) {
			return LoginErrorCodes_DBError;
		}

		return LoginErrorCodes_None;
	}

	template<typename _ReturnType,typename _Iterator>
	_ReturnType Join(_Iterator begin, _Iterator end, const _ReturnType& splitter) {

		_ReturnType result{};

		for (_Iterator current = begin; current != end; ++current) {
			result += *current;
			if (current + 1 != end)
				result += splitter;
		}

		return result;
	}


	UserInformationStoreErrorCodes MainLayer::StoreUserInformation(const UserData& userData)
	{


		auto db = Application::Get()->GetDatabase();

		try {
			//Delete old data
			{
				std::string jobDelete = fmt::format("DELETE FROM WANTEDUSERJOB WHERE UNIQUE_USERID = '{}'", m_UserData.UniqueUserID);
				db->Execute(jobDelete);
				std::string companyDelete = fmt::format("DELETE FROM WANTEDUSERCOMPANY WHERE UNIQUE_USERID = '{}'", m_UserData.UniqueUserID);
				db->Execute(companyDelete);
			}

			std::string statement = fmt::format("UPDATE USER SET FIRSTNAME = '{}',LASTNAME = '{}',BIRTHDATE = '{}',PLACEOFSTUDY = '{}' WHERE UNIQUE_USERID = '{}'", userData.FirstName, userData.LastName,
				userData.BirthDate, userData.PlaceOfEducation, m_UserData.UniqueUserID);

			db->Execute(statement);

			{
				std::string jobInsertCommand{};
				for (auto& job : userData.WantedJobPositions) {
					jobInsertCommand += fmt::format("INSERT INTO WANTEDUSERJOB(UNIQUE_USERID,JOB_TITLE) VALUES('{}','{}');", m_UserData.UniqueUserID, job);
				}
				if(!jobInsertCommand.empty())
					db->Execute(jobInsertCommand);
			}

			{
				std::string companyInsertCommand{};
				for (auto& company : userData.WantedCompanies) {
					companyInsertCommand += fmt::format("INSERT INTO WANTEDUSERCOMPANY(UNIQUE_USERID,COMPANY_NAME) VALUES('{}','{}');", m_UserData.UniqueUserID, company);
				}
				if(!companyInsertCommand.empty())
				db->Execute(companyInsertCommand);
			}
		}
		catch(std::exception& err) {
			return UserInformationStoreErrorCodes_DBError;
		}
	
		return UserInformationStoreErrorCodes_None;
	}

	
	

}