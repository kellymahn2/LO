#pragma once

#include "Job.h"
#include "Account.h"

#include "Person.h"

#include <vector>

namespace LinkedOut {
	class Company : public Account {
	public:
		Ref<Job> CreateJob() { m_PostedJobs.push_back(CreateRef<Job>()); return m_PostedJobs.back(); }

        const std::vector<Ref<Job>>& GetPostedJobs() const {
            return m_PostedJobs;
        }

        void SetPostedJobs(const std::vector<Ref<Job>>& postedJobs) {
            m_PostedJobs = postedJobs;
        }

        const std::string& GetCompanyName() const {
            return m_CompanyName;
        }

        void SetCompanyName(const std::string& companyName) {
            m_CompanyName = companyName;
        }

        const std::vector<Ref<Person>>& GetEmployees() const {
            return m_Employees;
        }

        void SetEmployees(const std::vector<Ref<Person>>& employees) {
            m_Employees = employees;
        }

	private:
		std::vector<Ref<Job>> m_PostedJobs;
		std::string m_CompanyName;
		std::vector<Ref<Person>> m_Employees;
	};
}