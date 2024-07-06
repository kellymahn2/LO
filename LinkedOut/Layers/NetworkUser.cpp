#include "NetworkUser.h"
#include "MainLayer.h"
#include "CustomUI/ClickableLabel.h"

namespace LinkedOut {
	void NetworkUser::Show()
	{
		m_Requests.clear();
		Ref<Person> p = std::dynamic_pointer_cast<Person>(MainLayer::Get().m_CurrentUser);
		std::vector<Ref<Person>> requestPeople = MainLayer::Get().GetUserRequests(p);
		if (requestPeople.empty()) {
			
		}
		else {
			for (auto& person : requestPeople) {
				m_Requests.push_back(CreateScope<RequestUI>(person, m_RequestDiv.Widget));
			}
		}
		m_MainDiv.Widget->show();
	}
	void NetworkUser::Hide()
	{
		m_Requests.clear();
		m_MainDiv.Widget->hide();
	}
	void* NetworkUser::GetMainFrame() const
	{
		return nullptr;
	}
	void NetworkUser::OnUpdate()
	{
	}
	void NetworkUser::SetupNetwork()
	{
		m_MainDiv = HDivision(MainLayer::Get().m_WindowCentralWidget);
		m_RequestMainDiv = VDivision(m_MainDiv.Widget);
		m_SuggestionMainDiv = VDivision(m_MainDiv.Widget);
		m_MainDiv.Layout->addWidget(m_RequestMainDiv.Widget);
		m_MainDiv.Layout->addWidget(m_SuggestionMainDiv.Widget);
	}
	RequestUI::~RequestUI()
	{
		delete m_MainDiv;
	}
	void RequestUI::SetupRequestUI(QWidget* parent)
	{
		m_MainDiv = new QHBoxLayout(parent);
		auto label = new ClickableLabel(this);
		label->setText(QString::fromStdString(m_Account->GetUsername()));

		m_MainDiv->addWidget(label);
		m_MainDiv->addStretch();
		m_MainDiv->addWidget(new QPushButton("Accept", this));
	}
	void SuggestUI::SetupSuggestUI(QWidget* parent)
	{
		m_MainDiv = new QVBoxLayout(parent);
		auto label = new ClickableLabel(this);
		label->setText(QString::fromStdString(m_Account->GetUsername()));
		m_MainDiv->addWidget(label);
		m_MainDiv->addWidget(new QPushButton("Follow", this));
	}
}

