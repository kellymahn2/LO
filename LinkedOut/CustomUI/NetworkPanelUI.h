#pragma once


#include "Core/Post.h"
#include "Core/Person.h"

#include "Layers/PostUI.h"
#include "Layers/CommentUI.h"
#include "Layers/MainLayer.h"
#include "ProfilePanelUI.h"

#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QMessageBox>
#include <QTextEdit>
#include <QStyle>

#include <QTextEdit>

#include <QWidget>


namespace LinkedOut {
	class RequestUI : public QWidget {
		Q_OBJECT
	public:
		RequestUI(Ref<Person> acc, QWidget* parent)
			:QWidget(parent), m_Account(acc)
		{
			SetupRequestUI(parent);
			setLayout(m_MainDiv);
		}
		~RequestUI() {
			delete m_MainDiv;
		}
	private:
		void SetupRequestUI(QWidget* parent) {
			m_MainDiv = new QHBoxLayout(parent);
			auto label = new ClickableLabel(this);
			label->setText(QString::fromStdString(m_Account->GetUsername()));

			QObject::connect(label, &ClickableLabel::clicked, [this]() {
				PopupWindow* window = new PopupWindow(640, 480, this, PopupWindowFlagBit_Popup);
				QVBoxLayout* layout = new QVBoxLayout;
				window->setLayout(layout);
				auto ui = m_Account->MakeOtherProfilePanelUI(window);
				layout->addWidget(ui);
				ui->Show();
				window->show();
			});

			m_MainDiv->addWidget(label);
			m_MainDiv->addStretch();
			auto btn = new QPushButton("Accept", this);
			QObject::connect(btn, &QPushButton::clicked, [this]() {
				MainLayer::Get().AcceptFollow(m_Account);
				MainLayer::Get().GetNavMenu()->Refresh();
			});
			m_MainDiv->addWidget(btn);
		}
	private:
		Ref<Person> m_Account;

		QHBoxLayout* m_MainDiv;
	};

	class SuggestUI : public QWidget {
		Q_OBJECT

	public:
		SuggestUI(Ref<Person> acc, QWidget* parent)
			:QWidget(parent), m_Account(acc)
		{
			SetupSuggestUI(parent);
			setLayout(m_MainDiv);
		}
		~SuggestUI() {
			delete m_MainDiv;
		}
	private:
		void SetupSuggestUI(QWidget* parent) {
			m_MainDiv = new QHBoxLayout;
			auto label = new ClickableLabel(this);
			label->setText(QString::fromStdString(m_Account->GetUsername()));
			QObject::connect(label, &ClickableLabel::clicked, [this]() {
				PopupWindow* window = new PopupWindow(640, 480, this, PopupWindowFlagBit_Popup);
				QVBoxLayout* layout = new QVBoxLayout;
				window->setLayout(layout);
				auto ui = m_Account->MakeOtherProfilePanelUI(window);
				layout->addWidget(ui);
				ui->Show();
				window->show();
			});

			m_MainDiv->addWidget(label);
			auto btn = new QPushButton("Follow", this);
			QPushButton::connect(btn, &QPushButton::clicked, [=]() {
				MainLayer::Get().RequestFollow(m_Account->GetAccountID());
				MainLayer::Get().GetNavMenu()->Refresh();
			});
			m_MainDiv->addWidget(btn);
			m_MainDiv->addStretch();
		}
	private:
		Ref<Person> m_Account;
		QHBoxLayout* m_MainDiv;
	};

	class FollowUI : public QWidget {
		Q_OBJECT

	public:
		FollowUI(Ref<Person> acc, QWidget* parent)
			:QWidget(parent), m_Account(acc)
		{
			SetupFollowUI(parent);
			setLayout(m_MainDiv);
		}
		~FollowUI() {
			delete m_MainDiv;
		}
	private:
		void SetupFollowUI(QWidget* parent) {
			m_MainDiv = new QHBoxLayout(parent);
			auto label = new ClickableLabel(this);
			label->setText(QString::fromStdString(m_Account->GetUsername()));
			QObject::connect(label, &ClickableLabel::clicked, [this]() {
				PopupWindow* window = new PopupWindow(640, 480, this,PopupWindowFlagBit_Popup);
				QVBoxLayout* layout = new QVBoxLayout;
				window->setLayout(layout);
				auto ui = m_Account->MakeOtherProfilePanelUI(window);
				layout->addWidget(ui);
				ui->Show();
				window->show();
			});
			m_MainDiv->addWidget(label);
			
			auto followingID = MainLayer::Get().GetCurrentUser()->GetFollowingID();
			if (std::find(followingID.begin(),followingID.end(),m_Account->GetAccountID()) == followingID.end()) {
				auto btn = new QPushButton("Follow",this);
				m_MainDiv->addWidget(btn);
				QObject::connect(btn, &QPushButton::clicked, [btn,this]() {
					MainLayer::Get().Follow(MainLayer::Get().GetCurrentUser()->GetAccountID(), m_Account->GetAccountID());
					MainLayer::Get().GetCurrentUser()->Follow(m_Account->GetAccountID());
					btn->deleteLater();
				});
			}
			
		}
	private:
		Ref<Person> m_Account;
		QHBoxLayout* m_MainDiv;
	};

	class NetworkPanelUI : public QWidget {
		Q_OBJECT
	public:

		NetworkPanelUI(QWidget* parent)
			:QWidget(parent)
		{}
		virtual ~NetworkPanelUI() {}

		virtual void Show() = 0;

		virtual void Hide() = 0;
	};

	class PersonNetworkRequestsLayer : public QWidget {
		Q_OBJECT
	public:
		PersonNetworkRequestsLayer(QWidget* parent)
			:QWidget(parent)
		{
			m_MainLayout = new QVBoxLayout(this);

			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			m_LoadMoreLabel = new ClickableLabel(m_ScrollWidget);
			m_LoadMoreLabel->setText("More...");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
				});

			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();
			m_MainLabel = new QLabel("Requests", this);
			m_MainLabel->setStyleSheet("font-weight:bold;font-style:italic;font-size:8px");
			m_MainLayout->addWidget(m_MainLabel);
			m_MainLayout->addWidget(m_ScrollArea);
		}

		void AddRequest(Ref<Person> acc) {
			m_Accounts.push_back(acc);
		}

		void ShowMore(uint32_t amount) {
			uint32_t shouldShow = std::min(m_CurrentShown + amount, (uint32_t)m_Accounts.size());

			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
				QLayoutItem* item = scrollLayout->itemAt(scrollLayout->count() - 1);
				scrollLayout->removeItem(item);
			}

			uint32_t currentShown = m_CurrentShown;

			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());

			//Show normal posts
			for (; currentShown < m_Accounts.size(); ++currentShown) {
				RequestUI* job = new RequestUI(m_Accounts[currentShown], m_ScrollWidget);
				//QLabel* job = new QLabel("Job", m_ScrollWidget);
				if (scrollLayout) {
					scrollLayout->addWidget(job);
				}
			}

			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
				scrollLayout->addWidget(m_LoadMoreLabel);
				scrollLayout->addStretch();
			}

			m_CurrentShown = shouldShow;
		}


		void ClearPosts() {
			m_Accounts.clear();

			delete m_MainLabel;
			delete m_ScrollWidget;
			delete m_ScrollArea;

			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			m_LoadMoreLabel = new ClickableLabel(m_ScrollWidget);
			m_LoadMoreLabel->setText("More...");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
				});

			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();
			m_MainLabel = new QLabel("Requests", this);
			m_MainLabel->setStyleSheet("font-weight:bold;font-style:italic;font-size:8px");
			m_MainLayout->addWidget(m_MainLabel);
			m_MainLayout->addWidget(m_ScrollArea);

			m_CurrentShown = 0;
		}


	private:

		QLabel* m_MainLabel;


		QVBoxLayout* m_MainLayout;
		QScrollArea* m_ScrollArea;
		QWidget* m_ScrollWidget;
		ClickableLabel* m_LoadMoreLabel;
		std::vector<Ref<Person>> m_Accounts;
		uint32_t m_CurrentShown = 0;
	};

	class PersonNetworkSuggestsLayer : public QWidget {
		Q_OBJECT
	public:
		PersonNetworkSuggestsLayer(QWidget* parent)
			:QWidget(parent)
		{
			m_MainLayout = new QVBoxLayout(this);
			
			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			m_LoadMoreLabel = new ClickableLabel(m_ScrollWidget);
			m_LoadMoreLabel->setText("More...");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
				});

			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();

			m_MainLabel = new QLabel("Suggestions", this);
			m_MainLabel->setStyleSheet("font-weight:bold;font-style:italic;font-size:8px");
			m_MainLayout->addWidget(m_MainLabel);
			m_MainLayout->addWidget(m_ScrollArea);
		}


		void AddSuggest(Ref<Person> acc) {
			m_Accounts.push_back(acc);
		}

		void ShowMore(uint32_t amount) {
			uint32_t shouldShow = std::min(m_CurrentShown + amount, (uint32_t)m_Accounts.size());

			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
				QLayoutItem* item = scrollLayout->itemAt(scrollLayout->count() - 1);
				scrollLayout->removeItem(item);
			}

			uint32_t currentShown = m_CurrentShown;

			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());

			//Show normal posts
			for (; currentShown < m_Accounts.size(); ++currentShown) {
				SuggestUI* job = new SuggestUI(m_Accounts[currentShown], m_ScrollWidget);
				//QLabel* job = new QLabel("Job", m_ScrollWidget);
				if (scrollLayout) {
					scrollLayout->addWidget(job);
				}
			}

			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
				scrollLayout->addWidget(m_LoadMoreLabel);
				scrollLayout->addStretch();
			}

			m_CurrentShown = shouldShow;
		}


		void ClearPosts() {
			m_Accounts.clear();

			delete m_MainLabel;
			delete m_ScrollWidget;
			delete m_ScrollArea;

			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			m_LoadMoreLabel = new ClickableLabel(m_ScrollWidget);
			m_LoadMoreLabel->setText("More...");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
				});

			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();
			m_MainLabel = new QLabel("Suggestions", this);
			m_MainLabel->setStyleSheet("font-weight:bold;font-style:italic;font-size:8px");
			m_MainLayout->addWidget(m_MainLabel);
			m_MainLayout->addWidget(m_ScrollArea);

			m_CurrentShown = 0;
		}


	private:
		QLabel* m_MainLabel;
		QVBoxLayout* m_MainLayout;
		QScrollArea* m_ScrollArea;
		QWidget* m_ScrollWidget;
		ClickableLabel* m_LoadMoreLabel;
		std::vector<Ref<Person>> m_Accounts;
		uint32_t m_CurrentShown = 0;
	};

	class PersonNetworkPanelUI : public NetworkPanelUI {
	public:
		PersonNetworkPanelUI(Ref<Person>p,QWidget* parent)
			:NetworkPanelUI(parent),m_Person(p)
		{
			SetupNetwork();
		}

		void Show() override {

			show();
			m_MainFrame->show();
			m_RequestsLayer->ClearPosts();
			m_SuggestsLayer->ClearPosts();

			auto requests = MainLayer::Get().GetUserRequests(m_Person);

			for (auto& request : requests) {
				m_RequestsLayer->AddRequest(request);
			}

			m_RequestsLayer->ShowMore(10);


			auto suggests = MainLayer::Get().GetUserSuggestions(m_Person);

			for (auto& suggest : suggests) {
				m_SuggestsLayer->AddSuggest(suggest);
			}

			m_SuggestsLayer->ShowMore(10);
		}

		void Hide() override {
			hide();
			m_MainFrame->hide();
		}

	private:
		void SetupNetwork() {
			QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

			//Main frame
			{
				m_MainFrame = new QFrame(this);
				m_MainFrame->setObjectName(QString::fromUtf8("Home_MainFrame"));
				m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;"));
				m_MainFrameLayout = new QHBoxLayout;
				m_MainFrameLayout->setContentsMargins(0, 0, 0, 0);
			}

			m_RequestsLayer = new PersonNetworkRequestsLayer(m_MainFrame);
			m_SuggestsLayer = new PersonNetworkSuggestsLayer(m_MainFrame);

			m_MainFrameLayout->addWidget(m_RequestsLayer);
			m_MainFrameLayout->addWidget(m_SuggestsLayer);

			//m_MainFrameLayout->addWidget(m_SuggestsLayout.Widget);

			m_MainFrame->setLayout(m_MainFrameLayout);
			m_MainLayout = new QVBoxLayout(this);
			m_MainLayout->addWidget(m_MainFrame);
		}
	private:
		QVBoxLayout* m_MainLayout;
		QFrame* m_MainFrame;
		QHBoxLayout* m_MainFrameLayout;

		PersonNetworkRequestsLayer* m_RequestsLayer;
		PersonNetworkSuggestsLayer* m_SuggestsLayer;


		Ref<Person> m_Person;
	};

	class CompanyNetworkFollowsLayer : public QWidget {
		Q_OBJECT
	public:
		CompanyNetworkFollowsLayer(QWidget* parent)
			:QWidget(parent)
		{
			m_MainLayout = new QVBoxLayout(this);

			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			m_LoadMoreLabel = new ClickableLabel(m_ScrollWidget);
			m_LoadMoreLabel->setText("More...");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
				});

			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();

			
			m_MainLayout->addWidget(m_ScrollArea);
		}
		void AddAccount(Ref<Person> acc) {
			m_Accounts.push_back(acc);
		}

		void ShowMore(uint32_t amount) {
			uint32_t shouldShow = std::min(m_CurrentShown + amount, (uint32_t)m_Accounts.size());

			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
				QLayoutItem* item = scrollLayout->itemAt(scrollLayout->count() - 1);
				scrollLayout->removeItem(item);
			}

			uint32_t currentShown = m_CurrentShown;

			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());

			//Show normal posts
			for (; currentShown < m_Accounts.size(); ++currentShown) {
				FollowUI* job = new FollowUI(m_Accounts[currentShown], m_ScrollWidget);

				if (scrollLayout) {
					scrollLayout->addWidget(job);
				}
			}

			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
				scrollLayout->addWidget(m_LoadMoreLabel);
				scrollLayout->addStretch();
			}

			m_CurrentShown = shouldShow;
		}


		void ClearPosts() {
			m_Accounts.clear();

			delete m_ScrollWidget;
			delete m_ScrollArea;

			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			m_LoadMoreLabel = new ClickableLabel(m_ScrollWidget);
			m_LoadMoreLabel->setText("More...");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
				});

			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();
			
			m_MainLayout->addWidget(m_ScrollArea);
			m_CurrentShown = 0;
		}


	private:
		QVBoxLayout* m_MainLayout;
		QScrollArea* m_ScrollArea;
		QWidget* m_ScrollWidget;
		ClickableLabel* m_LoadMoreLabel;
		std::vector<Ref<Person>> m_Accounts;
		uint32_t m_CurrentShown = 0;
	};

	class CompanyNetworkPanelUI : public NetworkPanelUI {
	public:

		CompanyNetworkPanelUI(Ref<Company> c, QWidget* parent)
			:NetworkPanelUI(parent), m_Company(c)
		{
			SetupNetwork();
		}

		void Show() {
			show();
			m_MainFrame->show();
			m_FollowsLayer->ClearPosts();

			auto followers = MainLayer::Get().GetCompanyFollowers(m_Company);

			for (auto& follower : followers) {
				m_FollowsLayer->AddAccount(follower);
			}

			m_FollowsLayer->ShowMore(10);
		}

		void Hide() {
			hide();
			m_MainFrame->hide();
		}

	private:

		void SetupNetwork() {
			QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

			//Main frame
			{
				m_MainFrame = new QFrame(this);
				m_MainFrame->setObjectName(QString::fromUtf8("Home_MainFrame"));
				m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;"));
				m_MainFrameLayout = new QHBoxLayout;
				m_MainFrameLayout->setContentsMargins(0, 0, 0, 0);
			}

			m_FollowsLayer = new CompanyNetworkFollowsLayer(m_MainFrame);
			m_MainFrameLayout->addWidget(m_FollowsLayer);

			/*m_MainFrameLayout->addWidget(m_RequestsLayer);
			m_MainFrameLayout->addWidget(m_SuggestsLayer);*/

			//m_MainFrameLayout->addWidget(m_SuggestsLayout.Widget);

			m_MainFrame->setLayout(m_MainFrameLayout);
			m_MainLayout = new QVBoxLayout(this);
			m_MainLayout->addWidget(m_MainFrame);
		}

	private:

		QVBoxLayout* m_MainLayout;
		QFrame* m_MainFrame;
		QHBoxLayout* m_MainFrameLayout;

		CompanyNetworkFollowsLayer* m_FollowsLayer;

		Ref<Company> m_Company;
	};

}