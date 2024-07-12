#include "JobPanel.h"
#include "CustomUI/JobPanelUI.h"

namespace LinkedOut {
	JobPanel::JobPanel(QWidget* parent)
		:NavPanel(parent)
	{
		m_MainLayout = new QVBoxLayout(this);
		m_MainLayout->setContentsMargins(0, 0, 0, 0);
		setLayout(m_MainLayout);
	}
	JobPanel::~JobPanel()
	{
		delete m_JobPanelUI;
	}
	void JobPanel::Show()
	{
		if (m_JobPanelUI)
			m_JobPanelUI->Show();
		show();
	}
	void JobPanel::Hide()
	{
		if (m_JobPanelUI)
			m_JobPanelUI->Hide();
		hide();
	}
	void JobPanel::SetContext(Ref<Account> context)
	{
		m_Account = context;
		delete m_JobPanelUI;
		m_JobPanelUI = m_Account->MakeJobLayer(this);
		m_MainLayout->addWidget(m_JobPanelUI);
	}
	void JobPanel::Clear()
	{
		delete m_JobPanelUI;
		m_JobPanelUI = nullptr;
	}
}