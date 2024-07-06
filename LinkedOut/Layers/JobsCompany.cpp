#include "JobsCompany.h"
#include "MainLayer.h"

namespace LinkedOut {

	static const char* JobTypeToString(JobType type) {
		switch (type)
		{
		case LinkedOut::JobType::FullTime:return "Full-Time";
		case LinkedOut::JobType::PartTime:return "Part-Time";
		case LinkedOut::JobType::Contract:return "Contract";
		case LinkedOut::JobType::Temporary:return "Temporary";
		case LinkedOut::JobType::Other:return "Other";
		case LinkedOut::JobType::Volunteer:return "Volunteer";
		case LinkedOut::JobType::Internship:return "Internship";
		}
		return "";
	}
	static const char* WorkspaceTypeToString(WorkspaceType type) {
		switch (type)
		{
		case LinkedOut::WorkspaceType::OnSite:return "Onsite";
		case LinkedOut::WorkspaceType::Remote:return "Remote";
		case LinkedOut::WorkspaceType::Hybrid:return "Hybrid";
		}
		return "";
	}



	void JobsCompany::Show()
	{
	}
	void JobsCompany::Hide()
	{
	}
	void* JobsCompany::GetMainFrame() const
	{
		return nullptr;
	}
	void JobsCompany::OnUpdate()
	{
	}
	void JobsCompany::SetupJobsUI()
	{
		m_MainDiv = VDivision(MainLayer::Get().m_WindowCentralWidget);

		auto newJobBtn = new QPushButton("Create Job", m_MainDiv.Widget);
		m_MainDiv.Layout->addWidget(newJobBtn);
	}
	void CompanyJobUI::SetupJobUI(QWidget* parent)
	{

		m_TreeNode = new TreeNode(QString::fromStdString(m_Job->GetJobName()), this);

		VDivision* mainDiv = new VDivision(m_TreeNode);
		m_Person->Serialize(mainDiv->Widget);
		m_Job->Serialize(mainDiv->Widget);
		m_TreeNode->AddToTab(mainDiv->Widget);
	}
}
