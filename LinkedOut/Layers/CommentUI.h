#pragma once

#include "Core/Layer.h"
#include "CustomUI/Division.h"
#include "CustomUI/Separator.h"
#include "CustomUI/ClickableLabel.h"
#include "CustomUI/TitledButton.h"
#include "CustomUI/PopupWindow.h"

#include "Core/Post.h"
#include "PostUI.h"


#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QMessageBox>
#include <QTextEdit>
#include <QStyle>

#include <QTextEdit>
namespace LinkedOut {
	class CommentUI : public QFrame {
	public:

		CommentUI(Ref<Comment> comment, QWidget* parent = nullptr)
			: QFrame(parent),
			m_HeaderDiv(this),
			m_UsernameLabel(new QLabel("Username", this)),
			m_ContentDiv(this),
			m_ContentLabel(new ClickableLabel(this)),
			m_TimestampLabel(new QLabel("Timestamp", this)),
			m_TopSeparator(new Separator(this)),
			m_MainLayout(new QVBoxLayout)
		{

			//Header
			m_HeaderDiv.Layout->addWidget(m_UsernameLabel);
			m_HeaderDiv.Layout->addWidget(m_TimestampLabel);
			m_HeaderDiv.Layout->setContentsMargins(0, 0, 0, 0);

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


			QString postText = ShortenText(QString::fromStdString(comment->GetContentText()), 200, font());

			m_ContentLabel->setText(postText);

			if (postText == QString::fromStdString(comment->GetContentText())) {
				m_ContentLabel->setCursor(Qt::ArrowCursor);
			}
			else {
				QObject::connect(m_ContentLabel, &ClickableLabel::clicked, [this, postText, comment]() {
					if (m_ContentLabel->text() == postText)
						m_ContentLabel->setText(QString::fromStdString(comment->GetContentText()));
					else
						m_ContentLabel->setText(postText);
					});
			}

			m_ContentDiv.Layout->addWidget(m_ContentLabel);

			m_MainLayout->addWidget(m_ContentDiv.Widget);

			m_UsernameLabel->setText(QString::fromStdString(comment->GetCommenterName()));
			{
				std::string timestamp;
				const Time& time = comment->GetTimeSent();

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


		VDivision m_HeaderDiv;
		QLabel* m_UsernameLabel;
		QLabel* m_TimestampLabel;

		Separator* m_TopSeparator;

		HDivision m_ContentDiv;
		ClickableLabel* m_ContentLabel;

		QVBoxLayout* m_MainLayout;
	};
}