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
#include <QBoxLayout>

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
		m_PostsLayout->ClearPosts();

		Ref<Person> p = std::dynamic_pointer_cast<Person>(MainLayer::Get().m_CurrentUser);
		auto posts = MainLayer::Get().GetSuggestedUserPosts(p);

		for (auto& post : posts.FollowingPosts) {
			m_PostsLayout->AddPost(post);
		}
		
		for (auto& post : posts.SuggestedPosts) {
			m_PostsLayout->AddSuggestedPost(post);
		}

		for (auto& post : posts.RandomPosts) {
			m_PostsLayout->AddSuggestedPost(post);
		}
		ShowMore(10);
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

		m_CommentWindow = new PopupWindow(640, 480, m_MainFrame, PopupWindowFlagBit_Popup);
		m_CommentWindowLayout = new QVBoxLayout(m_CommentWindow);
		m_CommentWindow->setLayout(m_CommentWindowLayout);

		m_CommentsLayer = new CommentsLayer(m_CommentWindow);
		m_CommentWindowLayout->addWidget(m_CommentsLayer);

		m_MainFrameLayout->addWidget(m_PostsLayout);

		m_MainFrame->setLayout(m_MainFrameLayout);
		
		m_MainLayer->m_LayersLayout->addWidget(m_MainFrame);
	}
	void HomeLayer::ShowComments(Ref<Post> post)
	{
		m_CommentsLayer->SetPost(post);
		m_CommentWindow->show();
	}
	void PostsLayer::SetFollowingPosts(const std::vector<Ref<Post>>& posts)
	{
	}
	void PostsLayer::SetSuggestedPosts(const std::vector<Ref<Post>>& posts)
	{
	}
	void PostsLayer::SetRandomPosts(const std::vector<Ref<Post>>& posts)
	{
	}
	void CommentsLayer::AddNewCommentUI(QWidget* parent)
	{
		QVBoxLayout* mainDiv = (parent->layout() ? (QVBoxLayout*)parent->layout() : new QVBoxLayout(parent));
		QTextEdit* edit;
		{
			std::string style = "border:1px dashed black;background-color:rgb(86, 152, 144);";
			edit = new QTextEdit(parent);
			edit->setStyleSheet(QString::fromStdString(style));
			mainDiv->addWidget(edit);
			edit->setPlaceholderText("Post content");
		}
		
			
			
		{
			std::string style = "border:none;background-color:rgb(86, 152, 144);";
			HDivision* bottomDiv = new HDivision(parent);
			bottomDiv->Widget->setStyleSheet("border:none;");
			auto imageButton = new QPushButton("Image", parent);
			imageButton->setStyleSheet(QString::fromStdString(style));
			bottomDiv->Layout->addWidget(imageButton);
			bottomDiv->Layout->addStretch();
			auto postButton = new QPushButton("Post", parent);
			postButton->setStyleSheet(QString::fromStdString(style));
			bottomDiv->Layout->addWidget(postButton);
			mainDiv->addWidget(bottomDiv->Widget);

			QObject::connect(postButton, &QPushButton::clicked, [=]() {
				MainLayer::Get().CommentOnPost(m_Post, edit->document()->toPlainText().toStdString(),"","");
			});
		}
			

		mainDiv->setContentsMargins(10, 5, 10, 5);
		parent->setLayout(mainDiv);
	}
}