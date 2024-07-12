#pragma once

#include "Core/Post.h"
#include "Core/Person.h"
#include "Core/Company.h"
#include "CustomUI/TreeNode.h"
#include "ComboboxList.h"

#include "Layers/PostUI.h"
#include "Layers/CommentUI.h"
#include "Layers/MainLayer.h"

#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>
#include <QMessageBox>
#include <QTextEdit>
#include <QStyle>

#include <QTextEdit>
#include <QCombobox>
#include <QLineEdit>

#include <QWidget>

namespace LinkedOut {
	class JobPanelUI : public QWidget{
		Q_OBJECT
	public:
		JobPanelUI(QWidget* parent)
			:QWidget(parent)
		{}
		
		virtual void Show() = 0;
		virtual void Hide() = 0;

	};


	class UserJobUI : public QWidget {
		Q_OBJECT
	public:
		UserJobUI(Ref<Job> job,PersonJobs::State state, QWidget* parent)
			:QWidget(parent), m_Job(job)
		{
			SetupJobUI(parent,state);
			setLayout(m_Layout);
			m_Layout->addWidget(m_TreeNode);
		}
		~UserJobUI() {
			delete m_TreeNode;
		}

	private:
		void SetupJobUI(QWidget* parent,PersonJobs::State state) {
			m_TreeNode = new TreeNode(QString::fromStdString(m_Job->GetJobName()), this);

			m_TreeNode->SetHeaderStyleSheet("background-color:white;color:black;font-weight:bold;border-radius:5px;");
			m_TreeNode->SetTabStyleSheet("background-color:white;color:black;font-weight:bold;border-radius:5px;");

			m_Layout = new QVBoxLayout;

			VDivision* mainDiv = new VDivision(m_TreeNode);
			m_Job->Serialize(mainDiv->Widget);
			HDivision div(mainDiv->Widget);
			QPushButton* btn = new QPushButton("Apply for position", div.Widget);
			
			div.Layout->addWidget(btn);

			btn->setStyleSheet("background-color:rgb(51,48,65);color:white;");
			QObject::connect(btn, &QPushButton::clicked, [this]() {
				MainLayer::Get().MakeJobRequest(m_Job->GetID());
				m_TreeNode->SetHeaderStyleSheet("background-color:aqua;color:black;font-weight:bold;border-radius:5px;");
			});
			
			mainDiv->Layout->addWidget(div.Widget);

			m_TreeNode->AddToTab(mainDiv->Widget);

			switch (state)
			{
			case LinkedOut::PersonJobs::State::None:break;
			case LinkedOut::PersonJobs::State::Pending:
			{
				m_TreeNode->SetHeaderStyleSheet("background-color:aqua;color:black;font-weight:bold;border-radius:5px;");
				break;
			}
			}
		}
	private:
		Ref<Job> m_Job;
		
		QVBoxLayout* m_Layout;
		TreeNode* m_TreeNode;
	};

	class PersonJobsLayer : public QWidget {
		Q_OBJECT
	public:
		explicit PersonJobsLayer(QWidget* parent = nullptr)
			: QWidget(parent){
			m_MainLayout = new QVBoxLayout(this);

			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			m_LoadMoreLabel = new ClickableLabel(m_ScrollWidget);
			m_LoadMoreLabel->setText("More...");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
				});
			
			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();

			m_MainLayout->addWidget(m_ScrollArea);
		}

		void AddJob(PersonJobs job) {
			m_Jobs.push_back(job);
		}

		void ShowMore(uint32_t amount) {
			uint32_t shouldShow = std::min(m_CurrentShown + amount, (uint32_t)m_Jobs.size());

			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
				QLayoutItem* item = scrollLayout->itemAt(scrollLayout->count() - 1);
				scrollLayout->removeItem(item);
			}

			uint32_t currentShown = m_CurrentShown;

			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());

			//Show normal posts
			for (; currentShown < m_Jobs.size(); ++currentShown) {
				UserJobUI* job = new UserJobUI(m_Jobs[currentShown].Job, m_Jobs[currentShown].JobState, m_ScrollWidget);
				//QLabel* job = new QLabel("Job", m_ScrollWidget);
				if (scrollLayout) {
					scrollLayout->addWidget(job);
				}
			}

			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
				scrollLayout->addWidget(m_LoadMoreLabel);
				scrollLayout->addStretch();
			}

			m_CurrentShown = shouldShow;
		}

		void ClearPosts() {
			m_Jobs.clear();


			delete m_ScrollWidget;
			delete m_ScrollArea;

			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			m_LoadMoreLabel = new ClickableLabel(m_ScrollWidget);
			m_LoadMoreLabel->setText("More...");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
				});

			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();

			m_MainLayout->addWidget(m_ScrollArea);

			m_CurrentShown = 0;
		}

		QVBoxLayout* GetScrollLayout() {
			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
			return scrollLayout;
		}
	private:

		QVBoxLayout* m_MainLayout;
		QScrollArea* m_ScrollArea;
		QWidget* m_ScrollWidget;
		ClickableLabel* m_LoadMoreLabel;

		std::vector<PersonJobs> m_Jobs;
		uint32_t m_CurrentShown = 0;

	};

	class PersonJobPanelUI : public JobPanelUI {
	public:
		PersonJobPanelUI(Ref<Person> p, QWidget* parent)
			:m_Person(p), JobPanelUI(parent)
		{
			//Main frame
			{
				m_MainFrame = new QFrame(this);
				m_MainFrame->setObjectName(QString::fromUtf8("Home_MainFrame"));
				m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;"));
				m_MainFrameLayout = new QHBoxLayout;
				m_MainFrameLayout->setContentsMargins(0, 0, 0, 0);
			}

			m_JobsLayer = new PersonJobsLayer(m_MainFrame);

			
			m_MainFrameLayout->addWidget(m_JobsLayer);

			m_MainFrame->setLayout(m_MainFrameLayout);
			m_MainLayout = new QVBoxLayout(this);
			m_MainLayout->addWidget(m_MainFrame);
		}

		void Show()override {
			show();
			m_MainFrame->show();
			m_JobsLayer->ClearPosts();
			
			auto jobs = MainLayer::Get().GetUserJobs(m_Person);

			for (auto& job : jobs) {
				//m_JobUIs.push_back(CreateScope<UserJobUI>(job,m_JobsScrollWidget));
				m_JobsLayer->AddJob(job);
			}

			m_JobsLayer->ShowMore(10);

		}
		void Hide()override {
			hide();
			m_MainFrame->hide();
		}
	private:
		Ref<Person> m_Person;
		QVBoxLayout* m_MainLayout;
		QFrame* m_MainFrame;
		QHBoxLayout* m_MainFrameLayout;
		PersonJobsLayer* m_JobsLayer;
	};

	class CompanyJobUI : public QWidget {
		Q_OBJECT
	public:
		CompanyJobUI(Ref<Company>company,Ref<Job> job, Ref<Person> person, QWidget* parent)
			:QWidget(parent), m_Job(job), m_Person(person),m_Company(company)
		{
			m_Layout = new QVBoxLayout;
			setLayout(m_Layout);
			SetupJobUI(parent);
			m_Layout->addWidget(m_TreeNode);
		}

		~CompanyJobUI()
		{
			delete m_TreeNode;
		}

	private:
		void SetupJobUI(QWidget* parent)
		{
			m_TreeNode = new TreeNode(QString::fromStdString(m_Job->GetJobName()), this);

			VDivision* mainDiv = new VDivision(m_TreeNode);
			m_Person->Serialize(mainDiv->Widget);
			m_Job->Serialize(mainDiv->Widget);

			HDivision bottomDiv(mainDiv->Widget);
			//bottomDiv.Layout->addStretch();
			
			auto acceptBtn = new QPushButton("Accept",bottomDiv.Widget);
			acceptBtn->setStyleSheet("background-color:green;");
			bottomDiv.Layout->addWidget(acceptBtn);
			QObject::connect(acceptBtn, &QPushButton::clicked, [=]() {
				qDebug("hello");
				MainLayer::Get().AcceptJobRequest(m_Company, m_Job, m_Person);
				MainLayer::Get().GetNavMenu()->SetActiveTab("Jobs");
				});


			auto rejectBtn = new QPushButton("Reject",bottomDiv.Widget);
			rejectBtn->setStyleSheet("background-color:red;");
			bottomDiv.Layout->addWidget(rejectBtn);
			QObject::connect(rejectBtn, &QPushButton::clicked, [=]() {
				qDebug("hello");
				MainLayer::Get().DenyJobRequest(m_Job->GetID(), m_Person);
				MainLayer::Get().GetNavMenu()->SetActiveTab("Jobs");
			});

			mainDiv->Layout->addWidget(bottomDiv.Widget);

			m_TreeNode->AddToTab(mainDiv->Widget);
		}
	private:

		Ref<Job> m_Job;
		Ref<Person> m_Person;
		Ref<Company> m_Company;
		QVBoxLayout* m_Layout; 
		TreeNode* m_TreeNode;
	};

	class CompanyJobsLayer : public QWidget {
		Q_OBJECT
	public:
		explicit CompanyJobsLayer(Ref<Company> company,QWidget* parent = nullptr)
			: QWidget(parent),m_Company(company)
		{
			m_MainLayout = new QVBoxLayout(this);

			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			m_LoadMoreLabel = new ClickableLabel(m_ScrollWidget);
			m_LoadMoreLabel->setText("More...");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
				});

			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();

			m_MainLayout->addWidget(m_ScrollArea);
		}

		void AddJob(Ref<Job> job,Ref<Person> person) {
			m_Jobs.push_back(job);
			m_People.push_back(person);
		}

		void ShowMore(uint32_t amount) {
			uint32_t shouldShow = std::min(m_CurrentShown + amount, (uint32_t)m_Jobs.size());

			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
				QLayoutItem* item = scrollLayout->itemAt(scrollLayout->count() - 1);
				scrollLayout->removeItem(item);
			}

			uint32_t currentShown = m_CurrentShown;

			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());

			//Show normal posts
			for (; currentShown < m_Jobs.size(); ++currentShown) {
				CompanyJobUI* job = new CompanyJobUI(m_Company,m_Jobs[currentShown],m_People[currentShown], m_ScrollWidget);
				//QLabel* job = new QLabel("Job", m_ScrollWidget);
				if (scrollLayout) {
					scrollLayout->addWidget(job);
				}
			}

			{
				QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
				scrollLayout->addWidget(m_LoadMoreLabel);
				scrollLayout->addStretch();
			}

			m_CurrentShown = shouldShow;
		}

		void ClearPosts() {
			m_Jobs.clear();


			delete m_ScrollWidget;
			delete m_ScrollArea;

			// Create scroll area and scroll widget
			m_ScrollWidget = new QWidget(this);
			m_ScrollArea = new QScrollArea(this);
			m_ScrollArea->setWidgetResizable(true);
			m_ScrollArea->setWidget(m_ScrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(m_ScrollWidget);
			//scrollLayout->addStretch(0);  // Ensure items are stacked from top to bottom
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			m_LoadMoreLabel = new ClickableLabel(m_ScrollWidget);
			m_LoadMoreLabel->setText("More...");
			m_LoadMoreLabel->setStyleSheet(
				"QLabel{color:rgb(0,0,238);}"
				"QLabel:hover{color:rgb(23,24,214);}");
			QObject::connect(m_LoadMoreLabel, &ClickableLabel::clicked, [this]() {
				ShowMore(10);
				});

			scrollLayout->addWidget(m_LoadMoreLabel);
			scrollLayout->addStretch();

			m_MainLayout->addWidget(m_ScrollArea);

			m_CurrentShown = 0;
		}

		QVBoxLayout* GetScrollLayout() {
			QVBoxLayout* scrollLayout = qobject_cast<QVBoxLayout*>(m_ScrollWidget->layout());
			return scrollLayout;
		}
	private:

		QVBoxLayout* m_MainLayout;
		QScrollArea* m_ScrollArea;
		QWidget* m_ScrollWidget;
		ClickableLabel* m_LoadMoreLabel;
		
		Ref<Company> m_Company;

		std::vector<Ref<Job>> m_Jobs;
		std::vector<Ref<Person>> m_People;
		uint32_t m_CurrentShown = 0;
	};

	class CompanyJobPanelUI : public JobPanelUI{
	public:
		CompanyJobPanelUI(Ref<Company> company,QWidget* parent)
			:JobPanelUI(parent),m_Company(company)
		{
			//Main frame
			{
				m_MainFrame = new QFrame(this);
				m_MainFrame->setObjectName(QString::fromUtf8("Home_MainFrame"));
				m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;"));
				m_MainFrameLayout = new QVBoxLayout;
				m_MainFrameLayout->setContentsMargins(0, 0, 0, 0);
			}

			auto newJobBtn = new QPushButton("Create Job", m_MainFrame);

			QObject::connect(newJobBtn, &QPushButton::clicked, [this]() {
				m_NewJobWindow = new PopupWindow(800, 600, m_MainFrame, PopupWindowFlagBit_Popup);
				m_NewJobWindow->show();
				SetupNewJobPopup();
			});

			m_MainFrameLayout->addWidget(newJobBtn);
			m_JobsLayer = new CompanyJobsLayer(m_Company,m_MainFrame);

			m_MainFrameLayout->addWidget(m_JobsLayer);

			m_MainFrame->setLayout(m_MainFrameLayout);
			m_MainLayout = new QVBoxLayout(this);
			m_MainLayout->addWidget(m_MainFrame);
		}


		void Show()override {
			show();
			m_MainFrame->show();
			m_JobsLayer->ClearPosts();

			auto jobs = MainLayer::Get().GetCompanyCreatedJobs(m_Company);

			for (auto& job : jobs) {
				//m_JobUIs.push_back(CreateScope<UserJobUI>(job,m_JobsScrollWidget));
				for (auto& p : job.PeopleRequested) {
					m_JobsLayer->AddJob(job.Job, p);
				}
			}

			m_JobsLayer->ShowMore(10);
		}
		void Hide()override {
			hide();
			m_MainFrame->hide();
		}
	private:
		void SetupNewJobPopup() {
			const char* workspaceTypes[] = {"None","On-Site","Remote","Hybrid"};
			const char* jobTypes[] = 
			{ 
			"None",
			"FullTime",
			"PartTime",
			"Contract",
			"Temporary",
			"Other",
			"Volunteer",
			"Internship" 
			};

			QWidget* scrollWidget = new QWidget(this);
			QScrollArea* scrollArea = new QScrollArea(this);
			scrollArea->setWidgetResizable(true);
			scrollArea->setWidget(scrollWidget);

			QVBoxLayout* scrollLayout = new QVBoxLayout(scrollWidget);
			scrollLayout->setDirection(QBoxLayout::TopToBottom);

			scrollWidget->setLayout(scrollLayout);

			QVBoxLayout* layout = new QVBoxLayout(m_NewJobWindow);
			m_NewJobWindow->setLayout(layout);
			
			QLineEdit* jobTitleEdit;
			QComboBox* workspaceTypeBox;
			QLineEdit* jobLocationEdit;
			QLineEdit* salaryEdit;
			QComboBox* jobTypeBox;

			ComboBoxList* skillList;
			
			{
				HDivision div(scrollWidget);
				QLabel* label = new QLabel("Job title:", div.Widget);
				div.Layout->addWidget(label);
				jobTitleEdit = new QLineEdit(div.Widget);
				div.Layout->addWidget(jobTitleEdit);
				scrollLayout->addWidget(div.Widget);
			}
			{
				VDivision div(scrollWidget);
				QLabel* label = new QLabel("Workspace type:", div.Widget);
				div.Layout->addWidget(label);
				workspaceTypeBox = new QComboBox(div.Widget);

				for (int i = 0; i < 4; ++i) {
					workspaceTypeBox->addItem(workspaceTypes[i]);
				}
				workspaceTypeBox->setCurrentIndex(0);
				div.Layout->addWidget(workspaceTypeBox);
				scrollLayout->addWidget(div.Widget);
			}
			{
				HDivision div(scrollWidget);
				QLabel* label = new QLabel("Job location:", div.Widget);
				div.Layout->addWidget(label);
				jobLocationEdit = new QLineEdit(div.Widget);
				div.Layout->addWidget(jobLocationEdit);
				scrollLayout->addWidget(div.Widget);
			}
			{
				VDivision div(scrollWidget);
				QLabel* label = new QLabel("Job type:", div.Widget);
				div.Layout->addWidget(label);
				jobTypeBox = new QComboBox(div.Widget);

				for (int i = 0; i < 8; ++i) {
					jobTypeBox->addItem(jobTypes[i]);
				}
				jobTypeBox->setCurrentIndex(0);
				div.Layout->addWidget(jobTypeBox);
				scrollLayout->addWidget(div.Widget);
			}
			{
				HDivision div(scrollWidget);
				QLabel* label = new QLabel("Salary:", div.Widget);
				div.Layout->addWidget(label);
				salaryEdit = new QLineEdit(div.Widget);

				salaryEdit->setValidator(new QIntValidator(salaryEdit));
				div.Layout->addWidget(salaryEdit);
				scrollLayout->addWidget(div.Widget);
			}
			{
				HDivision div(scrollWidget);
				QLabel* label = new QLabel("Required skills:", div.Widget);
				div.Layout->addWidget(label);

				skillList = new ComboBoxList(div.Widget);
				div.Layout->addWidget(skillList);

				for (auto& skill : PossibleJobSkillTags) {
					skillList->AddItem(skill);
				}
				scrollLayout->addWidget(div.Widget);
			}
			layout->addWidget(scrollArea);

			{
				HDivision div(m_NewJobWindow);
				div.Layout->addStretch();
				QPushButton* btn = new QPushButton("Create", div.Widget);
				div.Layout->addWidget(btn);
				QObject::connect(btn, &QPushButton::clicked, [=]() {
					if (jobTitleEdit->text().isEmpty()) {
						MainLayer::Get().Error("Job title cant be empty");
						return;
					}
					if (workspaceTypeBox->currentIndex() == 0) {
						MainLayer::Get().Error("Workspace type cant be none");
						return;
					}
					if (jobLocationEdit->text().isEmpty()) {
						MainLayer::Get().Error("Job location cant be empty");
						return;
					}
					if (jobTypeBox->currentIndex() == 0) {
						MainLayer::Get().Error("Job type cant be none");
						return;
					}
					if (salaryEdit->text().isEmpty()) {
						MainLayer::Get().Error("Salary cant be empty");
						return;
					}

					int salary = salaryEdit->text().toInt();
					std::string jobTitle = jobTitleEdit->text().toStdString();
					WorkspaceType workspaceType = (WorkspaceType)workspaceTypeBox->currentIndex();
					std::string jobLocation = jobLocationEdit->text().toStdString();
					JobType jobType = (JobType)jobTypeBox->currentIndex();

					MainLayer::Get().CreateJob(m_Company, salary, jobTitle, jobLocation, jobType, workspaceType, skillList->GetSelected());
					m_NewJobWindow->hide();
					m_NewJobWindow->deleteLater();
				});
				layout->addWidget(div.Widget);
			}
		}

	private:
		Ref<Company> m_Company;
		QVBoxLayout* m_MainLayout;
		QFrame* m_MainFrame;
		QVBoxLayout* m_MainFrameLayout;
		CompanyJobsLayer* m_JobsLayer;
		PopupWindow* m_NewJobWindow;
	};
}