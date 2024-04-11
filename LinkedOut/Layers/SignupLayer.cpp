#include "SignupLayer.h"
#include "MainLayer.h"

#include "Core/Window.h"
#include "CommonErrors.h"

#include <QMainWindow>
#include <qpushbutton.h>
#include "ui_LinkedOut.h"
#include <QStyle>
#include <QStandardPaths>
#include <qlabel.h>
#include <qlayout.h>
#include <QValidator>
#include <qvalidator.h>
#include <QLayout>
#include "CustomUI/ClickableLabel.h"


namespace LinkedOut {

	SignupLayer::SignupLayer(MainLayer* mainLayer)
		:Layer("Signup layer")
	{
		m_MainLayer = mainLayer;
		SetupSignup();
	}



	SignupLayer::~SignupLayer() {
		Cleanup();
	}


	void SignupLayer::Cleanup() {
		//Signup button
		{
			delete m_Button;
		}

		//Captcha text input
		{
			delete m_CaptchaValidator;
			delete m_CaptchaTextInput;
		}

		//Captcha label
		{
			delete m_CaptchaLabel;
		}

		//Captcha frame
		{
			delete m_CaptchaFrame;
		}

		//To login page label
		{
			delete m_ToLoginLabel;
		}

		//Password text input
		{
			delete m_PasswordTextInput;
		}

		//Password label
		{
			delete m_PasswordLabel;
		}

		//Password frame
		{
			delete m_PasswordFrame;
		}

		//Username text input
		{
			delete m_UsernameTextInput;
		}

		//Username label
		{
			delete m_UsernameLabel;
		}

		//Username frame
		{
			delete m_UsernameFrame;
		}

		//Signup label
		{
			delete m_Label;
		}

		//Signup frame
		{
			delete m_Frame;
		}

		//Main frame
		{
			delete m_MainFrame;
		}

		delete m_CentralWidget;
	}



	void SignupLayer::Show() {
		m_MainFrame->show();
		m_IsShowing = true;
	}
	void SignupLayer::Hide() {
		m_MainFrame->hide();
		m_IsShowing = false;
	}

	void SignupLayer::OnUpdate() {
		auto window = Window::GetMainWindow()->GetNativeHandle();
		auto size = window->size();
		m_MainFrame->resize(size);

		auto frameSize = m_Frame->size();

		float startX = ((float)size.width() - (float)frameSize.width()) / 2.0f;
		float startY = ((float)size.height() - (float)frameSize.height()) / 2.0f;


		m_Frame->setGeometry(startX, startY, frameSize.width(), frameSize.height());

	}

	void SignupLayer::SetupSignup() {
		auto window = Window::GetMainWindow()->GetNativeHandle();

		QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

		//Main frame
		{
			m_MainFrame = new QFrame(m_MainLayer->m_WindowCentralWidget);
			m_MainFrame->setObjectName(QString::fromUtf8("MainFrame"));
			m_MainFrame->setGeometry(QRect(0, 0, 1280, 720));
			sizePolicy.setHeightForWidth(m_MainFrame->sizePolicy().hasHeightForWidth());
			m_MainFrame->setSizePolicy(sizePolicy);
			m_MainFrame->setSizeIncrement(QSize(0, 0));
			m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;width:inherited;"));
			m_MainFrame->setFrameShape(QFrame::StyledPanel);
			m_MainFrame->setFrameShadow(QFrame::Raised);
		}

		//Signup frame
		{
			m_Frame = new QFrame(m_MainFrame);
			m_Frame->setObjectName(QString::fromUtf8("SignupFrame"));
			m_Frame->setGeometry(QRect(450, 110, 380, 500));
			m_Frame->setStyleSheet(QString::fromUtf8("background-color:rgb(86, 152, 144);\n"
				"border-style: solid; border-width: 2px; border-color: #000000;\n"
				"	border-radius: 20px;"));
			m_Frame->setFrameShape(QFrame::NoFrame);
			m_Frame->setFrameShadow(QFrame::Raised);
		}

		//Signup label
		{
			m_Label = new QLabel(m_Frame);
			m_Label->setObjectName(QString::fromUtf8("SignupLabel"));
			m_Label->setGeometry(QRect(150, 10, 80, 41));
			m_Label->setStyleSheet(QString::fromUtf8("border:none;font-weight:bold;font-size:20px;"));
			m_Label->setAlignment(Qt::AlignCenter);
			m_Label->setText(QApplication::translate("LinkedOutClass", "Sign up", nullptr));
		}

		QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

		//Username frame
		{
			m_UsernameFrame = new QFrame(m_Frame);
			m_UsernameFrame->setObjectName(QString::fromUtf8("UsernameFrame"));
			m_UsernameFrame->setGeometry(QRect(0, 60, 380, 141));
			sizePolicy1.setHorizontalStretch(0);
			sizePolicy1.setVerticalStretch(0);
			sizePolicy1.setHeightForWidth(m_UsernameFrame->sizePolicy().hasHeightForWidth());
			m_UsernameFrame->setSizePolicy(sizePolicy1);
			m_UsernameFrame->setStyleSheet(QString::fromUtf8("border-radius:0;border-top:none;border-bottom:none;\n"
				""));
			m_UsernameFrame->setFrameShape(QFrame::NoFrame);
			m_UsernameFrame->setFrameShadow(QFrame::Raised);
		}

		//Username label
		{
			m_UsernameLabel = new QLabel(m_UsernameFrame);
			m_UsernameLabel->setObjectName(QString::fromUtf8("UsernameLabel"));
			m_UsernameLabel->setGeometry(QRect(0, 50, 71, 41));
			m_UsernameLabel->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
			m_UsernameLabel->setText(QApplication::translate("LinkedOutClass", "User name:  ", nullptr));
		}

		//Username text input
		{
			m_UsernameTextInput = new QLineEdit(m_UsernameFrame);
			m_UsernameTextInput->setObjectName(QString::fromUtf8("UsernameTextInput"));
			m_UsernameTextInput->setGeometry(QRect(70, 50, 291, 41));
			m_UsernameTextInput->setAcceptDrops(false);
			m_UsernameTextInput->setStyleSheet(QString::fromUtf8("border-style: solid; border-width: 2px; border-color: #000000;"));
			m_UsernameTextInput->setMaxLength(9999);
			m_UsernameTextInput->setAlignment(Qt::AlignCenter);
			m_UsernameTextInput->setPlaceholderText(QApplication::translate("LinkedOutClass", "User name", nullptr));
		}

		//Password frame
		{
			m_PasswordFrame = new QFrame(m_Frame);
			m_PasswordFrame->setObjectName(QString::fromUtf8("PasswordFrame"));
			m_PasswordFrame->setGeometry(QRect(0, 200, 380, 141));
			sizePolicy1.setHeightForWidth(m_PasswordFrame->sizePolicy().hasHeightForWidth());
			m_PasswordFrame->setSizePolicy(sizePolicy1);
			m_PasswordFrame->setStyleSheet(QString::fromUtf8("border-radius:0;border-top:none;border-bottom:none;\n"
				""));
			m_PasswordFrame->setFrameShape(QFrame::NoFrame);
			m_PasswordFrame->setFrameShadow(QFrame::Plain);
			m_PasswordFrame->setLineWidth(0);
		}

		//Password label
		{
			m_PasswordLabel = new QLabel(m_PasswordFrame);
			m_PasswordLabel->setObjectName(QString::fromUtf8("PasswordLabel"));
			m_PasswordLabel->setGeometry(QRect(0, 50, 71, 41));
			m_PasswordLabel->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
			m_PasswordLabel->setText(QApplication::translate("LinkedOutClass", "Password:   ", nullptr));
		}

		//Password text input
		{
			m_PasswordTextInput = new QLineEdit(m_PasswordFrame);
			m_PasswordTextInput->setObjectName(QString::fromUtf8("PasswordTextInput"));
			m_PasswordTextInput->setGeometry(QRect(70, 50, 291, 41));
			m_PasswordTextInput->setAcceptDrops(false);
			m_PasswordTextInput->setStyleSheet(QString::fromUtf8("border-style: solid; border-width: 2px; border-color: #000000;"));
			m_PasswordTextInput->setMaxLength(9999);
			m_PasswordTextInput->setAlignment(Qt::AlignCenter);
			m_PasswordTextInput->setPlaceholderText(QApplication::translate("LinkedOutClass", "Password", nullptr));
		}

		//To login page label
		{
			m_ToLoginLabel = new ClickableLabel(m_Frame);
			m_ToLoginLabel->setObjectName(QString::fromStdString("ToLoginLabel"));
			m_ToLoginLabel->setGeometry(QRect(0, 300, 380, 41));
			m_ToLoginLabel->setAlignment(Qt::AlignHCenter| Qt::AlignVCenter);
			m_ToLoginLabel->setText(QApplication::translate("LinkedOutClass", "Already have an account? Login!", nullptr));
			m_ToLoginLabel->setStyleSheet("border-top:none;border-bottom:none;border-radius:0px;");
		}

		//Captcha frame
		{
			m_CaptchaFrame = new QFrame(m_Frame);
			m_CaptchaFrame->setObjectName(QString::fromUtf8("Captcha_Frame"));
			m_CaptchaFrame->setGeometry(QRect(0, 340, 380, 61));
			sizePolicy1.setHeightForWidth(m_CaptchaFrame->sizePolicy().hasHeightForWidth());
			m_CaptchaFrame->setSizePolicy(sizePolicy1);
			m_CaptchaFrame->setStyleSheet(QString::fromUtf8("border-radius:0;border-top:none;border-bottom:none;\n"
				""));
			m_CaptchaFrame->setFrameShape(QFrame::NoFrame);
			m_CaptchaFrame->setFrameShadow(QFrame::Plain);
			m_CaptchaFrame->setLineWidth(0);
		}

		//Captcha label
		{
			m_CaptchaLabel = new QLabel(m_CaptchaFrame);
			m_CaptchaLabel->setObjectName(QString::fromUtf8("Captcha_Label"));
			m_CaptchaLabel->setGeometry(QRect(0, 10, 211, 41));
			m_CaptchaLabel->setStyleSheet(QString::fromUtf8("border-radius:0;border-top:none;border-bottom:none;font-style:italic;font-weight:bold;font-size:25px;border-right:none;\n"
				""));
			m_CaptchaLabel->setAlignment(Qt::AlignCenter);
		}

		//Captcha text input
		{
			m_CaptchaTextInput = new QLineEdit(m_CaptchaFrame);
			m_CaptchaTextInput->setObjectName(QString::fromUtf8("Captcha_Text_Input"));
			m_CaptchaTextInput->setGeometry(QRect(210, 10, 151, 41));
			m_CaptchaTextInput->setAcceptDrops(false);
			m_CaptchaTextInput->setStyleSheet(QString::fromUtf8("border-style: solid; border-width: 2px; border-color: #000000;"));
			m_CaptchaValidator = new QIntValidator;
			m_CaptchaTextInput->setValidator(m_CaptchaValidator);
			m_CaptchaTextInput->setMaxLength(4);
			m_CaptchaTextInput->setAlignment(Qt::AlignCenter);
			m_CaptchaTextInput->setPlaceholderText(QApplication::translate("LinkedOutClass", "Captcha", nullptr));
		}

		//Signup button
		{
			m_Button = new QPushButton(m_Frame);
			m_Button->setObjectName(QString::fromUtf8("SignupButton"));
			m_Button->setGeometry(QRect(115, 440, 151, 41));
			m_Button->setAutoFillBackground(false);
			m_Button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
				"	border-style: solid; border-width: 2px; border-color: #000000;\n"
				"	border-radius: 20px;\n"
				"	background-color: rgb(86,152,144);\n"
				"}\n"
				"\n"
				"\n"
				"QPushButton:hover{\n"
				"	background-color: rgb(100,180,160);\n"
				"}\n"
				"QPushButton:disabled{\n"
				"background-color: rgb(120, 216, 192);"
				"}"));
			m_Button->setFlat(true);
			m_Button->setText(QApplication::translate("LinkedOutClass", "Sign up", nullptr));
		}

		SetupSignupEvents();
	}

	UserData SignupLayer::GetUserDataFromSignupForm() {
		
		std::string username = m_UsernameTextInput->text().toStdString();
		std::string password = m_PasswordTextInput->text().toStdString();

		UserData ret;
		ret.Username = username;
		ret.Password = password;

		return ret;
	}


	void SignupLayer::CleanupInputs()
	{
		m_PasswordTextInput->setText("");
		m_CaptchaTextInput->setText("");
		m_Button->setEnabled(false);
	}

	void SignupLayer::CleanAllInputs()
	{
		CleanupInputs();
		m_UsernameTextInput->setText("");
	}

	void SignupLayer::OnInputChanged(const QString&) {
		m_Button->setEnabled(UsernameHasInput() && PasswordHasInput() && CaptchaHasInput());
	}

	bool SignupLayer::UsernameHasInput()
	{
		return !m_UsernameTextInput->text().isEmpty();
	}

	bool SignupLayer::PasswordHasInput()
	{
		return !m_PasswordTextInput->text().isEmpty();
	}

	bool SignupLayer::CaptchaHasInput()
	{
		return !m_CaptchaTextInput->text().isEmpty();
	}

	void SignupLayer::SetupSignupEvents() {
		QObject::connect(m_Button, &QPushButton::clicked, [this]() {
			if (m_MainLayer->m_LastCaptchaNumber != m_CaptchaTextInput->text().toUInt()) {
				m_MainLayer->m_MessageLayer->Error(INCORRECT_CAPTCHA);
				CleanupInputs();
				return;
			}

			auto userData = GetUserDataFromSignupForm();
			if (userData.IsValid()) {

				SignupErrorCodes ec = m_MainLayer->Signup(userData);
				if (ec == SignupErrorCodes::None)
					return;
				switch (ec)
				{
				case LinkedOut::SignupErrorCodes::UserExists:
					m_MainLayer->m_MessageLayer->Error(USERNAME_EXISTS);
				}
			}
			else {
				m_MainLayer->m_MessageLayer->Error(INVALID_USERNAME_OR_PASSWORD);
			}
			m_MainLayer->SwitchToSignup(false);
		});

		QObject::connect(m_UsernameTextInput, &QLineEdit::textChanged, LO_BIND_FN(OnInputChanged));
		QObject::connect(m_PasswordTextInput, &QLineEdit::textChanged, LO_BIND_FN(OnInputChanged));
		QObject::connect(m_CaptchaTextInput, &QLineEdit::textChanged, LO_BIND_FN(OnInputChanged));



		QObject::connect(m_ToLoginLabel, &ClickableLabel::clicked, [this]() {
			m_MainLayer->SwitchToLogin(true);
			});

	}


}