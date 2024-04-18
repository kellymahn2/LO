#pragma once


#include <QLineEdit>
#include <QKeyEvent>


enum InputTypeFlagBits {
    InputTypeBit_Character = 1,
    InputTypeBit_Digit = 2
};

typedef int InputTypeFlag;

class UnitLengthInput : public QLineEdit {
public:
    UnitLengthInput(InputTypeFlag flag,QWidget *parent = nullptr) : QLineEdit(parent),m_InputFlag(flag) {
        // Set maximum input length to 1
        setMaxLength(1);
    }

protected:
    // Override keyPressEvent to handle keyboard input
    void keyPressEvent(QKeyEvent* event) override {
        // Accept only characters that are single and non-modifier keys
        if (event->text().length() == 1 && !event->isAutoRepeat()) {
            // Check if the input type flag matches the input character type
            QChar inputChar = event->text().at(0);
            bool isValidInput = ((m_InputFlag & InputTypeBit_Character) && inputChar.isLetter()) ||
                ((m_InputFlag & InputTypeBit_Digit) && inputChar.isDigit());
            if (isValidInput) {
                QLineEdit::keyPressEvent(event);
            }
        }
    }

private:
    InputTypeFlag m_InputFlag;
        
};