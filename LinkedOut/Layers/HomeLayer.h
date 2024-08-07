#pragma once


#include "NavPanel.h"
#include "CustomUI/Division.h"
#include "CustomUI/Separator.h"
#include "CustomUI/ClickableLabel.h"
#include "CustomUI/TitledButton.h"
#include "CustomUI/PopupWindow.h"

#include "Core/Account.h"


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
class HomeLayerUI;

namespace LinkedOut {

	class MainLayer;

	class HomeLayer : public  NavPanel{
	public:
		HomeLayer(QWidget* parent);
		~HomeLayer();

		void Show() override;

		void Hide() override;

		/*void AddPost(Ref<Post> post) { 
			m_PostsLayout->AddPost(post);
		}
		void ClearPosts() {
			m_PostsLayout->ClearPosts();
		}

		void ShowMore(uint32_t amount) {
			m_PostsLayout->ShowMore(amount);
		}*/
		void SetContext(Ref<Account> person);
		void Clear()override;
	private:
		QVBoxLayout* m_MainLayout;
		HomeLayerUI* m_HomeLayerUI = nullptr;
		Ref<Account> m_Account;
	};
}