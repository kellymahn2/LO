#include "NetworkPanel.h"

#include "CustomUI/NetworkPanelUI.h"

namespace LinkedOut {
	NetworkPanel::NetworkPanel(QWidget* parent)
		:NavPanel(parent)
	{
		m_MainLayout = new QVBoxLayout(this);
		m_MainLayout->setContentsMargins(0, 0, 0, 0);
		setLayout(m_MainLayout);
	}
	NetworkPanel::~NetworkPanel()
	{
		delete m_NetworkPanelUI;
	}
	void NetworkPanel::Show()
	{
		if (m_NetworkPanelUI)
			m_NetworkPanelUI->Show();
		show();
	}
	void NetworkPanel::Hide()
	{
		if (m_NetworkPanelUI)
			m_NetworkPanelUI->Hide();
		hide();
	}
	void NetworkPanel::SetContext(Ref<Account> context)
	{
		m_Account = context;
		delete m_NetworkPanelUI;
		m_NetworkPanelUI = m_Account->MakeNetworkLayer(this);
		m_MainLayout->addWidget(m_NetworkPanelUI);
	}
	void NetworkPanel::Clear()
	{
		delete m_NetworkPanelUI;
		m_NetworkPanelUI = nullptr;
	}
}