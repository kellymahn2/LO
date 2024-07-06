#include "NetworkCompany.h"


#include "MainLayer.h"
namespace LinkedOut {
	
	void NetworkComany::Show()
	{
		m_Followers.clear();
		Ref<Company> p = std::dynamic_pointer_cast<Company>(MainLayer::Get().m_CurrentUser);
		std::vector<Ref<Person>> followers = MainLayer::Get().GetCompanyFollowers(p);

		for (auto& person : followers) {
			m_Followers.push_back(CreateScope<CompanyFollowerUI>(person, m_MainDivision.Widget));
		}
		m_MainDivision.Widget->show();

	}

	void NetworkComany::Hide()
	{
		m_Followers.clear();
		m_MainDivision.Widget->hide();
	}

	void* NetworkComany::GetMainFrame() const
	{
		return nullptr;
	}

	void NetworkComany::OnUpdate()
	{
	}
	void NetworkComany::SetupNetwork()
	{
		m_MainDivision = VDivision(MainLayer::Get().m_WindowCentralWidget);
	}
	void CompanyFollowerUI::SetupFollowerUI(QWidget* parent)
	{
		m_MainDiv = new QHBoxLayout(parent);
		auto label = new ClickableLabel(this);
		label->setText(QString::fromStdString(m_Account->GetUsername()));
		m_MainDiv->addWidget(label);
	}
}
