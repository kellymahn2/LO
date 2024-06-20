#pragma once

#include <QLabel>
#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>

class SearchBar : public QWidget
{
    Q_OBJECT
public:
    SearchBar(QWidget* parent = nullptr) : QWidget(parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);

        QLabel* iconLabel = new QLabel(this);
        QPixmap pixmap("Resources/NavigationSearchBarIcon.png");
        iconLabel->setPixmap(pixmap);
        iconLabel->setFixedSize(pixmap.size());

        QLineEdit* lineEdit = new QLineEdit(this);
        lineEdit->setPlaceholderText("Search");

        QObject::connect(lineEdit, &QLineEdit::textChanged, this, [=](const QString& text) {
            iconLabel->setVisible(text.isEmpty());
            });

        layout->addWidget(iconLabel);
        layout->addWidget(lineEdit);

        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(5);

        setLayout(layout);
    }
};