#include "MessagePanel.h"

#include "CustomUI/MessagingPanelUI.h"

namespace LinkedOut {
	MessagePanel::MessagePanel(QWidget* parent)
		:NavPanel(parent)
	{
		m_MainLayout = new QVBoxLayout(this);
		m_MainLayout->setContentsMargins(0, 0, 0, 0);
		setLayout(m_MainLayout);
	}
	MessagePanel::~MessagePanel()
	{
		delete m_MessagePanelUI;
	}
	void MessagePanel::Show()
	{
		if (m_MessagePanelUI)
			m_MessagePanelUI->Show();
		show();
	}
	void MessagePanel::Hide()
	{
		if (m_MessagePanelUI)
			m_MessagePanelUI->Hide();
		hide();
	}
	void MessagePanel::SetContext(Ref<Account> context)
	{
		m_Account = context;
		delete m_MessagePanelUI;
		m_MessagePanelUI = m_Account->MakeMessagePanel(this);
		m_MainLayout->addWidget(m_MessagePanelUI);
	}
	void MessagePanel::Clear()
	{
		delete m_MessagePanelUI;
		m_MessagePanelUI = nullptr;
	}
}