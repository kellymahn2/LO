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

        virtual AccountType GetAccountType()const { return AccountType::Company; }

        HomeLayerUI* MakeHomeLayer(QWidget* parent) override;
        JobPanelUI* MakeJobLayer(QWidget* parent) override;
        LoginLayerUI* MakeLoginUI(QWidget* parent) override;
        SignupLayerUI* MakeSignupUI(QWidget* parent) override;

        virtual void Serialize(QWidget* parent) override;


        void AddEmployee(Ref<Person> person) {
            m_Employees.push_back(person);
        }

        void RemoveJob(Ref<Job> job) {
            auto mainIt = std::find(m_PostedJobs.begin(), m_PostedJobs.end(), job);
            if (mainIt == m_PostedJobs.end())
                return;
            m_PostedJobs.erase(mainIt);
        }



	private:
		std::vector<Ref<Job>> m_PostedJobs;
		std::string m_CompanyName;
		std::vector<Ref<Person>> m_Employees;

        // Inherited via Account
        NetworkPanelUI* MakeNetworkLayer(QWidget* parent) override;

        // Inherited via Account
        ProfilePanelUI* MakeProfilePanelUI(bool currentUser, QWidget* parent) override;
        OtherProfilePanelUI* MakeOtherProfilePanelUI(QWidget* parent) override;
        MessagePanelUI* MakeMessagePanel(QWidget* parent)override;
    };
}