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

namespace LinkedOut {
	class MessagePanel : public NavPanel {
	public:
		MessagePanel(QWidget* parent);

		~MessagePanel();

		void Show() override;

		void Hide() override;

		void SetContext(Ref<Account> context);

		void Clear() override;

	private:
		QVBoxLayout* m_MainLayout;
		MessagePanelUI* m_MessagePanelUI = nullptr;
		Ref<Account> m_Account;
	};
}
