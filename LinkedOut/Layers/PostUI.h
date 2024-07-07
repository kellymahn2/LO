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
		PostUI(Ref<Post> post, bool needsFollowing, std::function<void(Ref<Post>)>&& commentsCallback, QWidget* parent = nullptr);

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
		ClickableLabel* m_FollowButton;
		Separator* m_TopSeparator;
		
		VDivision m_ContentDiv;
		ClickableLabel* m_ContentLabel;
		ClickableLabel* m_WhoLikedThis;
		PopupWindow* m_WhoLikedThisWindow;
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