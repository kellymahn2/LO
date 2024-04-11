#include "SplashLayer.h"
#include "Core/Window.h"
#include "MainLayer.h"

#include <QMainWindow>
#include <qpushbutton.h>
#include "ui_LinkedOut.h"
#include <QStyle>
#include <QStandardPaths>
#include <qlabel.h>
#include <qlayout.h>
#include <QValidator>
#include <qvalidator.h>


namespace LinkedOut {


	SplashLayer::SplashLayer(MainLayer* mainLayer)
		:Layer("Splash layer")
	{
		m_MainLayer = mainLayer;
		SetupSplash();
	}
	SplashLayer::~SplashLayer(){
		Cleanup();
	}
	void SplashLayer::OnUpdate(){
		auto window = Window::GetMainWindow()->GetNativeHandle();
		auto size = window->size();
		m_MainFrame->resize(size);

		auto frameSize = m_Frame->size();

		float startX = ((float)size.width() - (float)frameSize.width()) / 2.0f;
		float startY = ((float)size.height() - (float)frameSize.height()) / 2.0f;


		m_Frame->setGeometry(startX, startY, frameSize.width(), frameSize.height());

	}
	void SplashLayer::Show(){
		m_MainFrame->show();
		m_IsShowing = true;
	}
	void SplashLayer::Hide(){
		m_IsShowing = false;
		m_MainFrame->hide();
	}



	void SplashLayer::SetupSplash() {

		auto window = Window::GetMainWindow()->GetNativeHandle();

		QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);




		//Main frame
		{
			m_MainFrame = new QFrame(m_MainLayer->m_WindowCentralWidget);
			m_MainFrame->setObjectName(QString::fromUtf8("SplashMainFrame"));
			m_MainFrame->setGeometry(QRect(0, 0, 1280, 720));
			sizePolicy.setHeightForWidth(m_MainFrame->sizePolicy().hasHeightForWidth());
			m_MainFrame->setSizePolicy(sizePolicy);
			m_MainFrame->setSizeIncrement(QSize(0, 0));
			m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;width:inherited;"));
			m_MainFrame->setFrameShape(QFrame::StyledPanel);
			m_MainFrame->setFrameShadow(QFrame::Raised);
		}

		//Splash frame
		{
			m_Frame = new QFrame(m_MainFrame);
			m_Frame->setObjectName(QString::fromUtf8("Splash_Frame"));
			m_Frame->setFrameShape(QFrame::NoFrame);
			m_Frame->setFrameShadow(QFrame::Raised);
		}

#define WELCOME_TEXT "    LinkedOut is a professional networking platform for\n    individuals and businesses.It's ideal for job seekers,\n    businesses seeking to expand, and professionals aiming\n    to grow their careers. With features like job searching,\n    networking, and professional development resources, LinkedIn offers valuable opportunities for connection and advancement in various industries."

		//Welcome text
		{
			m_SplashWelcomeText = new QLabel(m_Frame);
			m_SplashWelcomeText->setObjectName(QString::fromUtf8("SplashWeclomeText"));
			m_SplashWelcomeText->setGeometry(QRect(120, 90, 500, 231));
			sizePolicy.setHeightForWidth(m_SplashWelcomeText->sizePolicy().hasHeightForWidth());
			m_SplashWelcomeText->setSizePolicy(sizePolicy);
			m_SplashWelcomeText->setStyleSheet(QString::fromUtf8("font: 75 italic 16pt \"Candara\";"));
			m_SplashWelcomeText->setTextFormat(Qt::PlainText);
			m_SplashWelcomeText->setScaledContents(false);
			m_SplashWelcomeText->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);
			m_SplashWelcomeText->setWordWrap(true);
			m_SplashWelcomeText->setText(QApplication::translate("window", "Welcome to LinkedOut!\n"
				"\n"
				"\n"
				"\n"
				WELCOME_TEXT, nullptr));
		}

		//Weclome image
		{
			m_SplashWelcomeImage = new QLabel(m_Frame);
			m_SplashWelcomeImage->setObjectName(QString::fromUtf8("SplashWelcomeImage"));
			m_SplashWelcomeImage->setGeometry(QRect(720, 70, 271, 231));
			m_SplashWelcomeImage->setFrameShape(QFrame::NoFrame);
			m_SplashWelcomeImage->setPixmap(QPixmap(QString::fromUtf8("Resources/Splash_Image.jpg")));
			m_SplashWelcomeImage->setScaledContents(true);
			m_SplashWelcomeImage->setText(QString());
		}


		
		
		//Signup button
		{
			m_SplashSignUpButton = new QPushButton(m_Frame);
			m_SplashSignUpButton->setObjectName(QString::fromUtf8("SplashSignupButton"));
			m_SplashSignUpButton->setGeometry(QRect(240, 460, 201, 51));
			m_SplashSignUpButton->setAutoFillBackground(false);
			m_SplashSignUpButton->setStyleSheet(QString::fromUtf8("QPushButton:hover{ 	background-color:rgba(38,38,39,220);color: white;}\n"
				"QPushButton{color:white;background-color:rgb(38, 38, 39);	border-style: none;border-radius: 20px;}"));
			m_SplashSignUpButton->setFlat(false);
			m_SplashSignUpButton->setText(QApplication::translate("window", "Sign Up", nullptr));


		}

		//Login button
		{

			m_SplashLoginButton = new QPushButton(m_Frame);
			m_SplashLoginButton->setObjectName(QString::fromUtf8("SplashLoginButton"));
			m_SplashLoginButton->setGeometry(QRect(470, 460, 201, 51));
			m_SplashLoginButton->setAutoFillBackground(false);
			m_SplashLoginButton->setStyleSheet(QString::fromUtf8("QPushButton:hover{ 	background-color:rgba(38,38,39,220);color: white;}\n"
				"QPushButton{color:white;background-color:rgb(38, 38, 39);	border-style: none;border-radius: 20px;}"));
			m_SplashLoginButton->setFlat(false);
			m_SplashLoginButton->setText(QApplication::translate("window", "Login", nullptr));
		}

		SetupSplashEvents();
	}

	void SplashLayer::SetupSplashEvents() {
		QObject::connect(m_SplashSignUpButton, &QPushButton::clicked, [this]() {
			m_MainLayer->SwitchToSignup(true);
			});
		QObject::connect(m_SplashLoginButton, &QPushButton::clicked, [this]() {
			m_MainLayer->SwitchToLogin(true);
			});
	}



	void SplashLayer::Cleanup() {
		//Login button
		{
			delete m_SplashLoginButton;
		}

		//Signup button
		{
			delete m_SplashSignUpButton;
		}

		//Welcome image
		{
			delete m_SplashWelcomeImage;
		}

		//Welcome text
		{
			delete m_SplashWelcomeText;
		}

		//Splash frame
		{
			delete m_Frame;
		}

		//Main frame
		{
			delete m_MainFrame;
		}
	}

}