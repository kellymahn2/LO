#pragma once

#include "Skill.h"
#include "Job.h"
#include "Account.h"


#include <vendor/fmt/fmt.h>
#include <unordered_map>


#define FIRST_NAME_OBJ_NAME "_FIRST_NAME"
#define LAST_NAME_OBJ_NAME "_LAST_NAME"
#define SKILL_OBJ_NAME(skill) fmt::format("_SKILL_{}",skill)


class QWidget;

namespace LinkedOut {
	class Person : public Account {
	public:

		void TakeJob(Ref<Job> job) {
			//TODO:take job.
		}

		const std::string& GetFirstName()const{return m_FirstName;}
		const std::string& GetLastName()const{return m_LastName;}
		const std::vector<std::string>& GetSkills()const { return m_Skills; }

		void SetFirstName(const std::string& FirstName){ m_FirstName = FirstName;}
		void SetLastName(const std::string& LastName){ m_LastName = LastName;}
		void SetSkills(const std::vector<std::string>& Skills) { m_Skills = Skills; }

		void Follow(const std::string& id);

		void Serialize(QWidget* parent);

		virtual AccountType GetAccountType()const { return AccountType::Person; }
		virtual HomeLayerUI* MakeHomeLayer(QWidget* parent) override;
		virtual MessagePanelUI* MakeMessagePanel(QWidget* parent) override;

		void SetJob(Ref<Job> job) { m_Job = job; }
		Ref<Job> GetJob()const { return m_Job; }
		// Inherited via Account
		JobPanelUI* MakeJobLayer(QWidget* parent) override;

		// Inherited via Account
		NetworkPanelUI* MakeNetworkLayer(QWidget* parent) override;

		// Inherited via Account
		ProfilePanelUI* MakeProfilePanelUI(bool currentUser, QWidget* parent) override;
		OtherProfilePanelUI* MakeOtherProfilePanelUI(QWidget* parent) override;


		// Inherited via Account
		LoginLayerUI* MakeLoginUI(QWidget* parent) override;
		SignupLayerUI* MakeSignupUI(QWidget* parent) override;
	private:
		std::string m_FirstName;
		std::string m_LastName;
		std::vector<std::string> m_Skills;
		Ref<Job> m_Job;
	};
}