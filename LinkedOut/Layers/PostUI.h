#pragma once
#include "Core/Base.h"
#include "Core/Post.h"

#include "CustomUI/ClickableLabel.h"
#include "CustomUI/Division.h"
#include "CustomUI/Separator.h"
#include "CustomUI/TitledButton.h"	
#include "CustomUI/PopupWindow.h"

#include <QFrame>
#include <functional>

namespace LinkedOut {

	class PostUI : public QFrame {
		Q_OBJECT

	public:
		PostUI(Ref<Post> post, std::function<void(Ref<Post>)>&& commentsCallback, QWidget* parent = nullptr)
			: QFrame(parent),
			m_HeaderDiv(this),
			m_UsernameDiv(this),
			m_UsernameLabel(new QLabel("Username", this)),
			m_ContentDiv(this),
			m_ContentLabel(new ClickableLabel(this)),
			m_TimestampLabel(new QLabel("Timestamp", this)),
			m_TopSeparator(new Separator(this)),
			m_BottomSeparator(new Separator(this)),
			m_BottomDiv(this),
			m_LikeButton(new TitledButton(QIcon("Resources/LikeIcon.png"), m_ButtonSize, m_ButtonSize,0,"Like", this)),
			m_CommentButton(new TitledButton(QIcon("Resources/CommentIcon.png"), m_ButtonSize, m_ButtonSize, 0, "Comment", this)),
			m_RepostButton(new TitledButton(QIcon("Resources/RepostIcon.png"), m_ButtonSize, m_ButtonSize, 0, "Repost", this)),
			m_SendButton(new TitledButton(QIcon("Resources/SendIcon.png"), m_ButtonSize, m_ButtonSize, 0, "Send", this)),
			m_MainLayout(new QVBoxLayout)
		{

			//Header
			m_UsernameDiv.Layout->addWidget(m_UsernameLabel);
			m_UsernameDiv.Layout->addWidget(m_TimestampLabel);
			m_UsernameDiv.Layout->setContentsMargins(0, 0, 0, 0);

			m_HeaderDiv.Layout->addWidget(m_UsernameDiv.Widget);

			//if (spec.NeedsFollowing) {
			//	m_HeaderDiv.Layout->addStretch();
			//	m_FollowButton = new ClickableLabel(this);
			//	m_FollowButton->setText("Follow");
			//	m_FollowButton->setStyleSheet("color:aqua;");
			//
			//	m_HeaderDiv.Layout->addWidget(m_FollowButton);
			//}
			m_HeaderDiv.Layout->setContentsMargins(0, 0, 0, 0);
			m_MainLayout->addWidget(m_HeaderDiv.Widget);
			m_TopSeparator->setStyleSheet("background-color:rgb(100, 165, 155);");
			m_MainLayout->addWidget(m_TopSeparator);
			
			
			QString postText = ShortenText(QString::fromStdString(post->GetContentText()), 200, font());
			
			m_ContentLabel->setText(postText);
			
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

	private:
		QString ShortenText(const QString& text, int maxWidth, const QFont& font, const QString& seeMoreText = " ...see more") {
			QFontMetrics metrics(font);
			int seeMoreWidth = metrics.horizontalAdvance(seeMoreText);

			if (metrics.horizontalAdvance(text) <= maxWidth) {
				return text;
			}

			QString shortenedText;
			for (int i = 0; i < text.length(); ++i) {
				shortenedText.append(text[i]);
				if (metrics.horizontalAdvance(shortenedText) + seeMoreWidth > maxWidth) {
					shortenedText.chop(1);
					break;
				}
			}

			return shortenedText + seeMoreText;
		}
	private:


		const uint32_t m_ButtonSize = 30;


		HDivision m_HeaderDiv;
		VDivision m_UsernameDiv;
		QLabel* m_UsernameLabel;
		QLabel* m_TimestampLabel;
		
		Separator* m_TopSeparator;
		
		HDivision m_ContentDiv;
		ClickableLabel* m_ContentLabel;
		
		Separator* m_BottomSeparator;
		HDivision m_BottomDiv;

		TitledButton* m_LikeButton;
		TitledButton* m_CommentButton;
		TitledButton* m_RepostButton;
		TitledButton* m_SendButton;
		QVBoxLayout* m_MainLayout;
		PopupWindow* m_CommentSectionWindow;
	};
}