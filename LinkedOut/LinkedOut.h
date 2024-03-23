#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LinkedOut.h"

class LinkedOut : public QMainWindow
{
    Q_OBJECT

public:
    LinkedOut(QWidget *parent = nullptr);
    ~LinkedOut();

private:
    Ui::LinkedOutClass ui;
};
