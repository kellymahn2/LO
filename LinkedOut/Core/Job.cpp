#include "Job.h"

#include <vendor/fmt/fmt.h>

#include <QLabel>

#include <QLayout>

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



	void Job::Serialize(QWidget* parent)
	{

		auto layout = parent->layout();

		{
			std::string str = fmt::format("Job title: {}", m_JobName);
			auto label = new QLabel(QString::fromStdString(str), parent);
			label->setObjectName(JOB_TITLE_OBJ_NAME);
			if(layout)
				layout->addWidget(label);
		}
		{
			std::string str = fmt::format("Company: {}", m_CompanyName);
			auto label = new QLabel(QString::fromStdString(str), parent);
			label->setObjectName(COMPANY_TITLE_OBJ_NAME);
			if (layout)
				layout->addWidget(label);
		}
		{
			std::string str = fmt::format("Salary: {}", m_Salary);
			auto label = new QLabel(QString::fromStdString(str), parent);
			label->setObjectName(SALARY_OBJ_NAME);
			if (layout)
				layout->addWidget(label);
		}
		{
			std::string str = fmt::format("Location: {}", m_Location);
			auto label = new QLabel(QString::fromStdString(str), parent);
			label->setObjectName(LOCATION_OBJ_NAME);
			if (layout)
				layout->addWidget(label);
		}
		{
			std::string str = fmt::format("Job type: {}", JobTypeToString(m_JobType));
			auto label = new QLabel(QString::fromStdString(str), parent);
			label->setObjectName(JOB_TYPE_OBJ_NAME);
			if (layout)
				layout->addWidget(label);
		}
		{
			std::string str = fmt::format("Workspace: {}", WorkspaceTypeToString(m_WorkspaceType));
			auto label = new QLabel(QString::fromStdString(str), parent);
			label->setObjectName(WORKSPACE_TYPE_OBJ_NAME);
			if (layout)
				layout->addWidget(label);
		}
		{
			auto mainLabel = new QLabel("Required skills: ", parent);
			mainLabel->setObjectName("_REQUIRED_SKILL_LABEL");
			
			if(layout)
				layout->addWidget(mainLabel);
			for (auto& skill : m_RequiredSkillTags) {
				std::string str = fmt::format("\t- {}", skill);
				auto label = new QLabel(QString::fromStdString(str), parent);
				label->setObjectName(QString::fromStdString(REQUIRED_SKILL_OBJ_NAME(skill)));
				if (layout)
					layout->addWidget(label);
			}
		}
	}

}