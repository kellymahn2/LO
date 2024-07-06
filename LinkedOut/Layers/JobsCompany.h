#pragma once
#include "Core/Layer.h"
#include "CustomUI/Division.h"
#include "CustomUI/TreeNode.h"

#include "Core/Person.h"
#include "Core/Job.h"

namespace LinkedOut {

	class CompanyJobUI : public QWidget {
	public:
		CompanyJobUI(Ref<Job> job, Ref<Person> person, QWidget* parent)
			:QWidget(parent), m_Job(job), m_Person(person)
		{
			SetupJobUI(parent);
		}

		~CompanyJobUI()
		{
			delete m_TreeNode;
		}

	private:
		void SetupJobUI(QWidget* parent);
	private:

		Ref<Job> m_Job;
		Ref<Person> m_Person;
	
		TreeNode* m_TreeNode;
	};


	class JobsCompany : public Layer{
	public:
		JobsCompany()
			:Layer("JobsCompany")
		{
			SetupJobsUI();
		
		}


		void Show() override;

		void Hide() override;

		void* GetMainFrame() const override;

		void OnUpdate() override;
	private:
		void SetupJobsUI();
	private:
		VDivision m_MainDiv;

	};
}