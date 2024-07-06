#pragma once
#include "Core/Layer.h"
#include "CustomUI/Division.h"
#include "Core/Account.h"
#include "Core/Person.h"
#include <QLabel>

namespace LinkedOut {

	class RequestUI : public QWidget{
		Q_OBJECT
	public:
		RequestUI(Ref<Person> acc,QWidget* parent)
			:QWidget(parent), m_Account(acc)
		{
			SetupRequestUI(parent);
			setLayout(m_MainDiv);
		}
		~RequestUI();
	private:
		void SetupRequestUI(QWidget* parent);
	private:
		Ref<Person> m_Account;
		
		QHBoxLayout* m_MainDiv;
	};

	class SuggestUI : public QWidget {
		Q_OBJECT
	
	public:
		SuggestUI(Ref<Person> acc, QWidget* parent)
			:QWidget(parent), m_Account(acc)
		{
			SetupSuggestUI(parent);
			setLayout(m_MainDiv);
		}
		~SuggestUI() {
			delete m_MainDiv;
		}
	private:
		void SetupSuggestUI(QWidget* parent);
	private:
		Ref<Person> m_Account;
		QVBoxLayout* m_MainDiv;
	};



	class NetworkUser : public Layer{
	public:
		NetworkUser()
			:Layer("NetworkUser")
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
		HDivision m_MainDiv;
		VDivision m_RequestMainDiv;
		VDivision m_RequestDiv;

		VDivision m_SuggestionMainDiv;
		VDivision m_SuggestionDiv;
		std::vector<Scope<RequestUI>> m_Requests;
	};
}