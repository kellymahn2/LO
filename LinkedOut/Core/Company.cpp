#include "Company.h"

#include "Core/Window.h"
#include "Layers/MainLayer.h"
#include "CustomUI/Division.h"
#include "CustomUI/HomeLayerUI.h"
#include "CustomUI/JobPanelUI.h"
#include "CustomUI/NetworkPanelUI.h"
#include "CustomUI/ProfilePanelUI.h"
#include "CustomUI/LoginUI.h"
#include "CustomUI/SignupUI.h"
#include "CustomUI/MessagingPanelUI.h"

#include <QFrame>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QMainWindow>
#include <QBoxLayout>
namespace LinkedOut {
	HomeLayerUI* Company::MakeHomeLayer(QWidget* parent)
	{
		return new CompanyHomeLayerUI(MainLayer::Get().GetCompany(m_AccountID),parent);
	}
	JobPanelUI* LinkedOut::Company::MakeJobLayer(QWidget* parent)
	{
		return new CompanyJobPanelUI(MainLayer::Get().GetCompany(m_AccountID), parent);
	}
	LoginLayerUI* Company::MakeLoginUI(QWidget* parent)
	{
		return new CompanyLoginLayerUI(parent);
	}
	void Company::Serialize(QWidget* parent)
	{
		auto layout = parent->layout();
		{
			std::string str = fmt::format("Company name: {}", m_CompanyName);
			auto label = new QLabel(QString::fromStdString(str), parent);
			if (layout)
				layout->addWidget(label);
		}
		{
			std::string str = fmt::format("Phone number: {}", m_PhoneNumber);
			auto label = new QLabel(QString::fromStdString(str), parent);
			if (layout)
				layout->addWidget(label);
		}
	}
	SignupLayerUI* Company::MakeSignupUI(QWidget* parent)
	{
		return new CompanySignupLayerUI(parent);
	}
	NetworkPanelUI* Company::MakeNetworkLayer(QWidget* parent)
	{
		return new CompanyNetworkPanelUI(MainLayer::Get().GetCompany(m_AccountID),parent);
	}
	ProfilePanelUI* Company::MakeProfilePanelUI(bool currentUser, QWidget* parent)
	{
		return new CompanyProfilePanelUI(MainLayer::Get().GetCompany(m_AccountID), true, parent);
	}
	MessagePanelUI* Company::MakeMessagePanel(QWidget* parent)
	{
		return new CompanyMessagePanelUI(MainLayer::Get().GetCompany(m_AccountID),parent);
	}
	OtherProfilePanelUI* Company::MakeOtherProfilePanelUI(QWidget* parent)
	{
		return new OtherCompanyProfilePanelUI(MainLayer::Get().GetCompany(m_AccountID),false,parent);
	}
}

