#include "OtherProfilePanel.h"
#include "CustomUI/ProfilePanelUI.h"

namespace LinkedOut {
	OtherProfilePanel::OtherProfilePanel(QWidget* parent)
		:NavPanel(parent)
	{
		m_MainLayout = new QVBoxLayout(this);
		m_MainLayout->setContentsMargins(0, 0, 0, 0);
		setLayout(m_MainLayout);
	}
	OtherProfilePanel::~OtherProfilePanel()
	{
		delete m_ProfilePanelUI;
	}
	void OtherProfilePanel::Show()
	{
		if (m_ProfilePanelUI)
			m_ProfilePanelUI->Show();
		show();
	}
	void OtherProfilePanel::Hide()
	{
		if (m_ProfilePanelUI)
			m_ProfilePanelUI->Hide();
		hide();
	}
	void OtherProfilePanel::SetContext(Ref<Account> context)
	{
		m_Account = context;
		delete m_ProfilePanelUI;
		m_ProfilePanelUI = m_Account->MakeOtherProfilePanelUI(this);
		m_MainLayout->addWidget(m_ProfilePanelUI);
	}
	void OtherProfilePanel::Clear()
	{
		delete m_ProfilePanelUI;
		m_ProfilePanelUI = nullptr;
	}
}