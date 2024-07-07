#pragma once


#include "Core/Layer.h"
#include "CustomUI/Division.h"
#include "CustomUI/Separator.h"
#include "CustomUI/ClickableLabel.h"
#include "CustomUI/TitledButton.h"
#include "CustomUI/PopupWindow.h"




#include "Core/Post.h"
#include "PostUI.h"
#include "CommentUI.h"

#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QMessageBox>
#include <QTextEdit>
#include <QStyle>

#include <QTextEdit>




class QScrollArea;

namespace LinkedOut {

	class MainLayer;

	


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
			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
			{
				QWidget* w = new QWidget(m_ScrollWidget);
				w->setLayout(new QVBoxLayout(w));
				w->setStyleSheet("background-color:rgb(86, 152, 144);");
				m_Post->Serialize(w);
				if (scrollLayout) {
					scrollLayout->addWidget(w);
				}
			}
			AddNewCommentUI(m_ScrollWidget);
			for (auto& comment : m_Post->GetComments()) {
				CommentUI* commentUI = new CommentUI(comment, m_ScrollWidget);

				if (scrollLayout) {
					scrollLayout->addWidget(commentUI);
				}
			}

			

			if(scrollLayout)
				scrollLayout->addStretch();
		}

		void AddNewCommentUI(QWidget* parent);

	private:
		QVBoxLayout* m_MainLayout;
		QScrollArea* m_ScrollArea;
		ClickableLabel* m_LoadMoreLabel;
		QWidget* m_ScrollWidget;
		Ref<Post> m_Post;

	};


	class PostsLayer : public QWidget {
		Q_OBJECT
	public:
		explicit PostsLayer(std::function<void(Ref<Post>)>&& commentsCallback, QWidget* parent = nullptr)
			: QWidget(parent),m_CommentsCallback(commentsCallback) {
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
				m_StartPostPopup = new PopupWindow(640, 480, this,PopupWindowFlagBit_Popup);

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

		void SetFollowingPosts(const std::vector<Ref<Post>>& posts);

		void SetSuggestedPosts(const std::vector<Ref<Post>>& posts);

		void SetRandomPosts(const std::vector<Ref<Post>>& posts);

		void AddPost(Ref<Post> post) {
		   m_Posts.push_back(post);
			
		   /* Post* post = new Post(spec, m_ScrollWidget);
		   QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
		   QLayoutItem* item = scrollLayout->itemAt(scrollLayout->count() - 1);
		   scrollLayout->removeItem(item);
		   if (scrollLayout) {
			   scrollLayout->addWidget(post);
		   }
		   scrollLayout->addStretch();
		   m_Posts.push_back(post);
		   if (m_CurrentShown == m_ShouldShow) {
			   post->hide();
		   }
		   else {
			   m_CurrentShown++;
		   }*/
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
			for (; currentShown < std::min(shouldShow,(uint32_t)m_Posts.size()); ++currentShown) {
				PostUI* post = new PostUI(m_Posts[currentShown],false, std::forward<decltype(m_CommentsCallback)>(m_CommentsCallback), m_ScrollWidget);
				
				if (scrollLayout) {
					scrollLayout->addWidget(post);
				}
			}

			//Show suggested posts
			for (uint32_t i = currentShown; i < shouldShow;++i) {
				if (i == currentShown) {
					QLabel* label = new QLabel("Suggested posts", m_ScrollWidget);
					if (scrollLayout) {
						scrollLayout->addWidget(label);
					}
				}
				PostUI* post = new PostUI(m_SuggestedPosts[i - m_Posts.size()],true, std::forward<decltype(m_CommentsCallback)>(m_CommentsCallback), m_ScrollWidget);

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
			// Remove all child widgets from the scroll widget
			QLayoutItem* child;
			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
			if (scrollLayout) {
				while ((child = scrollLayout->takeAt(0)) != nullptr) {
					delete child->widget();
					delete child;
				}
			}

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

				QTextEdit* edit = new QTextEdit(parent);
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
			}
			mainDiv->setContentsMargins(10, 5, 10, 5);
			parent->setLayout(mainDiv);
		}
	private:

		QVBoxLayout* m_MainLayout;
		QScrollArea* m_ScrollArea;
		ClickableLabel* m_LoadMoreLabel;
		QWidget* m_ScrollWidget;
		
		std::vector<Ref<Post>> m_Posts;
		std::vector<Ref<Post>> m_SuggestedPosts;
		uint32_t m_CurrentShown = 0;
		std::function<void(Ref<Post>)> m_CommentsCallback;

		QPushButton* m_StartPostButton;
		PopupWindow* m_StartPostPopup;
	};

	class HomeLayer : public Layer {
	public:
		HomeLayer(MainLayer* mainLayer);
		~HomeLayer();

		void Show() override;

		void Hide() override;

		void* GetMainFrame() const override;

		void OnUpdate() override;

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
		void SetupHome();
		void ShowComments(Ref<Post> post);
	private:
		MainLayer* m_MainLayer;
		QFrame* m_MainFrame;
		QHBoxLayout* m_MainFrameLayout;
		PostsLayer* m_PostsLayout;
		PopupWindow* m_CommentWindow;
		QVBoxLayout* m_CommentWindowLayout;
		CommentsLayer* m_CommentsLayer;
	};
}