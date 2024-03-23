#include "LinkedOut.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LinkedOut w;
    w.show();
    return a.exec();
}
