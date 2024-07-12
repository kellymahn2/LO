#include "PostUI.h"

#include "MainLayer.h"

#include <QApplication>
#include <QClipboard>

namespace LinkedOut {
	PostUI::PostUI(Ref<Post> post, bool needsFollowing, std::function<void(Ref<Post>)>&& commentsCallback, QWidget* parent)
		: QFrame(parent),
		m_HeaderDiv(this),
		m_UsernameDiv(this),
		m_UsernameLabel(new QLabel("Username", this)),
		m_ContentDiv(this),
		m_ContentLabel(new ClickableLabel(this)),
		m_WhoLikedThis(new ClickableLabel(this)),
		m_TimestampLabel(new QLabel("Timestamp", this)),
		m_TopSeparator(new Separator(this)),
		m_BottomSeparator(new Separator(this)),
		m_BottomDiv(this),
		m_LikeButton(new TitledButton(QIcon("Resources/LikeIcon.png"), m_ButtonSize, m_ButtonSize, 0, "Like", this)),
		m_CommentButton(new TitledButton(QIcon("Resources/CommentIcon.png"), m_ButtonSize, m_ButtonSize, 0, "Comment", this)),
		m_RepostButton(new TitledButton(QIcon("Resources/RepostIcon.png"), m_ButtonSize, m_ButtonSize, 0, "Repost", this)),
		m_SendButton(new TitledButton(QIcon("Resources/SendIcon.png"), m_ButtonSize, m_ButtonSize, 0, "Send", this)),
		m_MainLayout(new QVBoxLayout),m_Post(post)
	{

		//Header
		m_UsernameDiv.Layout->addWidget(m_UsernameLabel);
		m_UsernameDiv.Layout->addWidget(m_TimestampLabel);
		m_UsernameDiv.Layout->setContentsMargins(0, 0, 0, 0);

		m_HeaderDiv.Layout->addWidget(m_UsernameDiv.Widget);

		if (needsFollowing) {
			m_HeaderDiv.Layout->addStretch();
			m_FollowButton = new ClickableLabel(this);
			m_FollowButton->setText("Follow");
			m_FollowButton->setStyleSheet("color:aqua;");
			m_HeaderDiv.Layout->addWidget(m_FollowButton);
			QObject::connect(m_FollowButton, &ClickableLabel::clicked, [this,post]() {
				if (auto id = MainLayer::Get().CompanyAccountExists(post->GetPosterName());id != "") {
					MainLayer::Get().Follow(MainLayer::Get().GetCurrentUser()->GetAccountID(), id);
					m_FollowButton->deleteLater();
				}
				else if(auto id = MainLayer::Get().PersonAccountExists(post->GetPosterName());id != "") {
					MainLayer::Get().RequestFollow(id);
				}
			});
		}
		m_HeaderDiv.Layout->setContentsMargins(0, 0, 0, 0);
		m_MainLayout->addWidget(m_HeaderDiv.Widget);
		m_TopSeparator->setStyleSheet("background-color:rgb(100, 165, 155);");
		m_MainLayout->addWidget(m_TopSeparator);

		QString postText = ShortenText(QString::fromStdString(post->GetContentText()), 200, font());

		m_ContentLabel->setText(postText);

		if (!m_Post->GetContentPicture().empty()) {
			QPixmap image(m_Post->GetContentPicture().c_str());
			if (!image.isNull()) {
				QLabel* imageLabel = new QLabel(m_ContentDiv.Widget);
				imageLabel->setPixmap(image.scaled(QSize(100, 100), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
				m_ContentDiv.Layout->addWidget(imageLabel);
			}
		}

		if (postText == QString::fromStdString(post->GetContentText())) {
			m_ContentLabel->setCursor(Qt::ArrowCursor);
		}
		else {
			QObject::connect(m_ContentLabel, &ClickableLabel::clicked, [this, postText, post]() {
				if (m_ContentLabel->text() == postText)
					m_ContentLabel->setText(QString::fromStdString(post->GetContentText()));
				else
					m_ContentLabel->setText(postText);
			});
		}

		m_ContentDiv.Layout->addWidget(m_ContentLabel);

		{
			
			m_WhoLikedThisWindow = new PopupWindow(640, 480, m_ContentDiv.Widget, PopupWindowFlagBit_Popup);
			auto layout = new QVBoxLayout(m_WhoLikedThisWindow);
			m_WhoLikedThisWindow->setLayout(layout);

			for (uint32_t i = 0; i < std::min(3ULL, post->GetLikes().size()); ++i) {
				auto& like = post->GetLikes()[i];
				Ref<Person> p = MainLayer::Get().GetPerson(like.GetLikedBy());
				auto& t = like.GetLikedAt();
				layout->addWidget(new QLabel(QString::fromStdString(fmt::format("Liked by {} {}/{}/{} {}:{}", p->GetUsername(), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute())), m_WhoLikedThisWindow));
			}
			layout->addStretch();

			m_WhoLikedThis->setText("Who liked this");
			QObject::connect(m_WhoLikedThis, &ClickableLabel::clicked, [this]() {
				m_WhoLikedThisWindow->show();
			});

			m_ContentDiv.Layout->addWidget(m_WhoLikedThis);
		}


		m_MainLayout->addWidget(m_ContentDiv.Widget);

		m_MainLayout->addWidget(m_BottomSeparator);
		m_BottomSeparator->setStyleSheet("background-color:rgb(100, 165, 155);");

		m_BottomDiv.Layout->addWidget(m_LikeButton);
		m_LikeButton->setStyleSheet("QPushButton{background-color:rgba(0,0,0,0);}QLabel{color:black;font-weight:bold;font-style:italic;}");
		m_BottomDiv.Layout->addStretch();
		m_BottomDiv.Layout->addWidget(m_CommentButton);
		m_CommentButton->setStyleSheet("QPushButton{background-color:rgba(0,0,0,0);}QLabel{color:black;font-weight:bold;font-style:italic;}");
		m_BottomDiv.Layout->addStretch();
		m_BottomDiv.Layout->addWidget(m_RepostButton);
		m_RepostButton->setStyleSheet("QPushButton{background-color:rgba(0,0,0,0);}QLabel{color:black;font-weight:bold;font-style:italic;}");
		m_BottomDiv.Layout->addStretch();
		m_BottomDiv.Layout->addWidget(m_SendButton);
		m_SendButton->setStyleSheet("QPushButton{background-color:rgba(0,0,0,0);}QLabel{color:black;font-weight:bold;font-style:italic;}");
		m_BottomDiv.Layout->addStretch();

		QObject::connect(m_CommentButton, &TitledButton::clicked, [commentsCallback, post]() {commentsCallback(post); });

		QObject::connect(m_LikeButton, &TitledButton::clicked, [post, this]() {
			QLayout* layout = m_WhoLikedThisWindow->layout();
			MainLayer::Get().LikePost(post);
			auto& like = post->GetLikes().back();
			Ref<Person> p = MainLayer::Get().GetPerson(like.GetLikedBy());
			auto& t = like.GetLikedAt();
			layout->addWidget(new QLabel(QString::fromStdString(fmt::format("Liked by {} {}/{}/{} {}:{}", p->GetUsername(), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute())), m_WhoLikedThisWindow));

		});

		QObject::connect(m_RepostButton, &TitledButton::clicked,[post]() {
			MainLayer::Get().RepostPost(post);
		});

		QObject::connect(m_SendButton, &TitledButton::clicked, [post, this]() {
			QApplication::clipboard()->setText(QString::fromStdString(fmt::format("post_id:{}",post->GetPostID())));
		});

		m_MainLayout->addWidget(m_BottomDiv.Widget);

		m_UsernameLabel->setText(QString::fromStdString(post->GetPosterName()));
		{
			std::string timestamp;
			const Time& time = post->GetTimeSent();

			timestamp += time.GetYear() + "/";
			timestamp += time.GetMonth() + "/";
			timestamp += time.GetDay() + " ";
			timestamp += time.GetHour() + ":";
			timestamp += time.GetMinute();
			m_TimestampLabel->setText(QString::fromStdString(timestamp));
		}

		//Styles
		m_UsernameLabel->setStyleSheet("color:black;font-weight:bold;");
		m_ContentLabel->setStyleSheet("color:black;font-weight:bold;");
		m_TimestampLabel->setStyleSheet("color:rgb(210,215,210);font-weight:bold;");

		setLayout(m_MainLayout);
		setStyleSheet("background-color:rgb(86, 152, 144);");
	}
}