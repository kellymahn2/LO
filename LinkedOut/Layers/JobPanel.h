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

	class JobPanel : public NavPanel {
	public:
		JobPanel(QWidget* parent);

		~JobPanel();

		void Show() override;

		void Hide() override;

		void SetContext(Ref<Account> context);
		void Clear() override;
	private:

	private:
		QVBoxLayout* m_MainLayout;
		JobPanelUI* m_JobPanelUI = nullptr;
		Ref<Account> m_Account;
	};


}