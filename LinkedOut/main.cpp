#define TEST 0

#if TEST

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QVector>
#include <QMessageBox>
#include <vector>
#include <string>
#include <algorithm>



#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

class YourClass : public QWidget {
    Q_OBJECT

public:
    YourClass(QWidget* parent = nullptr) : QWidget(parent) {
        QHBoxLayout* layout = new QHBoxLayout;

        QPushButton* button1 = new QPushButton("Button 1");
        layout->addWidget(button1);

        layout->addStretch();

        QPushButton* button2 = new QPushButton("Button 2");
        layout->addWidget(button2);

        setLayout(layout);

        connect(button2, &QPushButton::clicked, this, &YourClass::yourSlotFunction);
    }

private slots:
    void yourSlotFunction() {
        qDebug() << "Button 2 clicked!";
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    YourClass window;
    window.show();

    return app.exec();
}

#include "main.moc"





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

