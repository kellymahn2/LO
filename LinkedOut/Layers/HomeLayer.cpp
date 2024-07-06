#include "HomeLayer.h"
#include "Core/Window.h"
#include "MainLayer.h"
#include "CustomUI/Division.h"

#include <QFrame>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QMainWindow>

namespace LinkedOut {
	HomeLayer::HomeLayer(MainLayer* mainLayer)
		:Layer("HomeLayer"),m_MainLayer(mainLayer)
	{
		SetupHome();
	}
	HomeLayer::~HomeLayer()
	{
	}
	void HomeLayer::Show()
	{
		m_MainFrame->show();
		MainLayer::Get().GetUserPosts();
	}
	void HomeLayer::Hide()
	{
		m_MainFrame->hide();
	}
	void* HomeLayer::GetMainFrame() const
	{
		return nullptr;
	}
	void HomeLayer::OnUpdate()
	{
	}
	void HomeLayer::SetupHome()
	{
		auto window = Window::GetMainWindow()->GetNativeHandle();

		QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

		//Main frame
		{
			m_MainFrame = new QFrame(m_MainLayer->m_WindowCentralWidget);
			m_MainFrame->setObjectName(QString::fromUtf8("Home_MainFrame"));
			m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;"));
			m_MainFrameLayout = new QHBoxLayout;
			m_MainFrameLayout->setContentsMargins(0, 0, 0, 0);
		}

		m_PostsLayout = new PostsLayer([this](Ref<Post> post) {
			ShowComments(post);
		}, m_MainFrame);

		m_CommentsLayer = new CommentsLayer(m_MainFrame);

		m_MainFrameLayout->addWidget(m_PostsLayout);
		m_MainFrameLayout->addWidget(m_CommentsLayer);
		m_CommentsLayer->hide();

		m_MainFrame->setLayout(m_MainFrameLayout);
		
		m_MainLayer->m_LayersLayout->addWidget(m_MainFrame);
	}
	void HomeLayer::ShowComments(Ref<Post> post)
	{
		m_PostsLayout->hide();
		m_CommentsLayer->SetPost(post);
		m_CommentsLayer->show();
	}
}