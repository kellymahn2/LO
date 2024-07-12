#pragma once

#include "Core/Post.h"
#include "Core/Person.h"

#include "Layers/PostUI.h"
#include "Layers/CommentUI.h"
#include "Layers/MainLayer.h"

#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QMessageBox>
#include <QTextEdit>
#include <QStyle>

#include <QTextEdit>

#include <QWidget>

namespace LinkedOut {

	class CommentsLayer : public QWidget {
		Q_OBJECT
	public:
		explicit CommentsLayer(QWidget* parent = nullptr)
			:QWidget(parent)
		{
			m_MainLayout = new QVBoxLayout(this);
			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			

			m_MainLayout->addWidget(m_ScrollArea);
			setLayout(m_MainLayout);
		}

		void SetPost(Ref<Post> post)
		{
			ClearCommentUIs();
			m_Post = post;
			CreateCommentUIs();
		}

	private:
		void ClearCommentUIs() {
			// Remove all child widgets from the scroll widget
			QLayoutItem* child;
			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
			if (scrollLayout) {
				while ((child = scrollLayout->takeAt(0)) != nullptr) {
					delete child->widget();
					delete child;
				}
			}
		}

		void CreateCommentUIs() {
			
			
			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());

				QWidget* w = new QWidget(m_ScrollWidget);
				w->setLayout(new QVBoxLayout(w));
				w->setStyleSheet("background-color:rgb(86, 152, 144);");
				m_Post->Serialize(w);
				if (scrollLayout) {
					scrollLayout->addWidget(w);
				}
			}



			AddNewCommentUI(m_ScrollWidget);

			m_CommentsDiv = VDivision(m_ScrollWidget);
			qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout())->addWidget(m_CommentsDiv.Widget);
			m_CommentsDiv.Layout->setDirection(QBoxLayout::BottomToTop);
			
			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_CommentsDiv.Layout);

			for (auto& comment : m_Post->GetComments()) {
				CommentUI* commentUI = new CommentUI(comment, m_CommentsDiv.Widget);

				if (scrollLayout) {
					scrollLayout->addWidget(commentUI);
				}
			}

			if (scrollLayout)
				scrollLayout->addStretch();
		}

		void AddNewCommentUI(QWidget* parent)
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
				auto postButton = new QPushButton("Post", parent);
				postButton->setStyleSheet(QString::fromStdString(style));
				bottomDiv->Layout->addWidget(postButton);
				mainDiv->addWidget(bottomDiv->Widget);

				QObject::connect(postButton, &QPushButton::clicked, [=]() {
					MainLayer::Get().CommentOnPost(m_Post, edit->document()->toPlainText().toStdString(), "", "");
					edit->setText("");
					QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_CommentsDiv.Layout);
					CommentUI* commentUI = new CommentUI(m_Post->GetComments().back(), m_CommentsDiv.Widget);

					m_CommentsDiv.Layout->addWidget(commentUI);
				});
			}

			mainDiv->setContentsMargins(10, 5, 10, 5);
			parent->setLayout(mainDiv);
		}

	private:
		VDivision m_CommentsDiv;
		QVBoxLayout* m_MainLayout;
		QScrollArea* m_ScrollArea;
		ClickableLabel* m_LoadMoreLabel;
		QWidget* m_ScrollWidget;
		Ref<Post> m_Post;
	};

	class PostsLayer : public QWidget {
		Q_OBJECT
	public:
		explicit PostsLayer(std::function<void(Ref<Post>)>&& commentsCallback, bool canFollow,QWidget* parent = nullptr)
			: QWidget(parent), m_CommentsCallback(commentsCallback),m_CanFollow(canFollow)
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
			m_LoadMoreLabel->setText("Load more posts");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
			});

			m_StartPostButton = new QPushButton(m_ScrollWidget);
			m_StartPostButton->setText("Start a post");
			m_StartPostButton->setStyleSheet("color:white;font-weight:bold;font-size:15px;background-color:rgb(86, 152, 144);border:2px solid white;border-radius:8px");
			{
				m_StartPostPopup = new PopupWindow(640, 480, this, PopupWindowFlagBit_Popup);

				std::string popupStyleSheet = "border:3px solid green;background-color:rgb(86, 152, 144);";

				m_StartPostPopup->setStyleSheet(QString::fromStdString(popupStyleSheet));
				SetupPopup(m_StartPostPopup);
				QObject::connect(m_StartPostButton, &QPushButton::clicked, [this]() {
					m_StartPostPopup->show();
				});
			}
			scrollLayout->addWidget(m_StartPostButton);
			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();

			m_MainLayout->addWidget(m_ScrollArea);
		}

		void AddPost(Ref<Post> post) {
			m_Posts.push_back(post);
		}

		void AddSuggestedPost(Ref<Post> post) {
			m_SuggestedPosts.push_back(post);
		}

		void ShowMore(uint32_t amount) {
			uint32_t shouldShow = std::min(m_CurrentShown + amount, (uint32_t)m_Posts.size() + (uint32_t)m_SuggestedPosts.size());

			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
				QLayoutItem* item = scrollLayout->itemAt(scrollLayout->count() - 1);
				scrollLayout->removeItem(item);

				item = scrollLayout->itemAt(scrollLayout->count() - 1);
				scrollLayout->removeItem(item);
			}

			uint32_t currentShown = m_CurrentShown;

			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());

			//Show normal posts
			for (; currentShown < std::min(shouldShow, (uint32_t)m_Posts.size()); ++currentShown) {
				PostUI* post = new PostUI(m_Posts[currentShown], false, std::forward<decltype(m_CommentsCallback)>(m_CommentsCallback), m_ScrollWidget);

				if (scrollLayout) {
					scrollLayout->addWidget(post);
				}
			}

			//Show suggested posts
			for (uint32_t i = currentShown; i < shouldShow; ++i) {
				if (i == currentShown) {
					QLabel* label = new QLabel("Suggested posts", m_ScrollWidget);
					if (scrollLayout) {
						scrollLayout->addWidget(label);
					}
				}
				PostUI* post = new PostUI(m_SuggestedPosts[i - m_Posts.size()], true && m_CanFollow, std::forward<decltype(m_CommentsCallback)>(m_CommentsCallback), m_ScrollWidget);

				if (scrollLayout) {
					scrollLayout->addWidget(post);
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
			m_Posts.clear();
			m_SuggestedPosts.clear();

			delete m_ScrollWidget;
			delete m_ScrollArea;


			// Remove all child widgets from the scroll widget
			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			m_LoadMoreLabel = new ClickableLabel(m_ScrollWidget);
			m_LoadMoreLabel->setText("Load more posts");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
				});

			m_StartPostButton = new QPushButton(m_ScrollWidget);
			m_StartPostButton->setText("Start a post");
			m_StartPostButton->setStyleSheet("color:white;font-weight:bold;font-size:15px;background-color:rgb(86, 152, 144);border:2px solid white;border-radius:8px");
			{
				m_StartPostPopup = new PopupWindow(640, 480, this, PopupWindowFlagBit_Popup);

				std::string popupStyleSheet = "border:3px solid green;background-color:rgb(86, 152, 144);";

				m_StartPostPopup->setStyleSheet(QString::fromStdString(popupStyleSheet));
				SetupPopup(m_StartPostPopup);
				QObject::connect(m_StartPostButton, &QPushButton::clicked, [this]() {
					m_StartPostPopup->show();
					});
			}
			scrollLayout->addWidget(m_StartPostButton);
			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();

			m_MainLayout->addWidget(m_ScrollArea);

			m_CurrentShown = 0;
		}

		QVBoxLayout* GetScrollLayout() {
			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
			return scrollLayout;
		}

	private:
		void SetupPopup(QWidget* parent)
		{
			QVBoxLayout* mainDiv = new QVBoxLayout(parent);
			QTextEdit* edit;
			{
				std::string style = "border:none;background-color:rgb(86, 152, 144);";
				HDivision* headerDiv = new HDivision(parent);
				headerDiv->Widget->setStyleSheet("border:none;");
				headerDiv->Layout->addStretch();
				auto button = new QPushButton(QIcon("Resources/MessageCloseButton.png"), "", parent);
				button->setStyleSheet(QString::fromStdString(style));
				//button->setFlat(true);
				headerDiv->Layout->addWidget(button);
				mainDiv->addWidget(headerDiv->Widget);
			}
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


				QObject::connect(imageButton, &QPushButton::clicked, [this]() {
					m_LastImagePath = "";
					auto path = OpenFileDialog("Images (*.png *.jpg *.jpeg)");
					if (path.has_value())
						m_LastImagePath = path.value();
					});

				QObject::connect(postButton, &QPushButton::clicked, [parent,edit,this]() {
					MainLayer::Get().MakePost(edit->document()->toPlainText().toStdString(), m_LastImagePath,"");
					parent->hide();
				});
			}
			mainDiv->setContentsMargins(10, 5, 10, 5);
			parent->setLayout(mainDiv);
		}
	private:

		QVBoxLayout* m_MainLayout;
		QScrollArea* m_ScrollArea;
		QWidget* m_ScrollWidget;
		ClickableLabel* m_LoadMoreLabel;

		std::vector<Ref<Post>> m_Posts;
		std::vector<Ref<Post>> m_SuggestedPosts;
		uint32_t m_CurrentShown = 0;
		std::function<void(Ref<Post>)> m_CommentsCallback;

		QPushButton* m_StartPostButton;
		PopupWindow* m_StartPostPopup;
		bool m_CanFollow;
		std::string m_LastImagePath;

	};

	class HomeLayerUI : public QWidget {
		Q_OBJECT
	public:

		HomeLayerUI(QWidget* parent)
			:QWidget(parent)
		{}
		virtual ~HomeLayerUI() {}

		virtual void Show()  = 0;

		virtual void Hide()  = 0;

		virtual void* GetMainFrame() const  = 0;

		virtual void OnUpdate()  = 0;

		virtual void AddPost(Ref<Post> post) = 0;
		virtual void ClearPosts() = 0;
		virtual void ShowMore(uint32_t amount) = 0;
	};

	class PersonHomeLayerUI : public HomeLayerUI {
	public:
		PersonHomeLayerUI(Ref<Person> p,QWidget* parent)
			:m_Person(p),HomeLayerUI(parent)
		{
			SetupHome();
			setLayout(m_MainLayout);
		}
		~PersonHomeLayerUI() {

		}

		void Show() override {
			show();
			m_MainFrame->show();
			m_PostsLayout->ClearPosts();

			auto posts = MainLayer::Get().GetSuggestedUserPosts(m_Person);

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

		void Hide() override {
			hide();
			m_MainFrame->hide();
		}

		void* GetMainFrame() const override { return nullptr; }

		void OnUpdate() override {}

		void AddPost(Ref<Post> post) {
			m_PostsLayout->AddPost(post);
		}
		void ClearPosts() {
			m_PostsLayout->ClearPosts();
		}

		void ShowMore(uint32_t amount) {
			m_PostsLayout->ShowMore(amount);
		}

	private:
		void SetupHome() {
			QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

			//Main frame
			{
				m_MainFrame = new QFrame(this);
				m_MainFrame->setObjectName(QString::fromUtf8("Home_MainFrame"));
				m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;"));
				m_MainFrameLayout = new QHBoxLayout;
				m_MainFrameLayout->setContentsMargins(0, 0, 0, 0);
			}

			m_PostsLayout = new PostsLayer([this](Ref<Post> post) {
				m_CommentsLayer->SetPost(post);
				m_CommentWindow->show();
				},true, m_MainFrame);

			m_CommentWindow = new PopupWindow(640, 480, m_MainFrame, PopupWindowFlagBit_Popup);
			m_CommentWindowLayout = new QVBoxLayout(m_CommentWindow);
			m_CommentWindow->setLayout(m_CommentWindowLayout);

			m_CommentsLayer = new CommentsLayer(m_CommentWindow);
			m_CommentWindowLayout->addWidget(m_CommentsLayer);

			m_MainFrameLayout->addWidget(m_PostsLayout);

			m_MainFrame->setLayout(m_MainFrameLayout);
			m_MainLayout = new QVBoxLayout(this);
			m_MainLayout->addWidget(m_MainFrame);
		}
	private:
		QVBoxLayout* m_MainLayout;
		QFrame* m_MainFrame;
		QHBoxLayout* m_MainFrameLayout;
		PostsLayer* m_PostsLayout;
		PopupWindow* m_CommentWindow;
		QVBoxLayout* m_CommentWindowLayout;
		CommentsLayer* m_CommentsLayer;
		Ref<Person> m_Person;
	};

	class CompanyHomeLayerUI : public HomeLayerUI {
	public:
		CompanyHomeLayerUI(Ref<Company> c, QWidget* parent)
			:m_Company(c), HomeLayerUI(parent)
		{
			SetupHome();
			setLayout(m_MainLayout);
		}
		~CompanyHomeLayerUI() {

		}

		void Show() override {
			show();
			m_MainFrame->show();

			auto posts = MainLayer::Get().GetSuggestedCompanyPostsFromFollowed(m_Company);

			for (auto& post : posts) {
				m_PostsLayout->AddPost(post);
			}
			
			ShowMore(10);
		}

		void Hide() override {
			hide();
			m_MainFrame->hide();
			m_PostsLayout->ClearPosts();
		}

		void* GetMainFrame() const override { return nullptr; }

		void OnUpdate() override {}

		void AddPost(Ref<Post> post) {
			m_PostsLayout->AddPost(post);
		}
		void ClearPosts() {
			m_PostsLayout->ClearPosts();
		}

		void ShowMore(uint32_t amount) {
			m_PostsLayout->ShowMore(amount);
		}

	private:
		void SetupHome() {
			QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

			//Main frame
			{
				m_MainFrame = new QFrame(this);
				m_MainFrame->setObjectName(QString::fromUtf8("Home_MainFrame"));
				m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;"));
				m_MainFrameLayout = new QHBoxLayout;
				m_MainFrameLayout->setContentsMargins(0, 0, 0, 0);
			}

			m_PostsLayout = new PostsLayer([this](Ref<Post> post) {
				m_CommentsLayer->SetPost(post);
				m_CommentWindow->show();
				},false, m_MainFrame);

			m_CommentWindow = new PopupWindow(640, 480, m_MainFrame, PopupWindowFlagBit_Popup);
			m_CommentWindowLayout = new QVBoxLayout(m_CommentWindow);
			m_CommentWindow->setLayout(m_CommentWindowLayout);

			m_CommentsLayer = new CommentsLayer(m_CommentWindow);
			m_CommentWindowLayout->addWidget(m_CommentsLayer);

			m_MainFrameLayout->addWidget(m_PostsLayout);

			m_MainFrame->setLayout(m_MainFrameLayout);
			m_MainLayout = new QVBoxLayout(this);
			m_MainLayout->addWidget(m_MainFrame);
		}
	private:
		QVBoxLayout* m_MainLayout;
		QFrame* m_MainFrame;
		QHBoxLayout* m_MainFrameLayout;
		PostsLayer* m_PostsLayout;
		PopupWindow* m_CommentWindow;
		QVBoxLayout* m_CommentWindowLayout;
		CommentsLayer* m_CommentsLayer;
		Ref<Company> m_Company;
	};

}
