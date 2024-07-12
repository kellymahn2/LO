#pragma once

#include "Core/Post.h"
#include "Core/Person.h"

#include "Layers/PostUI.h"
#include "Layers/CommentUI.h"
#include "Layers/MainLayer.h"
#include "HomeLayerUI.h"
#include "ComboboxList.h"

#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QMessageBox>
#include <QTextEdit>
#include <QStyle>

#include <QTextEdit>
#include <QLineEdit>

#include <QWidget>

namespace LinkedOut {

	class OtherProfilePanelUI : public QWidget{
		Q_OBJECT
	public:
		OtherProfilePanelUI(Ref<Account> account,bool showPassword,QWidget* parent)
			:QWidget(parent),m_Account(account)
		{
			m_Layout = new QVBoxLayout;
			setLayout(m_Layout);
			SetupUI(showPassword);
			setStyleSheet("background-color:rgb(51,52,54);color:white;");
		}

		virtual void Show() {
			show();
		}
		virtual void Hide(){
			hide();
		}
	private:
		void SetupUI(bool showPassword) {
			if(!m_Account->GetImagePath().empty()){
				QPixmap pixmap(m_Account->GetImagePath().c_str());
				if (!pixmap.isNull()) {
					QLabel* image = new QLabel(this);
					image->setPixmap(pixmap.scaled(QSize(100, 100), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
					image->setStyleSheet("border-radius:25px");
					m_Layout->addWidget(image);
				}
			}

			{
				std::string str = fmt::format("Username: {}", m_Account->GetUsername());

				QLabel* username = new QLabel(QString::fromStdString(str),this);
				auto followingIDs = MainLayer::Get().GetCurrentUser()->GetFollowingID();
				m_Layout->addWidget(username);
				if (std::find(followingIDs.begin(), followingIDs.end(), m_Account->GetAccountID()) != followingIDs.end()) {
					QPushButton* followBtn = new QPushButton("Follow", this);
					m_Layout->addWidget(followBtn);
					if (m_Account->GetAccountType() == AccountType::Company) {
						MainLayer::Get().Follow(MainLayer::Get().GetCurrentUser()->GetAccountID(), m_Account->GetAccountID());
					}
					else if (m_Account->GetAccountType() == AccountType::Person) {
						MainLayer::Get().RequestFollow(m_Account->GetAccountID());
					}
				}
			}
			if (showPassword) {
				std::string str = fmt::format("Password: {}", m_Account->GetPassword());
				QLabel* password = new QLabel(QString::fromStdString(str),this);
				m_Layout->addWidget(password);
			}
			{
				std::string str = fmt::format("Email: {}", m_Account->GetEmail());
				QLabel* email = new QLabel(QString::fromStdString(str), this);
				m_Layout->addWidget(email);
			}
			
			m_Account->Serialize(this);

			if(m_Account->GetPosts().size()){
				std::string str = fmt::format("Posts:");
				
				QScrollArea* scrollArea = new QScrollArea(this);
				QWidget* scrollWidget = new QWidget(scrollArea);
				scrollArea->setWidget(scrollWidget);
				scrollArea->setWidgetResizable(true);

				QVBoxLayout* scrollLayout = new QVBoxLayout(scrollWidget);
				//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
				scrollLayout->setDirection(QBoxLayout::TopToBottom);
				scrollWidget->setLayout(scrollLayout);

				for (auto& post : m_Account->GetPosts()) {
					QWidget* postWidget = new QWidget(scrollWidget);
					QVBoxLayout* layout = new QVBoxLayout;
					postWidget->setLayout(layout);
					
					post->Serialize(postWidget);
					postWidget->setStyleSheet("background-color:rgb(86, 152, 144);color:black;");
					postWidget->show();

					scrollLayout->addWidget(postWidget);
				}
				scrollLayout->addStretch();

				m_Layout->addWidget(scrollArea);
			}
		}
	protected:
		Ref<Account> m_Account;
		QVBoxLayout* m_Layout;
		PostsLayer* m_PostsLayer;
	};

	class OtherPersonProfilePanelUI : public OtherProfilePanelUI {
	public:
		OtherPersonProfilePanelUI(Ref<Person> person, bool showPassword, QWidget* parent)
			:OtherProfilePanelUI(person, showPassword, parent)
		{
		}
	private:

	};

	class OtherCompanyProfilePanelUI : public OtherProfilePanelUI {
	public:
		OtherCompanyProfilePanelUI(Ref<Company> company, bool showPassword, QWidget* parent)
			:OtherProfilePanelUI(company, showPassword, parent)
		{
		}
	private:

	};


	class ProfilePanelUI : public QWidget {
		Q_OBJECT
	public:
		ProfilePanelUI(bool showPassword, QWidget* parent)
			:QWidget(parent)
		{
		}

		virtual void Show() {
			show();
		}
		virtual void Hide() {
			hide();
		}

	};

	class PersonProfilePanelUI : public ProfilePanelUI {
	public:
		PersonProfilePanelUI(Ref<Person> person, bool showPassword, QWidget* parent)
			:ProfilePanelUI(showPassword, parent), m_Person(person)
		{
			
			SetupUI(showPassword);
			setStyleSheet("background-color:rgb(51,52,54);color:white;");
			//setStyleSheet("background-color:red;");
			m_Layout->setContentsMargins(0, 0, 0, 0);
		}
	private:
		void SetupUI(bool showPassword) {

			m_Layout = new QVBoxLayout;
			m_MainFrame = new QFrame(this);
			m_MainFrameLayout = new QVBoxLayout;


			QPixmap pixmap(m_Person->GetImagePath().c_str());
			if (pixmap.isNull()) {
				pixmap = QPixmap("Resources/NavigationProfileOpenIcon.png");
			}
			if (!pixmap.isNull()) {

				ClickableLabel* image = new ClickableLabel(m_MainFrame);
				image->setPixmap(pixmap.scaled(QSize(100, 100), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
				image->setStyleSheet("border-radius:25px");
				m_MainFrameLayout->addWidget(image);
				QObject::connect(image, &ClickableLabel::clicked, [this, image]() {
					auto path = OpenFileDialog("Images (*.png *.jpg *.jpeg)");
					if (path.has_value())
					{
						m_Person->SetImagePath(path.value());
						QPixmap pixmap(path.value().c_str());
						image->setPixmap(pixmap.scaled(QSize(100, 100), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
					}
				});
			}

			{
				std::string str = fmt::format("Username: {}", m_Person->GetUsername());
				m_MainFrameLayout->addWidget(new QLabel(QString::fromStdString(str), m_MainFrame));

				auto followingIDs = MainLayer::Get().GetCurrentUser()->GetFollowingID();

				if (std::find(followingIDs.begin(), followingIDs.end(), m_Person->GetAccountID()) != followingIDs.end()) {
					QPushButton* followBtn = new QPushButton("Follow", m_MainFrame);
					m_MainFrameLayout->addWidget(followBtn);
					if (m_Person->GetAccountType() == AccountType::Company) {
						MainLayer::Get().Follow(MainLayer::Get().GetCurrentUser()->GetAccountID(), m_Person->GetAccountID());
					}
					else if (m_Person->GetAccountType() == AccountType::Person) {
						MainLayer::Get().RequestFollow(m_Person->GetAccountID());
					}
				}
			}
			if (showPassword) {
				std::string str = fmt::format("Password: {}", m_Person->GetPassword());
				QLabel* password = new QLabel(QString::fromStdString(str), m_MainFrame);
				m_MainFrameLayout->addWidget(password);
			}

			QLineEdit* firstnameEdit;
			QLineEdit* lastnameEdit;
			QLineEdit* emailEdit;
			ComboBoxList* combo;

			{
				HDivision div(m_MainFrame);
				div.Layout->addWidget(new QLabel("First name:", div.Widget));
				firstnameEdit = new QLineEdit(div.Widget);
				firstnameEdit->setText(QString::fromStdString(m_Person->GetFirstName()));
				div.Layout->addWidget(firstnameEdit);
				m_MainFrameLayout->addWidget(div.Widget);
			}
			{
				HDivision div(m_MainFrame);
				div.Layout->addWidget(new QLabel("Last name:", div.Widget));
				lastnameEdit = new QLineEdit(div.Widget);
				lastnameEdit->setText(QString::fromStdString(m_Person->GetLastName()));
				div.Layout->addWidget(lastnameEdit);
				m_MainFrameLayout->addWidget(div.Widget);
			}
			{
				HDivision div(m_MainFrame);
				div.Layout->addWidget(new QLabel("Email:", div.Widget));
				emailEdit = new QLineEdit(div.Widget);
				emailEdit->setText(QString::fromStdString(m_Person->GetEmail()));
				div.Layout->addWidget(emailEdit);
				m_MainFrameLayout->addWidget(div.Widget);
			}
			{
				HDivision div(m_MainFrame);
				{
					std::string skills = fmt::format("Skills:");
					auto label = new QLabel(QString::fromStdString(skills), div.Widget);
					div.Layout->addWidget(label);

					combo = new ComboBoxList(div.Widget);

					for (auto& skill : PossibleJobSkillTags) {
						combo->AddItem(skill);
					}

					for (auto& skill : m_Person->GetSkills()) {
						combo->Select(skill);
					}

					div.Layout->addStretch();
					div.Layout->addWidget(combo);
				}

				m_MainFrameLayout->addWidget(div.Widget);

			}

			{
				HDivision div(m_MainFrame);
				div.Layout->addStretch();
				auto submitBtn = new QPushButton("Submit", div.Widget);

				QObject::connect(submitBtn, &QPushButton::clicked, [=]() {
					MainLayer::Get().UpdatePerson(m_Person, m_Person->GetImagePath(), firstnameEdit->text().toStdString(), lastnameEdit->text().toStdString(), emailEdit->text().toStdString(),combo->GetSelected());
					});

				div.Layout->addWidget(submitBtn);
				m_MainFrameLayout->addWidget(div.Widget);
			}

			if (m_Person->GetPosts().size()) {
				std::string str = fmt::format("Posts:");

				QScrollArea* scrollArea = new QScrollArea(m_MainFrame);
				QWidget* scrollWidget = new QWidget(scrollArea);
				scrollArea->setWidget(scrollWidget);
				scrollArea->setWidgetResizable(true);

				QVBoxLayout* scrollLayout = new QVBoxLayout(scrollWidget);
				//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
				scrollLayout->setDirection(QBoxLayout::TopToBottom);
				scrollWidget->setLayout(scrollLayout);

				for (auto& post : m_Person->GetPosts()) {
					QWidget* postWidget = new QWidget(scrollWidget);
					QVBoxLayout* layout = new QVBoxLayout;
					postWidget->setLayout(layout);

					post->Serialize(postWidget);
					postWidget->setStyleSheet("background-color:rgb(86, 152, 144);color:black;");
					postWidget->show();

					scrollLayout->addWidget(postWidget);
				}
				scrollLayout->addStretch();

				m_MainFrameLayout->addWidget(scrollArea);
			}

			m_MainFrame->setLayout(m_MainFrameLayout);
			m_Layout->addWidget(m_MainFrame);

			setLayout(m_Layout);
		}
	private:
		QVBoxLayout* m_Layout;
		PostsLayer* m_PostsLayer;
		QFrame* m_MainFrame;
		QVBoxLayout* m_MainFrameLayout;
		Ref<Person> m_Person;
	};

	class CompanyProfilePanelUI : public ProfilePanelUI {
	public:
		CompanyProfilePanelUI(Ref<Company> company, bool showPassword, QWidget* parent)
			:ProfilePanelUI(showPassword, parent), m_Company(company)
		{
			
			SetupUI(showPassword);
			setStyleSheet("background-color:rgb(51,52,54);color:white;");
		}
	private:
		void SetupUI(bool showPassword) {
			m_Layout = new QVBoxLayout;
			setLayout(m_Layout);
			m_MainFrame = new QFrame(this);
			m_Layout->addWidget(m_MainFrame);
			m_MainFrameLayout = new QVBoxLayout;
			m_MainFrame->setLayout(m_MainFrameLayout);
			QPixmap pixmap(m_Company->GetImagePath().c_str());
			if (pixmap.isNull()) {
				pixmap = QPixmap("Resources/NavigationProfileOpenIcon.png");
			}
			if (!pixmap.isNull()) {
				ClickableLabel* image = new ClickableLabel(m_MainFrame);
				image->setPixmap(pixmap.scaled(QSize(100, 100), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
				image->setStyleSheet("border-radius:25px");
				m_MainFrameLayout->addWidget(image);
				QObject::connect(image, &ClickableLabel::clicked, [this, image]() {
					auto path = OpenFileDialog("Images (*.png *.jpg *.jpeg)");
					if (path.has_value())
					{
						m_Company->SetImagePath(path.value());
						QPixmap pixmap(path.value().c_str());
						image->setPixmap(pixmap.scaled(QSize(100, 100), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
					}
				});
			}

			{
				std::string str = fmt::format("Username: {}", m_Company->GetUsername());
				m_MainFrameLayout->addWidget(new QLabel(QString::fromStdString(str), m_MainFrame));

				auto followingIDs = MainLayer::Get().GetCurrentUser()->GetFollowingID();

				if (std::find(followingIDs.begin(), followingIDs.end(), m_Company->GetAccountID()) != followingIDs.end()) {
					QPushButton* followBtn = new QPushButton("Follow", m_MainFrame);
					m_MainFrameLayout->addWidget(followBtn);
					if (m_Company->GetAccountType() == AccountType::Company) {
						MainLayer::Get().Follow(MainLayer::Get().GetCurrentUser()->GetAccountID(), m_Company->GetAccountID());
					}
					else if (m_Company->GetAccountType() == AccountType::Person) {
						MainLayer::Get().RequestFollow(m_Company->GetAccountID());
					}
				}
			}
			if (showPassword) {
				std::string str = fmt::format("Password: {}", m_Company->GetPassword());
				QLabel* password = new QLabel(QString::fromStdString(str), m_MainFrame);
				m_MainFrameLayout->addWidget(password);
			}

			QLineEdit* nameEdit;
			QLineEdit* phoneEdit;
			QLineEdit* emailEdit;

			{
				HDivision div(m_MainFrame);
				div.Layout->addWidget(new QLabel("Company name:", div.Widget));
				nameEdit = new QLineEdit(div.Widget);
				nameEdit->setText(QString::fromStdString(m_Company->GetCompanyName()));
				div.Layout->addWidget(nameEdit);
				m_MainFrameLayout->addWidget(div.Widget);
			}
			{
				HDivision div(m_MainFrame);
				div.Layout->addWidget(new QLabel("Phone number:", div.Widget));
				phoneEdit = new QLineEdit(div.Widget);
				phoneEdit->setText(QString::fromStdString(m_Company->GetPhoneNumber()));
				div.Layout->addWidget(phoneEdit);
				m_MainFrameLayout->addWidget(div.Widget);
			}
			{
				HDivision div(m_MainFrame);
				div.Layout->addWidget(new QLabel("Email:", div.Widget));
				emailEdit = new QLineEdit(div.Widget);
				emailEdit->setText(QString::fromStdString(m_Company->GetEmail()));
				div.Layout->addWidget(emailEdit);
				m_MainFrameLayout->addWidget(div.Widget);
			}

			{
				HDivision div(m_MainFrame);
				div.Layout->addStretch();
				auto submitBtn = new QPushButton("Submit", div.Widget);

				QObject::connect(submitBtn, &QPushButton::clicked, [=]() {
					MainLayer::Get().UpdateCompany(m_Company, nameEdit->text().toStdString(), m_Company->GetImagePath(), phoneEdit->text().toStdString(), emailEdit->text().toStdString());
					});

				div.Layout->addWidget(submitBtn);
				m_MainFrameLayout->addWidget(div.Widget);
			}

			if (m_Company->GetPosts().size()) {
				std::string str = fmt::format("Posts:");

				QScrollArea* scrollArea = new QScrollArea(m_MainFrame);
				QWidget* scrollWidget = new QWidget(scrollArea);
				scrollArea->setWidget(scrollWidget);
				scrollArea->setWidgetResizable(true);

				QVBoxLayout* scrollLayout = new QVBoxLayout(scrollWidget);
				//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
				scrollLayout->setDirection(QBoxLayout::TopToBottom);
				scrollWidget->setLayout(scrollLayout);

				for (auto& post : m_Company->GetPosts()) {
					QWidget* postWidget = new QWidget(scrollWidget);
					QVBoxLayout* layout = new QVBoxLayout;
					postWidget->setLayout(layout);

					post->Serialize(postWidget);
					postWidget->setStyleSheet("background-color:rgb(86, 152, 144);color:black;");
					postWidget->show();

					scrollLayout->addWidget(postWidget);
				}
				scrollLayout->addStretch();

				m_MainFrameLayout->addWidget(scrollArea);
			}
		}
	private:

		QVBoxLayout* m_Layout;
		PostsLayer* m_PostsLayer;
		QFrame* m_MainFrame;
		QVBoxLayout* m_MainFrameLayout;
		Ref<Company> m_Company;
	};

}