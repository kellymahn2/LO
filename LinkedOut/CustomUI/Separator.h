#pragma once

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class Separator : public QFrame {
public:
    Separator(QWidget* parent = nullptr)
        : QFrame(parent)
    {
        setFrameShape(QFrame::HLine);
        setFrameShadow(QFrame::Sunken);
    }
};