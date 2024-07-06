#pragma once
#include "Core/Layer.h"

#include "CustomUI/Division.h"

#include "Core/Person.h"

namespace LinkedOut {

	class CompanyFollowerUI : public QWidget {
		Q_OBJECT
	public:
		CompanyFollowerUI(Ref<Person> acc, QWidget* parent)
			:QWidget(parent), m_Account(acc)
		{
			SetupFollowerUI(parent);
			setLayout(m_MainDiv);
		}
		~CompanyFollowerUI() {
			delete m_MainDiv;
		}
	private:

		void SetupFollowerUI(QWidget* parent);

	private:
		Ref<Person> m_Account;
		QHBoxLayout* m_MainDiv;

	};


	class NetworkComany : public Layer {
	public:
		NetworkComany()
			:Layer("NetworkCompany")
		{
			SetupNetwork();
		}

		void Show() override;
		void Hide() override;
		void* GetMainFrame() const override;
		void OnUpdate() override;

	private:
		void SetupNetwork();
	private:
		VDivision m_MainDivision;
		std::vector<Scope<CompanyFollowerUI>> m_Followers;
	};
}