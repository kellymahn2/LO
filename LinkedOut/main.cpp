#define TEST 0

#if TEST

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>

class TreeNode : public QWidget
{
    Q_OBJECT

public:
    explicit TreeNode(const QString& headerText, QWidget* parent = nullptr);

    void setHeaderStyleSheet(const QString& styleSheet);
    void setTabStyleSheet(const QString& styleSheet);
    void AddToTab(QWidget* widget);

private slots:
    void toggleContentVisibility();

private:
    QPushButton* toggleButton;
    QWidget* contentWidget;
    QVBoxLayout* mainLayout;
    QVBoxLayout* contentLayout;
    bool isContentVisible;
};

TreeNode::TreeNode(const QString& headerText, QWidget* parent)
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

    connect(toggleButton, &QPushButton::clicked, this, &TreeNode::toggleContentVisibility);
}

void TreeNode::setHeaderStyleSheet(const QString& styleSheet)
{
    toggleButton->setStyleSheet(styleSheet);
}

void TreeNode::setTabStyleSheet(const QString& styleSheet)
{
    contentWidget->setStyleSheet(styleSheet);
}

void TreeNode::AddToTab(QWidget* widget)
{
    contentLayout->addWidget(widget);
}

void TreeNode::toggleContentVisibility()
{
    isContentVisible = !isContentVisible;
    contentWidget->setVisible(isContentVisible);

    if (isContentVisible) {
        toggleButton->setIcon(QIcon(":/icons/arrow_down.png"));
    }
    else {
        toggleButton->setIcon(QIcon(":/icons/arrow_right.png"));
    }
}

#include "main.moc"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Load icons
    QFile::copy(":/icons/arrow_right.png", "./arrow_right.png");
    QFile::copy(":/icons/arrow_down.png", "./arrow_down.png");

    QWidget mainWindow;
    QVBoxLayout* mainLayout = new QVBoxLayout(&mainWindow);

    TreeNode* node1 = new TreeNode("Node 1 Header");
    node1->setHeaderStyleSheet("QPushButton { background-color: #F0F0F0; color: #333; font-weight: bold; border: 1px solid #CCC; padding: 5px; }");
    node1->setTabStyleSheet("QWidget { background-color: #FFF; border: 1px solid #CCC; padding: 10px; }");
    node1->AddToTab(new QLabel("Node 1 Tab Content"));

    TreeNode* node2 = new TreeNode("Node 2 Header");
    node2->setHeaderStyleSheet("QPushButton { background-color: #E0E0E0; color: #333; font-weight: bold; border: 1px solid #AAA; padding: 5px; }");
    node2->setTabStyleSheet("QWidget { background-color: #FFF; border: 1px solid #AAA; padding: 10px; }");
    node2->AddToTab(new QLabel("Node 2 Tab Content"));

    TreeNode* node3 = new TreeNode("Node 3 Header");
    node3->setHeaderStyleSheet("QPushButton { background-color: #D0D0D0; color: #333; font-weight: bold; border: 1px solid #888; padding: 5px; }");
    node3->setTabStyleSheet("QWidget { background-color: #FFF; border: 1px solid #888; padding: 10px; }");
    node3->AddToTab(new QLabel("Node 3 Tab Content"));

    mainLayout->addWidget(node1);
    mainLayout->addWidget(node2);
    mainLayout->addWidget(node3);

    mainWindow.setLayout(mainLayout);
    mainWindow.show();

    return app.exec();
}



#else

#include "Core/Application.h"

#include "Core/Window.h"

extern LinkedOut::Application* LinkedOut::CreateApplication(const LinkedOut::ApplicationSpecification& spec);

int main(int argc, char* argv[])
{
    LinkedOut::Application* application = LinkedOut::CreateApplication({ "Linked Out App",{argc,argv} });

    application->Run();

    delete application;
}

#endif // TEST

