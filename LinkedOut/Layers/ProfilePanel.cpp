#include "ProfilePanel.h"
#include "CustomUI/ProfilePanelUI.h"


namespace LinkedOut {
	ProfilePanel::ProfilePanel(QWidget* parent)
		:NavPanel(parent)
	{
		m_MainLayout = new QVBoxLayout(this);
		m_MainLayout->setContentsMargins(0, 0, 0, 0);
		setLayout(m_MainLayout);
	}
	ProfilePanel::~ProfilePanel()
	{
		delete m_ProfilePanelUI;
	}
	void ProfilePanel::Show()
	{
		if (m_ProfilePanelUI)
			m_ProfilePanelUI->Show();
		show();
	}
	void ProfilePanel::Hide()
	{
		if (m_ProfilePanelUI)
			m_ProfilePanelUI->Hide();
		hide();
	}
	void ProfilePanel::SetContext(Ref<Account> context)
	{
		m_Account = context;
		delete m_ProfilePanelUI;
		m_ProfilePanelUI = m_Account->MakeProfilePanelUI(true,this);
		m_MainLayout->addWidget(m_ProfilePanelUI);
	}
	void ProfilePanel::Clear()
	{
		delete m_ProfilePanelUI;
		m_ProfilePanelUI = nullptr;
	}
}