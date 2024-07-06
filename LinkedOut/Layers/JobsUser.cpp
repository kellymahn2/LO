#include "JobsUser.h"

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

	void JobsUser::SetupJobsUI()
	{
		m_MainDiv = VDivision(MainLayer::Get().m_WindowCentralWidget);
	}
	void JobsUser::Show()
	{
		m_JobUIs.clear();
		Ref<Person> p = std::dynamic_pointer_cast<Person>(MainLayer::Get().m_CurrentUser);
		std::vector<Ref<Job>> jobs = MainLayer::Get().GetUserJobs(p);

		for(auto& job : jobs){
			m_JobUIs.push_back(CreateScope<UserJobUI>(job, m_MainDiv.Widget));
		}
		m_MainDiv.Widget->show();
	}
	void JobsUser::Hide()
	{
		m_MainDiv.Widget->hide();
	}
	void* JobsUser::GetMainFrame() const
	{
		return nullptr;
	}
	void JobsUser::OnUpdate()
	{
	}
	void UserJobUI::SetupJobUI(QWidget* parent)
	{
		m_TreeNode = new TreeNode(QString::fromStdString(m_Job->GetJobName()), this);

		VDivision* mainDiv = new VDivision(m_TreeNode);
		m_Job->Serialize(mainDiv->Widget);

		m_TreeNode->AddToTab(mainDiv->Widget);
	}
}