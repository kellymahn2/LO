#pragma once
#include "Skill.h"

#include <string>
#include <vector>

#include <vendor/fmt/fmt.h>

#define JOB_TITLE_OBJ_NAME "_JOB_TITLE"
#define COMPANY_TITLE_OBJ_NAME "_COMPANY_TITLE"
#define SALARY_OBJ_NAME "_SALARY"
#define LOCATION_OBJ_NAME "_LOCATION"
#define WORKSPACE_TYPE_OBJ_NAME "_WORKSPACE_TYPE"
#define JOB_TYPE_OBJ_NAME "_JOB_TYPE"
#define REQUIRED_SKILL_OBJ_NAME(skill) fmt::format("_REQUIRED_SKILL_{}",skill)


class QWidget;
class QLayout;



namespace LinkedOut {

	enum class WorkspaceType {
		None,
		OnSite,
		Remote,
		Hybrid
	};

	enum class JobType {
		None,
		FullTime,
		PartTime, 
		Contract,
		Temporary,
		Other,
		Volunteer,
		Internship
	};

	class Job {
	public:

        uint64_t GetSalary() const {
            return m_Salary;
        }

        void SetSalary(uint64_t salary) {
            m_Salary = salary;
        }

        const std::string& GetJobName() const {
            return m_JobName;
        }

        void SetJobName(const std::string& jobName) {
            m_JobName = jobName;
        }

        const std::string& GetCompanyName() const {
            return m_CompanyName;
        }

        void SetCompanyName(const std::string& companyName) {
            m_CompanyName = companyName;
        }

        const std::vector<std::string>& GetRequiredSkills() const {
            return m_RequiredSkillTags;
        }

        void SetRequiredSkills(const std::vector<std::string>& requiredSkills) {
            m_RequiredSkillTags = requiredSkills;
        }

        WorkspaceType GetWorkspaceType() const {
            return m_WorkspaceType;
        }

        void SetWorkspaceType(WorkspaceType workspaceType) {
            m_WorkspaceType = workspaceType;
        }

        const std::string& GetLocation() const {
            return m_Location;
        }

        void SetLocation(const std::string& location) {
            m_Location = location;
        }

        JobType GetJobType() const {
            return m_JobType;
        }

        void SetJobType(JobType jobType) {
            m_JobType = jobType;
        }

        void Serialize(QWidget* parent);

        auto& GetID()const { return m_ID; }
        void SetID(const std::string& id) { m_ID = id; }

    private:

	private:
		uint64_t m_Salary;
		std::string m_JobName;
		std::string m_CompanyName;
		std::vector<std::string> m_RequiredSkillTags;
		WorkspaceType m_WorkspaceType;
		std::string m_Location;
		JobType m_JobType;
        std::string m_ID;
	};

}

