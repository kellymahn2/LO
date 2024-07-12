#include "HomeLayer.h"
#include "Core/Window.h"
#include "MainLayer.h"
#include "CustomUI/Division.h"

#include "CustomUI/HomeLayerUI.h"

#include <QFrame>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QMainWindow>
#include <QBoxLayout>

namespace LinkedOut {
	HomeLayer::HomeLayer(QWidget* parent)
		:NavPanel(parent)
	{
		m_MainLayout = new QVBoxLayout(this);
		m_MainLayout->setContentsMargins(0, 0, 0, 0);
		setLayout(m_MainLayout);
	}
	HomeLayer::~HomeLayer()
	{
		delete m_HomeLayerUI;
	}
	void HomeLayer::Show()
	{
		if (m_HomeLayerUI)
			m_HomeLayerUI->Show();
		show();
	}
	void HomeLayer::Hide()
	{
		if (m_HomeLayerUI)
			m_HomeLayerUI->Hide();
		hide();
	}
	void HomeLayer::SetContext(Ref<Account> person)
	{
		m_Account = person;
		delete m_HomeLayerUI;
		m_HomeLayerUI = m_Account->MakeHomeLayer(this);
		m_MainLayout->addWidget(m_HomeLayerUI);
	}
	void HomeLayer::Clear()
	{
		delete m_HomeLayerUI;
		m_HomeLayerUI = nullptr;
	}
}