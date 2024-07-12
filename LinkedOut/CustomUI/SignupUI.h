#pragma once

#include "Core/Post.h"
#include "Core/Person.h"

#include "Layers/PostUI.h"
#include "Layers/CommentUI.h"
#include "Layers/MainLayer.h"
#include "HomeLayerUI.h"
#include "Core/Random.h"

#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QMessageBox>
#include <QTextEdit>
#include <QStyle>

#include <QTextEdit>
#include <QGridLayout>
#include <QLineEdit>

#include <QWidget>

namespace LinkedOut {
	class SignupLayerUI : public QWidget{
		Q_OBJECT
	public:
		SignupLayerUI(QWidget* parent)
			:QWidget(parent)
		{}

		virtual void Show() = 0;
		virtual void Hide() = 0;

	private:

	};

	class PersonSignupLayerUI : public SignupLayerUI {
	public:
		PersonSignupLayerUI(QWidget* parent)
			:SignupLayerUI(parent)
		{
			m_Layout = new QVBoxLayout;
			setLayout(m_Layout);
			m_Layout->addStretch();
			//m_Layout->setAlignment(Qt::AlignCenter);
			setStyleSheet("background-color:rgb(51,52,54);");
			SetupUI();
			m_Layout->addStretch();
		}
		void Show() {
			show();
		}
		void Hide() {
			hide();
		}
	private:

		void SetupUI() {
			HDivision mainDiv(this);

			mainDiv.Layout->addStretch();

			QFrame* mainFrame = new QFrame(mainDiv.Widget);
			mainFrame->setStyleSheet("*{background-color:rgb(86, 152, 144);color:white;font-size:15px;border-radius:10px;}QLineEdit,QPushButton{border:2px solid white;}");
			QVBoxLayout* layout = new QVBoxLayout(mainFrame);

			QLineEdit* usernameEdit;
			QLineEdit* passwordEdit;
			QLabel* captchaLabel;
			QLineEdit* captchaEdit;

			int captcha;

			{
				auto label = new QLabel("Signup person");
				layout->addWidget(label);
				layout->addStretch();
			}

			{
				HDivision div(mainFrame);
				div.Layout->addWidget(new QLabel("Username:", div.Widget));
				div.Layout->addStretch();
				usernameEdit = new QLineEdit(div.Widget);
				div.Layout->addWidget(usernameEdit);
				layout->addWidget(div.Widget);
			}
			{
				HDivision div(mainFrame);
				div.Layout->addWidget(new QLabel("Password:", div.Widget));
				div.Layout->addStretch();
				passwordEdit = new QLineEdit(div.Widget);
				div.Layout->addWidget(passwordEdit);
				layout->addWidget(div.Widget);
			}
			{
				HDivision div(mainFrame);
				captchaLabel = new QLabel(div.Widget);
				captcha = Random::GenerateInt32(1000, 10000);
				captchaLabel->setText(QString("Captcha %1").arg(captcha));
				div.Layout->addWidget(captchaLabel);
				div.Layout->addStretch();
				captchaEdit = new QLineEdit(div.Widget);
				div.Layout->addWidget(captchaEdit);
				layout->addWidget(div.Widget);
			}

			auto cleanInput = [=]() {
				usernameEdit->setText("");
				passwordEdit->setText("");
				captchaEdit->setText("");
			};


			{
				auto btn = new QPushButton("Signup", mainFrame);
				layout->addStretch();
				layout->addWidget(btn);
				QObject::connect(btn, &QPushButton::clicked, [=]() {
					if (usernameEdit->text().isEmpty()) {
						MainLayer::Get().Error("Username field cant be empty");
						cleanInput();
						return;
					}
					if (passwordEdit->text().isEmpty()) {
						MainLayer::Get().Error("Password field cant be empty");
						cleanInput();
						return;
					}
					if (captchaEdit->text().isEmpty()) {
						MainLayer::Get().Error("Captcha field cant be empty");
						cleanInput();
						return;
					}
					if (captchaEdit->text().toInt() != captcha) {
						MainLayer::Get().Error("Incorrect captcha");
						cleanInput();
						return;
					}
					std::string username = usernameEdit->text().toStdString();
					std::string password = passwordEdit->text().toStdString();

					if (MainLayer::Get().AccountExists(username)) {
						MainLayer::Get().Error("Account with username already exists");
						cleanInput();
						return;
					}

					MainLayer::Get().SignupPerson(username, password);
					MainLayer::Get().SwitchToNavigation();
					});
			}

			mainDiv.Layout->addWidget(mainFrame);
			mainDiv.Layout->addStretch();
			m_Layout->addWidget(mainDiv.Widget);
		}

	private:
		QVBoxLayout* m_Layout;
	};

	class CompanySignupLayerUI : public SignupLayerUI {
	public:
		CompanySignupLayerUI(QWidget* parent)
			:SignupLayerUI(parent)
		{
			m_Layout = new QVBoxLayout;
			setLayout(m_Layout);
			m_Layout->addStretch();
			//m_Layout->setAlignment(Qt::AlignCenter);
			setStyleSheet("background-color:rgb(51,52,54);");
			SetupUI();
			m_Layout->addStretch();
		}
		void Show() {
			show();
		}
		void Hide() {
			hide();
		}
	private:

		void SetupUI() {
			HDivision mainDiv(this);

			mainDiv.Layout->addStretch();

			QFrame* mainFrame = new QFrame(mainDiv.Widget);
			mainFrame->setStyleSheet("*{background-color:rgb(86, 152, 144);color:white;font-size:15px;border-radius:10px;}QLineEdit,QPushButton{border:2px solid white;}");
			QVBoxLayout* layout = new QVBoxLayout(mainFrame);

			QLineEdit* usernameEdit;
			QLineEdit* passwordEdit;
			QLabel* captchaLabel;
			QLineEdit* captchaEdit;


			{
				auto label = new QLabel("Signup company");
				layout->addWidget(label);
				layout->addStretch();
			}

			{
				HDivision div(mainFrame);
				div.Layout->addWidget(new QLabel("Username:", div.Widget));
				div.Layout->addStretch();
				usernameEdit = new QLineEdit(div.Widget);
				div.Layout->addWidget(usernameEdit);
				layout->addWidget(div.Widget);
			}
			{
				HDivision div(mainFrame);
				div.Layout->addWidget(new QLabel("Password:", div.Widget));
				div.Layout->addStretch();
				passwordEdit = new QLineEdit(div.Widget);
				div.Layout->addWidget(passwordEdit);
				layout->addWidget(div.Widget);
			}
			int captcha;
			{
				HDivision div(mainFrame);
				captchaLabel = new QLabel(div.Widget);
				captcha = Random::GenerateInt32(1000, 10000);
				captchaLabel->setText(QString("Captcha %1").arg(captcha));
				div.Layout->addWidget(captchaLabel);
				div.Layout->addStretch();
				captchaEdit = new QLineEdit(div.Widget);
				div.Layout->addWidget(captchaEdit);
				layout->addWidget(div.Widget);
			}

			auto cleanInput = [=]() {
				usernameEdit->setText("");
				passwordEdit->setText("");
				captchaEdit->setText("");
				};


			{
				auto btn = new QPushButton("Signup", mainFrame);
				layout->addStretch();
				layout->addWidget(btn);
				QObject::connect(btn, &QPushButton::clicked, [=]() {
					if (usernameEdit->text().isEmpty()) {
						MainLayer::Get().Error("Username field cant be empty");
						cleanInput();
						return;
					}
					if (passwordEdit->text().isEmpty()) {
						MainLayer::Get().Error("Password field cant be empty");
						cleanInput();
						return;
					}
					if (captchaEdit->text().isEmpty()) {
						MainLayer::Get().Error("Captcha field cant be empty");
						cleanInput();
						return;
					}
					if (captchaEdit->text().toInt() != captcha) {
						MainLayer::Get().Error("Incorrect captcha");
						cleanInput();
						return;
					}
					std::string username = usernameEdit->text().toStdString();
					std::string password = passwordEdit->text().toStdString();

					if (MainLayer::Get().AccountExists(username)) {
						MainLayer::Get().Error("Account with username already exists");
						cleanInput();
						return;
					}

					MainLayer::Get().SignupCompany(username, password);
					MainLayer::Get().SwitchToNavigation();
					});
			}

			mainDiv.Layout->addWidget(mainFrame);
			mainDiv.Layout->addStretch();
			m_Layout->addWidget(mainDiv.Widget);
		}

	private:
		QVBoxLayout* m_Layout;
	};


}


