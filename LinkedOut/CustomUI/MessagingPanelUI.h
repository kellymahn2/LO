#pragma once

#include "Core/Post.h"
#include "Core/Person.h"
#include "Core/Company.h"
#include "CustomUI/TreeNode.h"

#include "Layers/PostUI.h"
#include "Layers/CommentUI.h"
#include "Layers/MainLayer.h"

#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QMessageBox>
#include <QTextEdit>
#include <QStyle>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QTimer>

#include <QWidget>

namespace LinkedOut {
	class DirectMessageUI : public QWidget {
		Q_OBJECT
	public:
		DirectMessageUI(Ref<Person> sender,Ref<DirectMessage> message, QWidget* parent)
			:QWidget(parent),m_Message(message),m_Sender(sender)
		{
			SetupDirectMessageUI();
			setLayout(m_Layout);
			//setStyleSheet("background-color:lightgreen;");
		}

		~DirectMessageUI() {
		
		}

		QFrame* GetMainFrame() { return m_MainFrame; }

	private:

		void SetupDirectMessageUI() {
			m_Layout = new QVBoxLayout;
			m_MainFrameLayout = new QVBoxLayout;

			m_MainFrame = new QFrame(this);

			QLabel* userName = new QLabel(QString::fromStdString(m_Sender->GetUsername()), m_MainFrame);
			m_MainFrameLayout->addWidget(userName);
			{
				auto& text = m_Message->GetContentText();
				auto loc = text.find("post_id:");
				if (loc != -1)
				{
					loc += 8;
					std::string post_id = text.substr(loc);
					if (MainLayer::Get().PostExists(post_id)) {

						ClickableLabel* label = new ClickableLabel(m_MainFrame);
						label->setText("Click to see post");
						m_MainFrameLayout->addWidget(label);

						QObject::connect(label, &ClickableLabel::clicked, [post_id, this]() {
							PopupWindow* window = new PopupWindow(640, 480, m_MainFrame, PopupWindowFlagBit_Popup);
							QVBoxLayout* layout = new QVBoxLayout;
							window->setLayout(layout);
							MainLayer::Get().GetPost(post_id)->Serialize(window);
							layout->addStretch();
							window->show();
							});
					}
					else {
						QLabel* textContent = new QLabel(QString::fromStdString(m_Message->GetContentText()), m_MainFrame);
						m_MainFrameLayout->addWidget(textContent);

						if (!m_Message->GetContentPicture().empty()) {
							QPixmap image(m_Message->GetContentPicture().c_str());
							QLabel* imageLabel = new QLabel(m_MainFrame);
							imageLabel->setPixmap(image);
							m_MainFrameLayout->addWidget(imageLabel);
						}

					}
				}
				else {
					QLabel* textContent = new QLabel(QString::fromStdString(m_Message->GetContentText()), m_MainFrame);
					m_MainFrameLayout->addWidget(textContent);

					if (!m_Message->GetContentPicture().empty()) {
						QPixmap image(m_Message->GetContentPicture().c_str());
						QLabel* imageLabel = new QLabel(m_MainFrame);
						imageLabel->setPixmap(image);
						m_MainFrameLayout->addWidget(imageLabel);
					}

				}

				
			}
			
			//if (!m_Message->GetContentVideo().empty()) {
			//	static QMediaPlayer* player = new QMediaPlayer(nullptr);
			//	QVideoWidget* videoWidget = new QVideoWidget(this);
			//	player->setVideoOutput(videoWidget);
			//	player->setMedia(QUrl::fromLocalFile(QString::fromStdString(m_Message->GetContentVideo())));
			//	videoWidget->setMinimumSize(320, 240);  // Adjust size as needed
			//	m_Layout->addWidget(videoWidget);
			//	player->play();
			//}

			auto& time = m_Message->GetTimeSent();
			std::string fmtTime = fmt::format("{}/{}/{} {}:{}", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
			QLabel* timeLabel = new QLabel(QString::fromStdString(fmtTime), m_MainFrame);
			m_MainFrameLayout->addWidget(timeLabel);
			m_MainFrame->setLayout(m_MainFrameLayout);
			m_Layout->addWidget(m_MainFrame);
		}
	private:
		Ref<DirectMessage> m_Message;
		Ref<Person> m_Sender;
		QVBoxLayout* m_Layout;
		QFrame* m_MainFrame;
		QVBoxLayout* m_MainFrameLayout;
	};

	class DirectMessageHistoryUI : public QWidget {
		Q_OBJECT
	public:
		DirectMessageHistoryUI(Ref<Person> p1,Ref<Person>p2,const std::vector<Ref<DirectMessage>>& messages,QWidget* parent)
			:QWidget(parent),m_P1(p1),m_P2(p2)
		{
			m_Layout = new QVBoxLayout;
			setLayout(m_Layout);
			SetupUI(messages);
		}
		void SetLastImagePath(const std::string& path) {
			m_LastImagePath = path;
		}

	private:
		void SetupUI(const std::vector<Ref<DirectMessage>>& messages) {

			{
				HDivision div1(this);
				div1.Layout->addWidget(new QLabel(QString::fromStdString(fmt::format("Chatting with {}",m_P2->GetUsername())), div1.Widget));
				div1.Layout->addStretch();
				m_Layout->addWidget(div1.Widget);
			}

			QScrollArea* scrollArea = new QScrollArea(this);
			QWidget* scrollWidget = new QWidget(scrollArea);
			scrollArea->setWidget(scrollWidget);
			scrollArea->setWidgetResizable(true);



			QVBoxLayout* scrollLayout = new QVBoxLayout(scrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);
			scrollWidget->setLayout(scrollLayout);

			for (auto& message : messages) {

				QWidget* widget = new QWidget(scrollWidget);
				QHBoxLayout* dmLayout = new QHBoxLayout;

				if (message->GetSenderID() == m_P1->GetAccountID()) {
					DirectMessageUI* ui = new DirectMessageUI(m_P1, message, widget);
					dmLayout->addStretch();
					dmLayout->addWidget(ui);
					ui->GetMainFrame()->setStyleSheet("background-color:rgb(0,92,75);color:white");
				}
				else if (message->GetSenderID() == m_P2->GetAccountID()) {
					DirectMessageUI* ui = new DirectMessageUI(m_P2, message, widget);
					ui->GetMainFrame()->setStyleSheet("background-color:rgb(32,44,51);color:white");
					dmLayout->addWidget(ui);
					dmLayout->addStretch();
				}

				widget->setLayout(dmLayout);
				scrollLayout->addWidget(widget);

			}

			

			m_Layout->addWidget(scrollArea);

			{
				VDivision div1(this);
				auto lineEdit = new QLineEdit(div1.Widget);

				div1.Layout->addWidget(lineEdit);

				HDivision div2(div1.Widget);
				auto imageBtn = new QPushButton("Image", div2.Widget);

				QObject::connect(imageBtn, &QPushButton::clicked, [this]() {
					m_LastImagePath = "";
					auto path = OpenFileDialog("Images (*.png *.jpg *.jpeg)");
					if (path.has_value())
						m_LastImagePath = path.value();
				});


				auto sendBtn = new QPushButton("Send", div2.Widget);

				connect(sendBtn, &QPushButton::clicked, div2.Widget,[scrollArea,scrollLayout,scrollWidget,lineEdit,this]() {
					Ref<DirectMessage> message = MainLayer::Get().SendMessage(m_P2, lineEdit->text().toStdString(), m_LastImagePath, "");

					QWidget* widget = new QWidget(scrollWidget);
					QHBoxLayout* dmLayout = new QHBoxLayout;

					if (message->GetSenderID() == m_P1->GetAccountID()) {
						DirectMessageUI* ui = new DirectMessageUI(m_P1, message, widget);
						dmLayout->addStretch();
						dmLayout->addWidget(ui);
						ui->GetMainFrame()->setStyleSheet("background-color:rgb(0,92,75);color:white");
					}
					else if (message->GetSenderID() == m_P2->GetAccountID()) {
						DirectMessageUI* ui = new DirectMessageUI(m_P2, message, widget);
						ui->GetMainFrame()->setStyleSheet("background-color:rgb(32,44,51);color:white");
						dmLayout->addWidget(ui);
						dmLayout->addStretch();
					}

					widget->setLayout(dmLayout);
					scrollLayout->addWidget(widget);
					
					m_LastImagePath = "";
					lineEdit->setText("");
				});


				div2.Layout->addWidget(imageBtn);

				div2.Layout->addWidget(sendBtn);
				div2.Layout->addStretch();

				div1.Layout->addWidget(div2.Widget);
				m_Layout->addWidget(div1.Widget);
			}
			
			m_ScrollArea = scrollArea;
		}
	protected:
		void showEvent(QShowEvent* event) {
			QWidget::showEvent(event);
			auto x = m_ScrollArea->verticalScrollBar()->maximum();
			m_ScrollArea->verticalScrollBar()->setValue(x);
		}
	private:
		QVBoxLayout* m_Layout;
		Ref<Person> m_P1,m_P2;
		QScrollArea* m_ScrollArea;
		std::string m_LastImagePath = ""; 
	};

	class MessagePanelUI : public QWidget {
		Q_OBJECT
	public:
		MessagePanelUI(QWidget* parent)
			:QWidget(parent)
		{}
		virtual ~MessagePanelUI() {}

		virtual void Show() = 0;

		virtual void Hide() = 0;
	};

	class PersonMessagesLayerUI : public QWidget {
		Q_OBJECT
	public:
		PersonMessagesLayerUI(Ref<Person> p1,QWidget* parent)
			:QWidget(parent),m_P1(p1)
		{
			m_MainLayout = new QVBoxLayout(this);

			auto btn = new QPushButton("New chat", this);
			m_MainLayout->addWidget(btn);
			m_NewDM = btn;

			QObject::connect(btn, &QPushButton::clicked, [=]() {
				PopupWindow* window = new PopupWindow(640, 480, this);
				QVBoxLayout* layout = new QVBoxLayout;

				for (auto& following : m_P1->GetFollowingID()) {

					if (m_P1->GetDirectMessages().find(following) != m_P1->GetDirectMessages().end())
						continue; 

					Ref<Person> p2 = MainLayer::Get().GetPerson(following);

					auto label = new ClickableLabel(window);
					label->setText(QString::fromStdString(p2->GetUsername()));

					QObject::connect(label, &ClickableLabel::clicked, [=]() {
						window->hide();
						window->close();
						if (m_CurrentMessageHistory)
							m_CurrentMessageHistory->hide();
						m_CurrentMessageHistory = new DirectMessageHistoryUI(m_P1, p2, {}, this);
						m_MainLayout->addWidget(m_CurrentMessageHistory);
						btn->hide();
						m_ScrollArea->hide();
						m_ScrollWidget->hide();
						m_CurrentMessageHistory->show();
						m_CurrentMessageHistory->SetLastImagePath("");
					});

					layout->addWidget(label);
				}
				layout->addStretch();
				window->setLayout(layout);
				window->show();
			});

			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			for (auto& [p2, dm] : m_P1->GetDirectMessages()) {
				auto person2 = MainLayer::Get().GetPerson(p2);

				QPushButton* btn = new QPushButton(QString::fromStdString(person2->GetUsername()), m_ScrollWidget);
				scrollLayout->addWidget(btn);
				DirectMessageHistoryUI* messageHistory = new DirectMessageHistoryUI(m_P1, person2, dm, this);

				QObject::connect(btn, &QPushButton::clicked, [=]() {
					btn->hide();
					m_ScrollArea->hide();
					m_ScrollWidget->hide();
					messageHistory->show();
					messageHistory->SetLastImagePath("");
					m_CurrentMessageHistory = messageHistory;
				});

				m_MainLayout->addWidget(messageHistory);
				messageHistory->hide();
			}

			scrollLayout->addStretch();

			m_MainLayout->addWidget(m_ScrollArea);
		}

		DirectMessageHistoryUI* GetCurrentMessageHistory() { return m_CurrentMessageHistory; }
		void SetCurrentMessageHistory(DirectMessageHistoryUI* ptr) { m_CurrentMessageHistory = ptr; }

		void HideCurrent() {
			if (!m_CurrentMessageHistory)
				return;
			m_NewDM->show();
			m_ScrollArea->show();
			m_ScrollWidget->show();
			m_CurrentMessageHistory->hide();
			m_CurrentMessageHistory->SetLastImagePath("");
			m_CurrentMessageHistory = nullptr;
		}

	private:
		QVBoxLayout* m_MainLayout;
		QScrollArea* m_ScrollArea;
		QWidget* m_ScrollWidget;

		DirectMessageHistoryUI* m_CurrentMessageHistory = nullptr;
		QPushButton* m_NewDM;
		Ref<Person> m_P1;
	};

	class PersonMessagePanelUI : public MessagePanelUI {
	public:
		PersonMessagePanelUI(Ref<Person> person, QWidget* parent)
			:MessagePanelUI(parent),m_Person(person)
		{
			SetupMessagePanel();
		}

		void Show() {
			
			show();
			m_MainFrame->show();
		}

		void Hide() {
			m_MessagesLayer->HideCurrent();
			hide();
			m_MainFrame->hide();
		}
	private:
		void SetupMessagePanel() {

			//Main frame
			{
				m_MainFrame = new QFrame(this);
				m_MainFrame->setObjectName(QString::fromUtf8("Home_MainFrame"));
				m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;"));
				m_MainFrameLayout = new QHBoxLayout;
				m_MainFrameLayout->setContentsMargins(0, 0, 0, 0);
			}

			m_MessagesLayer = new PersonMessagesLayerUI(m_Person,m_MainFrame);

			m_MainFrameLayout->addWidget(m_MessagesLayer);

			m_MainFrame->setLayout(m_MainFrameLayout);
			m_MainLayout = new QVBoxLayout(this);
			m_MainLayout->addWidget(m_MainFrame);
		}
	private:
		QVBoxLayout* m_MainLayout;
		QFrame* m_MainFrame;
		QHBoxLayout* m_MainFrameLayout;
		PersonMessagesLayerUI* m_MessagesLayer = nullptr;
		Ref<Person> m_Person;
	};

	class CompanyMessagePanelUI : public MessagePanelUI {
	public:
		CompanyMessagePanelUI(Ref<Company> company, QWidget* parent)
			:MessagePanelUI(parent), m_Company(company)
		{
			SetupMessagePanel();
		}

		void Show() {

			show();
			m_MainFrame->show();
		}

		void Hide() {
			hide();
			m_MainFrame->hide();
		}
	private:
		void SetupMessagePanel() {

			//Main frame
			{
				m_MainFrame = new QFrame(this);
				m_MainFrame->setObjectName(QString::fromUtf8("Home_MainFrame"));
				m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;"));
				m_MainFrameLayout = new QHBoxLayout;
				m_MainFrameLayout->setContentsMargins(0, 0, 0, 0);
			}


			m_MainFrameLayout->addWidget(new QLabel("Companies can't message.",m_MainFrame));

			m_MainFrame->setLayout(m_MainFrameLayout);
			m_MainLayout = new QVBoxLayout(this);
			m_MainLayout->addWidget(m_MainFrame);
		}
	private:
		QVBoxLayout* m_MainLayout;
		QFrame* m_MainFrame;
		QHBoxLayout* m_MainFrameLayout;
		Ref<Company> m_Company;
	};
}
