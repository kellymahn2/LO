#include "ErrorLayer.h"


#include "MainLayer.h"
#include "Core/Window.h"


#include <QFrame>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QIcon>
#include <QVBoxLayout>
#include <QIntValidator>



namespace LinkedOut {

	const float MessageBoxWidth = 200;
	const float MessageBoxHeight = 100;
	const float MessageBoxPadding = 5;


	MessageLayer::MessageLayer(MainLayer* mainLayer)
		:Layer("MessageLayer"),m_MainLayer(mainLayer)
	{
		QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);


		m_MessagesFrame = new QFrame(m_MainLayer->m_WindowCentralWidget);
		m_MessagesFrame->setObjectName(QString::fromUtf8("m_MessagesFrame"));
		m_MessagesFrame->setGeometry(QRect(850, 0, 300, 720));

		m_MessagesFrame->setSizePolicy(sizePolicy);
		m_MessagesFrame->setSizeIncrement(QSize(0, 0));
		m_MessagesFrame->setStyleSheet(QString::fromUtf8("background-color:rgba(51,52,54,0);"));
		//m_MessagesFrame->setStyleSheet(QString::fromUtf8("background-color:white;"));
		m_MessagesFrame->setFrameShape(QFrame::StyledPanel);
		m_MessagesFrame->setFrameShadow(QFrame::Raised);
		m_VerticalLayoutWidget = new QWidget(m_MessagesFrame);
		m_VerticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
		m_VerticalLayoutWidget->setGeometry(QRect(0, 0, 300, 720));
		m_MessagesVerticalLayout = new QVBoxLayout(m_VerticalLayoutWidget);
		m_MessagesVerticalLayout->setSpacing(6);
		m_MessagesVerticalLayout->setObjectName(QString::fromUtf8("m_MessagesVerticalLayout"));
		m_MessagesVerticalLayout->setAlignment(Qt::AlignBottom | Qt::AlignRight);

		m_MessageCloseIcon = new QIcon();
		m_MessageCloseIcon->addFile(QString::fromUtf8(":/LinkedOut/Resources/MessageCloseButtonOnHover.png"), QSize(), QIcon::Normal, QIcon::Off);

		//Per message



		/*m_MessageFrame = new QFrame(m_VerticalLayoutWidget);
		m_MessageFrame->setObjectName(QString::fromUtf8("m_MessageFrame"));
		m_MessageFrame->setFrameShape(QFrame::StyledPanel);
		m_MessageFrame->setFrameShadow(QFrame::Raised);
		m_MessageLabel = new QLabel(m_MessageFrame);
		m_MessageLabel->setObjectName(QString::fromUtf8("m_MessageLabel"));
		m_MessageLabel->setGeometry(QRect(0, 0, 181, 81));
		m_MessageLabel->setTextFormat(Qt::AutoText);
		m_MessageLabel->setScaledContents(false);
		m_MessageLabel->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);
		m_MessageLabel->setWordWrap(true);
		m_MessagePushButton = new QPushButton(m_MessageFrame);
		m_MessagePushButton->setObjectName(QString::fromUtf8("m_MessagePushButton"));
		m_MessagePushButton->setGeometry(QRect(185, 0, 24, 24));
		m_MessagePushButton->setStyleSheet(QString::fromUtf8("\n"
			"\n"
			"QPushButton {\n"
			"	border:none;\n"
			"}\n"
			"\n"
			"\n"
			"QPushButton:hover{\n"
			"		background-color:rgb(240,20,30);\n"
			"}"));
		
		m_MessagePushButton->setIcon(icon);
		m_MessagePushButton->setFlat(true);

		m_MessagesVerticalLayout->addWidget(m_MessageFrame);

		m_MessageLabel->setText(QApplication::translate("LinkedOutClass", "sdasdasdasdasdasdasdsadaddddddddddddddddddddddddd", nullptr));
		m_MessagePushButton->setText(QString());*/

	}

	MessageLayer::~MessageLayer()
	{
	}


	void MessageLayer::Show()
	{
		m_MessagesFrame->show();
		m_IsShowing = true;
	}

	void MessageLayer::Hide()
	{
		m_IsShowing = false;
		m_MessagesFrame->hide();
	}

	void MessageLayer::OnUpdate()
	{

		m_Last = m_Now;
		m_Now = std::chrono::high_resolution_clock::now();



		auto window = Window::GetMainWindow()->GetNativeHandle();
		auto size = window->size();
		m_MessagesFrame->setGeometry(size.width() - MessageBoxWidth, 0, MessageBoxWidth, size.height());
		


		for (auto& [time,message] : m_Messages) {
			if(message.IsVisible){
				message.MessageFrame->hide();
				message.IsVisible = false;
			}
		}

		auto mbBottomLeftCornerY = size.height();

		for (auto& [time,message]: m_Messages) {
			if (mbBottomLeftCornerY < 0)
				break;
			message.MessageFrame->setGeometry(0, mbBottomLeftCornerY - MessageBoxHeight, MessageBoxWidth, MessageBoxHeight);
			if (!message.IsVisible) {
				message.MessageFrame->show();
				message.IsVisible = true;
			}
			mbBottomLeftCornerY -=MessageBoxHeight;
			mbBottomLeftCornerY -= MessageBoxPadding;
		}


		UpdateTimers();
		RemoveExpiredMessages();
	}
	void MessageLayer::ShowMessage(const std::string& message, MessageSeverity severity)
	{
		Message msg;
		msg.MessageString = message;
		msg.Severity = severity;
		auto suppliedTime = std::chrono::steady_clock::now();
		msg.MessageAliveDurationInMS = 0.0f; // Default duration for messages
		msg.MessageDurationInMS = 5.0f;

		msg.MessageFrame = new QFrame(m_MessagesFrame);
		msg.MessageFrame->setObjectName(QString::fromUtf8("m_MessageFrame"));
		msg.MessageFrame->setFixedWidth(MessageBoxWidth);
		msg.MessageFrame->setFixedHeight(MessageBoxHeight);
		msg.MessageFrame->setStyleSheet("boder:5px solid black;background-color:red;");

		msg.MessageLabel = new QLabel(msg.MessageFrame);
		msg.MessageLabel->setObjectName(QString::fromUtf8("m_MessageLabel"));
		msg.MessageLabel->setGeometry(QRect(0, 0, MessageBoxWidth-30, MessageBoxHeight));
		msg.MessageLabel->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);
		msg.MessageLabel->setWordWrap(true);
		
		msg.MessageLabel->setText(QApplication::translate("LinkedOutClass", message.c_str(), nullptr));

		msg.MessagePushButton = new QPushButton("", msg.MessageFrame);
		msg.MessagePushButton->setObjectName(QString::fromUtf8("m_MessagePushButton"));
		msg.MessagePushButton->setGeometry(QRect(MessageBoxWidth-24, 0, 24, 24));
		msg.MessagePushButton->setStyleSheet(QString::fromUtf8("\n"
			"\n"
			"QPushButton {\n"
			"	border:none;\n"
			"}\n"
			"\n"
			"\n"
			"QPushButton:hover{\n"
			"		background-color:rgb(240,20,30);\n"
			"}"));

		msg.MessagePushButton->setIcon(*m_MessageCloseIcon);
		msg.MessagePushButton->setFlat(true);
		msg.MessageFrame->hide();


		QObject::connect(msg.MessagePushButton, &QPushButton::clicked, [&,suppliedTime]() {
			m_Messages.erase(suppliedTime);
			});

		// Customize appearance based on message severity
		switch (severity) {
		case MessageSeverity::Info:
			// Set style or color for info messages
			break;
		case MessageSeverity::Warn:
			// Set style or color for warning messages
			break;
		case MessageSeverity::Error:
			// Set style or color for error messages
			break;
		default:
			break;
		}

		m_Messages[suppliedTime] = msg;
	}
	void MessageLayer::RemoveExpiredMessages()
	{
		std::vector<std::chrono::steady_clock::time_point> timesToRemove;
		for (auto& [time,msg] : m_Messages) {
			if (msg.MessageAliveDurationInMS > msg.MessageDurationInMS) {
				timesToRemove.push_back(time);
			}
		}

		for (auto& time : timesToRemove) {
			auto msg = m_Messages[time];
			m_Messages.erase(time);
			DestroyMessage(msg);
		}

	}

	void MessageLayer::UpdateTimers()
	{
		auto ts = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(m_Now - m_Last).count()*1e-9f;
		for (auto& [time,msg] : m_Messages) {
			msg.MessageAliveDurationInMS += ts;
		}
		
	}
	
	void MessageLayer::DestroyMessage(MessageLayer::Message& msg)
	{
		msg.MessageFrame->hide();
		msg.MessageFrame->setParent(nullptr);
		delete msg.MessagePushButton;
		delete msg.MessageLabel;
		delete msg.MessageFrame;
	}
}


