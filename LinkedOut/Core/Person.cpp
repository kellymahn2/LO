#include "Person.h"

#include <QLabel>
#include <QLayout>

namespace LinkedOut {
	void Person::Serialize(QWidget* parent)
	{
		auto layout = parent->layout();
		{
			std::string str = fmt::format("First name: {}", m_FirstName);
			auto label = new QLabel(QString::fromStdString(str), parent);
			if (layout)
				layout->addWidget(label);
		}
		{
			std::string str = fmt::format("Last name: {}", m_LastName);
			auto label = new QLabel(QString::fromStdString(str), parent);
			if (layout)
				layout->addWidget(label);
		}
	}
}