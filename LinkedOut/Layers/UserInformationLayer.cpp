#include "UserInformationLayer.h"
#include "Core/Window.h"
#include "MainLayer.h"

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QApplication>



namespace LinkedOut {
	UserInformationLayer::UserInformationLayer(MainLayer* layer)
		:Layer("UserInformationLayer")

	{
		m_MainLayer = layer;
		SetupUserInformation();
	}
	void UserInformationLayer::SetContext(const UserData& userData)
	{
		m_UserData = userData;

		m_FirstNameTextInput->setText(QString::fromStdString(userData.FirstName));
		m_LastNameTextInput->setText(QString::fromStdString(userData.LastName));
		m_BirthDateTextInput->setText(QString::fromStdString(userData.BirthDate));
		m_PlaceOfEducationTextInput->setText(QString::fromStdString(userData.PlaceOfEducation));
	}
	UserInformationLayer::~UserInformationLayer()
	{
	}
	void UserInformationLayer::Show()
	{
		m_MainFrame->show();
		m_IsShowing = true;
	}
	void UserInformationLayer::Hide()
	{
		m_IsShowing = false;
		m_MainFrame->hide();
	}
	void UserInformationLayer::OnUpdate()
	{
	}
	void UserInformationLayer::SetupUserInformation()
	{
		auto window = Window::GetMainWindow()->GetNativeHandle();
		//Main frame
		{
			m_MainFrame = new QFrame(m_MainLayer->m_WindowCentralWidget);
			m_MainFrame->setObjectName(QString::fromUtf8("MainFrame"));
			m_MainFrame->setGeometry(QRect(0, 0, 1280, 720));
			m_MainFrame->setSizeIncrement(QSize(0, 0));
			m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;width:inherited;"));
			m_MainFrame->setFrameShape(QFrame::StyledPanel);
			m_MainFrame->setFrameShadow(QFrame::Raised);
		}
	   
		//User information frame
		{
			m_UserInformationFrame = new QFrame(m_MainFrame);
			m_UserInformationFrame->setObjectName(QString::fromUtf8("UserInformation_Frame"));
			m_UserInformationFrame->setGeometry(QRect(140, 60, 1000, 600));
			m_UserInformationFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(86, 152, 144);\n"
				"border-style: solid; border-width: 2px; border-color: #000000;\n"
				"	border-radius: 20px;"));
			m_UserInformationFrame->setFrameShape(QFrame::NoFrame);
			m_UserInformationFrame->setFrameShadow(QFrame::Raised);
		}

		//User profile label
		{
			m_UserProfileLabel = new QLabel(m_UserInformationFrame);
			m_UserProfileLabel->setObjectName(QString::fromUtf8("label"));
			m_UserProfileLabel->setGeometry(QRect(250, 10, 500, 100));
			QFont font;
			font.setFamily(QString::fromUtf8("Cascadia Mono"));
			font.setPointSize(15);
			font.setBold(true);
			font.setItalic(true);
			font.setWeight(75);
			font.setStrikeOut(false);
			font.setKerning(false);
			m_UserProfileLabel->setFont(font);
			m_UserProfileLabel->setStyleSheet(QString::fromUtf8("border-color:#FFFFFF;border-radius:50px;background-color:rgb(33, 150, 97)\n"
				""));
			m_UserProfileLabel->setAlignment(Qt::AlignCenter);
			m_UserProfileLabel->setText(QApplication::translate("LinkedOutClass", "User Profile", nullptr));
		}
		
		//First name frame
		{
			m_FirstNameFrame = new QFrame(m_UserInformationFrame);
			m_FirstNameFrame->setObjectName(QString::fromUtf8("frame"));
			m_FirstNameFrame->setGeometry(QRect(130, 130, 351, 100));
			m_FirstNameFrame->setStyleSheet(QString::fromUtf8("border:none;"));
			m_FirstNameFrame->setFrameShape(QFrame::StyledPanel);
			m_FirstNameFrame->setFrameShadow(QFrame::Raised);
		}
	  
		//First name text input
		{
			m_FirstNameTextInput = new QLineEdit(m_FirstNameFrame);
			m_FirstNameTextInput->setObjectName(QString::fromUtf8("lineEdit"));
			m_FirstNameTextInput->setGeometry(QRect(120, 30, 231, 40));
			m_FirstNameTextInput->setStyleSheet(QString::fromUtf8("background-color:rgb(33, 150, 97);\n"
				"border-style: solid; border-width: 2px; border-color: white;\n"
				"border-radius: 20px;"));
			m_FirstNameTextInput->setClearButtonEnabled(false);
		}
		
		QFont font1;
		font1.setFamily(QString::fromUtf8("Cascadia Mono"));
		font1.setPointSize(12);
		font1.setBold(true);
		font1.setItalic(true);
		font1.setWeight(75);

		//First name label
		{
			m_FirstNameLabel = new QLabel(m_FirstNameFrame);
			m_FirstNameLabel->setObjectName(QString::fromUtf8("label_2"));
			m_FirstNameLabel->setGeometry(QRect(0, 35, 111, 30));
			m_FirstNameLabel->setFont(font1);
			m_FirstNameLabel->setFocusPolicy(Qt::NoFocus);
			m_FirstNameLabel->setStyleSheet(QString::fromUtf8("border:none;"));
			m_FirstNameLabel->setText(QApplication::translate("LinkedOutClass", "First name:", nullptr));
		}

		//Last name frame
		{
			m_LastNameFrame = new QFrame(m_UserInformationFrame);
			m_LastNameFrame->setObjectName(QString::fromUtf8("frame_2"));
			m_LastNameFrame->setGeometry(QRect(510, 130, 351, 100));
			m_LastNameFrame->setStyleSheet(QString::fromUtf8("border:none;"));
			m_LastNameFrame->setFrameShape(QFrame::StyledPanel);
			m_LastNameFrame->setFrameShadow(QFrame::Raised);
		}

		//Last name text input
		{
			m_LastNameTextInput = new QLineEdit(m_LastNameFrame);
			m_LastNameTextInput->setObjectName(QString::fromUtf8("lineEdit_2"));
			m_LastNameTextInput->setGeometry(QRect(120, 30, 231, 40));
			m_LastNameTextInput->setStyleSheet(QString::fromUtf8("background-color:rgb(33, 150, 97);\n"
				"border-style: solid; border-width: 2px; border-color: white;\n"
				"border-radius: 20px;"));
			m_LastNameTextInput->setClearButtonEnabled(false);
		}
	   
		//Last name label
		{
			m_LastNameLabel = new QLabel(m_LastNameFrame);
			m_LastNameLabel->setObjectName(QString::fromUtf8("label_3"));
			m_LastNameLabel->setGeometry(QRect(0, 35, 111, 30));
			m_LastNameLabel->setFont(font1);
			m_LastNameLabel->setFocusPolicy(Qt::NoFocus);
			m_LastNameLabel->setStyleSheet(QString::fromUtf8("border:none;"));
			m_LastNameLabel->setText(QApplication::translate("LinkedOutClass", "Last name:", nullptr));
		}
		
	   //Birth date frame
		{
			m_BirthDateFrame = new QFrame(m_UserInformationFrame);
			m_BirthDateFrame->setObjectName(QString::fromUtf8("frame_3"));
			m_BirthDateFrame->setGeometry(QRect(130, 240, 351, 100));
			m_BirthDateFrame->setStyleSheet(QString::fromUtf8("border:none;"));
			m_BirthDateFrame->setFrameShape(QFrame::StyledPanel);
			m_BirthDateFrame->setFrameShadow(QFrame::Raised);
		}
		
		//Birth date text input
		{
			m_BirthDateTextInput = new QLineEdit(m_BirthDateFrame);
			m_BirthDateTextInput->setObjectName(QString::fromUtf8("lineEdit_3"));
			m_BirthDateTextInput->setGeometry(QRect(120, 30, 231, 40));
			m_BirthDateTextInput->setStyleSheet(QString::fromUtf8("background-color:rgb(33, 150, 97);\n"
				"border-style: solid; border-width: 2px; border-color: white;\n"
				"border-radius: 20px;"));
			m_BirthDateTextInput->setInputMethodHints(Qt::ImhDate);
			m_BirthDateTextInput->setClearButtonEnabled(false);
		}

		//Birth date label
		{
			m_BirthDateLabel = new QLabel(m_BirthDateFrame);
			m_BirthDateLabel->setObjectName(QString::fromUtf8("label_4"));
			m_BirthDateLabel->setGeometry(QRect(0, 35, 111, 30));
			m_BirthDateLabel->setFont(font1);
			m_BirthDateLabel->setFocusPolicy(Qt::NoFocus);
			m_BirthDateLabel->setStyleSheet(QString::fromUtf8("border:none;"));
			m_BirthDateLabel->setText(QApplication::translate("LinkedOutClass", "Birth date:", nullptr));
		}

		//Place of education frame
		{
			m_PlaceOfEducationFrame = new QFrame(m_UserInformationFrame);
			m_PlaceOfEducationFrame->setObjectName(QString::fromUtf8("frame_4"));
			m_PlaceOfEducationFrame->setGeometry(QRect(510, 240, 351, 100));
			m_PlaceOfEducationFrame->setStyleSheet(QString::fromUtf8("border:none;"));
			m_PlaceOfEducationFrame->setFrameShape(QFrame::StyledPanel);
			m_PlaceOfEducationFrame->setFrameShadow(QFrame::Raised);
		}
		
		//Place of education label
		{
			m_PlaceOfEducationLabel = new QLabel(m_PlaceOfEducationFrame);
			m_PlaceOfEducationLabel->setObjectName(QString::fromUtf8("label_5"));
			m_PlaceOfEducationLabel->setGeometry(QRect(45, 0, 261, 30));
			m_PlaceOfEducationLabel->setFont(font1);
			m_PlaceOfEducationLabel->setFocusPolicy(Qt::NoFocus);
			m_PlaceOfEducationLabel->setStyleSheet(QString::fromUtf8("border:none;"));
			m_PlaceOfEducationLabel->setText(QApplication::translate("LinkedOutClass", "School or College/University:", nullptr));
		}
		
		//Place of education combo box
		{
			m_PlaceOfEducationTextInput = new QLineEdit(m_PlaceOfEducationFrame);
			m_PlaceOfEducationTextInput->setObjectName(QString::fromUtf8("textInput"));
			m_PlaceOfEducationTextInput->setGeometry(QRect(60, 40, 231, 22));
			m_PlaceOfEducationTextInput->setStyleSheet(QString::fromUtf8("background-color:rgb(33, 150, 97);\n"
				"border-style: solid; border-width: 2px; border-color: white;\n"
				"border-radius: 20px;"));
			m_PlaceOfEducationTextInput->setFrame(true);

		}

		//Save buttton
		{
			m_SaveButton = new QPushButton(m_UserInformationFrame);
			m_SaveButton->setObjectName(QString::fromUtf8("pushButton"));
			m_SaveButton->setGeometry(QRect(400, 470, 200, 51));
			m_SaveButton->setStyleSheet(QString::fromUtf8("border-color:#FFFFFF;border-radius:25px;background-color:rgb(33, 150, 97)\n"));
			m_SaveButton->setText(QApplication::translate("LinkedOutClass", "Save", nullptr));
		}


		SetupUserInformationEvents();
		
	}
	void UserInformationLayer::SetupUserInformationEvents()
	{
		QObject::connect(m_SaveButton, &QPushButton::clicked, [this]() {
			m_UserData = GetUserInfoFromFields();
			m_MainLayer->StoreUserInformation(m_UserData);
			});
	}
	void UserInformationLayer::ValidateInput()
	{

	}
	UserData UserInformationLayer::GetUserInfoFromFields()
	{
		UserData userData{};
		userData.FirstName = m_FirstNameTextInput->text().toStdString();
		userData.LastName = m_LastNameTextInput->text().toStdString();
		userData.BirthDate = m_BirthDateTextInput->text().toStdString();
		userData.PlaceOfEducation = m_PlaceOfEducationTextInput->text().toStdString();
		return userData;

	}
}