#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class TreeNode : public QWidget
{
    Q_OBJECT

public:
    explicit TreeNode(const QString& headerText, QWidget* parent = nullptr)
        : QWidget(parent), isContentVisible(false)
    {
        toggleButton = new QPushButton(headerText, this);
        toggleButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        toggleButton->setCheckable(true);
        toggleButton->setChecked(false);
        toggleButton->setIcon(QIcon(":/icons/arrow_right.png"));
        toggleButton->setIconSize(QSize(16, 16));
        toggleButton->setStyleSheet("QPushButton { text-align:left; padding-left: 10px; }");

        contentWidget = new QWidget(this);
        contentLayout = new QVBoxLayout(contentWidget);
        contentWidget->setVisible(isContentVisible);

        mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(toggleButton);
        mainLayout->addWidget(contentWidget);

        setLayout(mainLayout);

        connect(toggleButton, &QPushButton::clicked, this, &TreeNode::ToggleContentVisibility);
    }

    void SetHeaderStyleSheet(const QString& styleSheet)
    {
        toggleButton->setStyleSheet(styleSheet);
    }

    void SetTabStyleSheet(const QString& styleSheet)
    {
        contentWidget->setStyleSheet(styleSheet);
    }

    void AddToTab(QWidget* widget)
    {
        contentLayout->addWidget(widget);
    }

private slots:
    void ToggleContentVisibility()
    {
        isContentVisible = !isContentVisible;
        contentWidget->setVisible(isContentVisible);
    }

private:
    QPushButton* toggleButton;
    QWidget* contentWidget;
    QVBoxLayout* mainLayout;
    QVBoxLayout* contentLayout;
    bool isContentVisible;
};
