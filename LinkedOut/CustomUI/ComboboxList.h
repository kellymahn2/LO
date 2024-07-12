#pragma once


#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QPushButton>

class ComboBoxList : public QWidget {
	Q_OBJECT
public:
	ComboBoxList(QWidget* parent)
		:QWidget(parent)
	{
		m_MainLayout = new QVBoxLayout(this);
		m_ComboBox = new QComboBox(this);
		m_MainLayout->addWidget(m_ComboBox);

		m_ScrollArea = new QScrollArea(this);
		m_ScrollWidget = new QWidget(m_ScrollArea);
		m_ScrollLayout = new QVBoxLayout(m_ScrollWidget);
		m_ScrollWidget->setLayout(m_ScrollLayout);
		m_ScrollArea->setWidgetResizable(true);
		m_ScrollArea->setWidget(m_ScrollWidget);

		m_ComboBox->addItem("Please choose an option");
		m_ComboBox->setCurrentIndex(0);

		m_MainLayout->addWidget(m_ScrollArea);

		auto c = &QComboBox::currentTextChanged;

		m_SelectFunc = [this](const QString& value) {
			static bool remove = false;
			if (value.isEmpty() || m_ComboBox->currentIndex() == 0)
				return;

			if (remove)
			{
				remove = false;
				return;
			}

			int index = m_ComboBox->currentIndex();

			QHBoxLayout* layout = new QHBoxLayout;
			QWidget* widget = new QWidget(m_ScrollWidget);
			layout->addWidget(new QLabel(value, widget));
			layout->addStretch();
			auto btn = new QPushButton("Remove", widget);
			layout->addWidget(btn);
			widget->setLayout(layout);
			m_ScrollLayout->removeItem(m_ScrollLayout->itemAt(m_ScrollLayout->count() - 1));

			m_ScrollLayout->addWidget(widget);
			m_ScrollLayout->addStretch();

			QObject::connect(btn, &QPushButton::clicked, [=]() {
				m_ScrollLayout->removeWidget(widget);
				widget->deleteLater();
				m_ComboBox->insertItem(index, value);
				auto ptr = std::find(m_Selected.begin(), m_Selected.end(), value.toStdString());
				if (ptr != m_Selected.end())
					m_Selected.erase(ptr);
				});

			remove = true;
			m_ComboBox->removeItem(m_ComboBox->currentIndex());
			m_ComboBox->setCurrentIndex(0);
			m_Selected.push_back(value.toStdString());
			};

		QObject::connect(m_ComboBox, &QComboBox::currentTextChanged, m_SelectFunc);
	}

	void AddItem(const std::string& item) {
		m_ComboBox->addItem(QString::fromStdString(item));
	}

	void Select(const std::string& item) {
		m_ComboBox->setCurrentIndex(m_ComboBox->findText(QString::fromStdString(item)));
	}

	const std::vector<std::string>& GetSelected()const { return m_Selected; }
private:
	QVBoxLayout* m_MainLayout;
	QComboBox* m_ComboBox;
	QScrollArea* m_ScrollArea;
	QWidget* m_ScrollWidget;
	QVBoxLayout* m_ScrollLayout;
	std::vector<std::string> m_Selected;
	int32_t m_CurrentIndex = 0;
	std::function<void(const QString& item)> m_SelectFunc;
};