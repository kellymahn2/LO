#pragma once

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
    


enum PopupWindowFlagBits {
    PopupWindowFlagBit_Widget = 0x00000000,
    PopupWindowFlagBit_Window = 0x00000001,
    PopupWindowFlagBit_Dialog = 0x00000002 | PopupWindowFlagBit_Window,
    PopupWindowFlagBit_Sheet = 0x00000004 | PopupWindowFlagBit_Window,
    PopupWindowFlagBit_Drawer = PopupWindowFlagBit_Sheet | PopupWindowFlagBit_Dialog,
    PopupWindowFlagBit_Popup = 0x00000008 | PopupWindowFlagBit_Window,
    PopupWindowFlagBit_Tool = PopupWindowFlagBit_Popup | PopupWindowFlagBit_Dialog,
    PopupWindowFlagBit_ToolTip = PopupWindowFlagBit_Popup | PopupWindowFlagBit_Sheet,
    PopupWindowFlagBit_SplashScreen = PopupWindowFlagBit_ToolTip | PopupWindowFlagBit_Dialog,
    PopupWindowFlagBit_Desktop = 0x00000010 | PopupWindowFlagBit_Window,
    PopupWindowFlagBit_SubWindow = 0x00000012,
    PopupWindowFlagBit_ForeignWindow = 0x00000020 | PopupWindowFlagBit_Window,
    PopupWindowFlagBit_CoverWindow = 0x00000040 | PopupWindowFlagBit_Window,
    PopupWindowFlagBit_FramelessWindowHint = 0x00000800,
    PopupWindowFlagBit_WindowTitleHint = 0x00001000,
    PopupWindowFlagBit_WindowMinimizeButtonHint = 0x00004000,
    PopupWindowFlagBit_WindowMaximizeButtonHint = 0x00008000,
    PopupWindowFlagBit_WindowMinMaxButtonsHint = PopupWindowFlagBit_WindowMinimizeButtonHint | PopupWindowFlagBit_WindowMaximizeButtonHint,
    
    PopupWindowFlagBit_WindowCloseButtonHint = 0x08000000,
    PopupWindowFlagBit_WindowFullscreenButtonHint = 0x80000000
};
typedef int PopupWindowFlags;
class PopupWindow : public QDialog {
    Q_OBJECT
public:
    
    PopupWindow(uint32_t width, uint32_t height, QWidget* parent = nullptr, PopupWindowFlags flags = 0) : QDialog(parent, { flags }) {
        setWindowTitle("Popup Window");
        resize((int)width, (int)height);
    }
    // Function to add widgets to the popup window at relative positions
    void addWidget(QWidget* widget, int x, int y) {
        widget->setParent(this);
    }

protected:
    void showEvent(QShowEvent* event) override {
        // Call the base class implementation
        QDialog::showEvent(event);

        // Emit a custom signal indicating the dialog is opened
        emit opened();
    }
    void closeEvent(QCloseEvent* event) override {
        // Call the base class implementation
        QDialog::closeEvent(event);

        // Emit a custom signal indicating the dialog is closed
        emit closed();
    }

signals:
    void opened();
    void closed();
private:
};
