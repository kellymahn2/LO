#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>

class TitledButton : public QWidget
{
    Q_OBJECT

public:
    TitledButton(const QIcon& pixmap, uint32_t width, uint32_t height, uint32_t padding, const QString& title, QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);

        QPushButton* button = new QPushButton(this);

        button->setIcon(pixmap);
        button->setIconSize({ (int)width,(int)height });
        button->setFixedSize({ (int)width + (int)padding,(int)height + (int)padding });

        QLabel* label = new QLabel(title, this);
        label->setAlignment(Qt::AlignCenter);

        layout->addWidget(button);
        layout->addWidget(label);

        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(5);

        setLayout(layout);

        connect(button, &QPushButton::clicked, this, &TitledButton::emitClickedSignal);
    }

signals:
    void clicked();
private slots:
    void emitClickedSignal()
    {
        emit clicked();
    }   
};