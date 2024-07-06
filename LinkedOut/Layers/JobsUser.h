#pragma once

#include "Core/Layer.h"
#include "CustomUI/Division.h"
#include "CustomUI/TreeNode.h"

#include "Core/Person.h"
#include "Core/Job.h"



namespace LinkedOut {

	class UserJobUI : public QWidget {
	public:
		UserJobUI(Ref<Job> job, QWidget* parent)
			:QWidget(parent) , m_Job(job)
		{
			SetupJobUI(parent);
		}
		~UserJobUI() {
			delete m_TreeNode;
		}

	private:
		void SetupJobUI(QWidget* parent);
	private:
		Ref<Job> m_Job;
		TreeNode* m_TreeNode;
	};

	class JobsUser : public Layer{
	public:
		JobsUser()
			:Layer("JobsUser")
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
		std::vector<Scope<UserJobUI>> m_JobUIs;
	};

}