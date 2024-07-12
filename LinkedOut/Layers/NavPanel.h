#pragma once
#include "Core/Account.h"

#include <QWidget>
namespace LinkedOut {

	class NavPanel : public QWidget{
		Q_OBJECT
	public:
		NavPanel(QWidget* parent)
			:QWidget(parent)
		{}
		
		
		virtual ~NavPanel() {}

		virtual void Show() = 0;
		virtual void Hide() = 0;

		virtual void SetContext(Ref<Account> account) = 0;
		virtual void Clear() = 0;
	};

}