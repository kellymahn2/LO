#include "Person.h"
#include "Core/Window.h"
#include "Layers/MainLayer.h"
#include "CustomUI/Division.h"
#include "CustomUI/HomeLayerUI.h"
#include "CustomUI/JobPanelUI.h"
#include "CustomUI/NetworkPanelUI.h"
#include "CustomUI/MessagingPanelUI.h"	
#include "CustomUI/ProfilePanelUI.h"
#include "CustomUI/LoginUI.h"
#include "CustomUI/SignupUI.h"


#include <QFrame>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QMainWindow>
#include <QBoxLayout>

namespace LinkedOut {
	void Person::Follow(const std::string& id)
	{
		m_FollowingID.push_back(id);
	}
	void Person::Serialize(QWidget* parent)
	{
		auto layout = parent->layout();
		{
			std::string str = fmt::format("First name: {}", m_FirstName);
			auto label = new QLabel(QString::fromStdString(str), parent);
			if (layout)
				layout->addWidget(label);
		}
		{
			std::string str = fmt::format("Last name: {}", m_LastName);
			auto label = new QLabel(QString::fromStdString(str), parent);
			if (layout)
				layout->addWidget(label);
		}
		{
			{
				std::string skills = fmt::format("Skills:");
				auto label = new QLabel(QString::fromStdString(skills), parent);
				if (layout)
					layout->addWidget(label);
			}

			for (auto& skill : m_Skills) {
				std::string str = fmt::format("  - {}", skill);
				auto label = new QLabel(QString::fromStdString(str), parent);
				if (layout)
					layout->addWidget(label);
			}
		}
	}
	HomeLayerUI* Person::MakeHomeLayer(QWidget* parent)
	{
		return new PersonHomeLayerUI(MainLayer::Get().GetPerson(m_AccountID), parent);
	}
	MessagePanelUI* Person::MakeMessagePanel(QWidget* parent)
	{
		return new PersonMessagePanelUI(MainLayer::Get().GetPerson(m_AccountID),parent);
	}
	JobPanelUI* Person::MakeJobLayer(QWidget* parent)
	{
		return new PersonJobPanelUI(MainLayer::Get().GetPerson(m_AccountID), parent);
	}
	NetworkPanelUI* Person::MakeNetworkLayer(QWidget* parent)
	{
		return new PersonNetworkPanelUI(MainLayer::Get().GetPerson(m_AccountID),parent);
	}
	ProfilePanelUI* Person::MakeProfilePanelUI(bool currentUser, QWidget* parent)
	{
		return new PersonProfilePanelUI(MainLayer::Get().GetPerson(m_AccountID), true, parent);
	}
	OtherProfilePanelUI* Person::MakeOtherProfilePanelUI(QWidget* parent)
	{
		return new OtherPersonProfilePanelUI(MainLayer::Get().GetPerson(m_AccountID),false,parent);
	}
	LoginLayerUI* Person::MakeLoginUI(QWidget* parent)
	{
		return new PersonLoginLayerUI(parent);
	}
	SignupLayerUI* Person::MakeSignupUI(QWidget* parent)
	{
		return new PersonSignupLayerUI(parent);
	}
}