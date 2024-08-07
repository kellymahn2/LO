#include "LoginLayer.h"

#include "MainLayer.h"
#include "Core/Window.h"
#include "CustomUI/ClickableLabel.h"
#include "CommonErrors.h"
#include "CustomUI/PopupWindow.h"
#include "CustomUI/UnitLengthInput.h"
#include "Core/Random.h"
#include "CustomUI/LoginUI.h"


#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QIntValidator>


namespace LinkedOut {

    LoginLayer::LoginLayer(MainLayer* mainLayer)
        :Layer("Login layer")
    {
        m_MainLayer = mainLayer;
        SetupLogin();
    }
    LoginLayer::~LoginLayer() {
        delete m_MainWidget;
    }

    void LoginLayer::OnUpdate() {
       /* auto window = Window::GetMainWindow()->GetNativeHandle();
        auto size = window->size();
        m_MainFrame->resize(size);

        auto frameSize = m_Frame->size();

        float startX = ((float)size.width() - (float)frameSize.width()) / 2.0f;
        float startY = ((float)size.height() - (float)frameSize.height()) / 2.0f;


        m_Frame->setGeometry(startX, startY, frameSize.width(), frameSize.height());

        if (!m_IsPopupOpen) {
            m_CodePopup->hide();
        }*/

    }

    void LoginLayer::Show() {
        m_MainWidget->show();
        /*m_MainFrame->show();
        m_IsShowing = true;*/
    }
    void LoginLayer::Hide() {
        m_MainWidget->hide();
        /*m_IsShowing = false;
        m_MainFrame->hide();*/
    }

	void LoginLayer::Cleanup() {
        //Code popup
        {
            delete m_UnitLengthInputs[3];
            delete m_UnitLengthInputs[2];
            delete m_UnitLengthInputs[1];
            delete m_UnitLengthInputs[0];

            delete m_PopupCode;

            delete m_CodePopup;
        }

        //Login button
        {
            delete m_LoginButton;
        }

        //Captcha text input
        {
            delete m_CaptchaValidator;
            delete m_CaptchaTextInput;
        }

        //Captcha label
        {
            delete m_CaptchaLabel;
        }

        //Captcha frame
        {
            delete m_CaptchaFrame;
        }

        //To signup page label
        {
            delete m_ToSignupLabel;
        }

        //Remember me checkbox
        {
            delete m_RememberMeCheckbox;
        }

        //Password text input
        {
            delete m_PasswordTextInput;
        }

        //Password label
        {
            delete m_PasswordLabel;
        }

        //Password frame
        {
            delete m_PasswordFrame;
        }

        //Username text input
        {
            delete m_UsernameTextInput;
        }

        //Username label
        {
            delete m_UsernameLabel;
        }

        //Username frame
        {
            delete m_UsernameFrame;
        }

        //Login label
        {
            delete m_Label;
        }

        //Login frame
        {
            delete m_Frame;
        }

        //Main frame
        {
            delete m_MainFrame;
        }

        delete m_CentralWidget;
	}

	void LoginLayer::SetupLogin(){
        VDivision mainDiv(MainLayer::Get().m_LayersWidget);
        mainDiv.Widget->setStyleSheet("background-color:rgb(51,52,54);color:white;");
        m_MainWidget = mainDiv.Widget;
        {
            HDivision div(mainDiv.Widget);
            auto personBtn = new QPushButton("Person", div.Widget);
            QObject::connect(personBtn, &QPushButton::clicked, [=]() {
                delete m_LayerUI;
                m_LayerUI = new PersonLoginLayerUI(mainDiv.Widget);
                mainDiv.Layout->addWidget(m_LayerUI);
                });
            auto companyBtn = new QPushButton("Company", div.Widget);
            QObject::connect(companyBtn, &QPushButton::clicked, [=]() {
                delete m_LayerUI;
                m_LayerUI = new CompanyLoginLayerUI(mainDiv.Widget);
                mainDiv.Layout->addWidget(m_LayerUI);
                });
            div.Layout->addWidget(personBtn);
            div.Layout->addWidget(companyBtn);
            mainDiv.Layout->addWidget(div.Widget);
        }
        m_LayerUI = new PersonLoginLayerUI(mainDiv.Widget);
        mainDiv.Layout->addWidget(m_LayerUI);

        MainLayer::Get().m_LayersLayout->addWidget(mainDiv.Widget);
        //auto window = Window::GetMainWindow()->GetNativeHandle();

        //QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        //sizePolicy.setHorizontalStretch(0);
        //sizePolicy.setVerticalStretch(0);
      

        ////Main frame
        //{
        //    m_MainFrame = new QFrame(m_MainLayer->m_WindowCentralWidget);
        //    m_MainFrame->setObjectName(QString::fromUtf8("Login_MainFrame"));
        //    m_MainFrame->setGeometry(QRect(0, 0, 1280, 720));
        //    sizePolicy.setHeightForWidth(m_MainFrame->sizePolicy().hasHeightForWidth());
        //    m_MainFrame->setSizePolicy(sizePolicy);
        //    m_MainFrame->setSizeIncrement(QSize(0, 0));
        //    m_MainFrame->setStyleSheet(QString::fromUtf8("background-color:rgb(51,52,54);color:white;width:inherited;"));
        //    m_MainFrame->setFrameShape(QFrame::StyledPanel);
        //    m_MainFrame->setFrameShadow(QFrame::Raised);
        //}
        //
        ////Login frame
        //{
        //    m_Frame = new QFrame(m_MainFrame);
        //    m_Frame->setObjectName(QString::fromUtf8("Login_Frame"));
        //    m_Frame->setGeometry(QRect(450, 110, 380, 500));
        //    m_Frame->setStyleSheet(QString::fromUtf8("background-color:rgb(86, 152, 144);\n"
        //        "border-style: solid; border-width: 2px; border-color: #000000;\n"
        //        "	border-radius: 20px;"));
        //    m_Frame->setFrameShape(QFrame::NoFrame);
        //    m_Frame->setFrameShadow(QFrame::Raised);
        //}
        //
        //QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        //
        ////Login Label
        //{
        //    m_Label = new QLabel(m_Frame);
        //    m_Label->setObjectName(QString::fromUtf8("Login_Label"));
        //    m_Label->setGeometry(QRect(150, 10, 80, 41));
        //    m_Label->setStyleSheet(QString::fromUtf8("border:none;font-weight:bold;font-size:20px;"));
        //    m_Label->setAlignment(Qt::AlignCenter);
        //    m_Label->setText(QApplication::translate("LinkedOutClass", "Login", nullptr));
        //}

        ////Username frame
        //{
        //    m_UsernameFrame = new QFrame(m_Frame);
        //    m_UsernameFrame->setObjectName(QString::fromUtf8("Username_Frame"));
        //    m_UsernameFrame->setGeometry(QRect(0, 60, 380, 141));
        //    sizePolicy1.setHorizontalStretch(0);
        //    sizePolicy1.setVerticalStretch(0);
        //    sizePolicy1.setHeightForWidth(m_UsernameFrame->sizePolicy().hasHeightForWidth());
        //    m_UsernameFrame->setSizePolicy(sizePolicy1);
        //    m_UsernameFrame->setStyleSheet(QString::fromUtf8("border-radius:0;border-top:none;border-bottom:none;\n"
        //        ""));
        //    m_UsernameFrame->setFrameShape(QFrame::NoFrame);
        //    m_UsernameFrame->setFrameShadow(QFrame::Raised);
        //}

        ////Username label
        //{
        //    m_UsernameLabel = new QLabel(m_UsernameFrame);
        //    m_UsernameLabel->setObjectName(QString::fromUtf8("Username_Label"));
        //    m_UsernameLabel->setGeometry(QRect(0, 50, 71, 41));
        //    m_UsernameLabel->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
        //    m_UsernameLabel->setText(QApplication::translate("LinkedOutClass", "User name: ", nullptr));
        //}

        ////Username text input
        //{
        //    m_UsernameTextInput = new QLineEdit(m_UsernameFrame);
        //    m_UsernameTextInput->setObjectName(QString::fromUtf8("Username_Text_Input"));
        //    m_UsernameTextInput->setGeometry(QRect(70, 50, 291, 41));
        //    m_UsernameTextInput->setAcceptDrops(false);
        //    m_UsernameTextInput->setStyleSheet(QString::fromUtf8("border-style: solid; border-width: 2px; border-color: #000000;"));
        //    m_UsernameTextInput->setMaxLength(9999);
        //    m_UsernameTextInput->setAlignment(Qt::AlignCenter);
        //    m_UsernameTextInput->setPlaceholderText(QApplication::translate("LinkedOutClass", "User name", nullptr));
        //}
       
        ////Password frame
        //{
        //    m_PasswordFrame = new QFrame(m_Frame);
        //    m_PasswordFrame->setObjectName(QString::fromUtf8("Password_Frame"));
        //    m_PasswordFrame->setGeometry(QRect(0, 200, 380, 141));
        //    sizePolicy1.setHeightForWidth(m_PasswordFrame->sizePolicy().hasHeightForWidth());
        //    m_PasswordFrame->setSizePolicy(sizePolicy1);
        //    m_PasswordFrame->setStyleSheet(QString::fromUtf8("border-radius:0;border-top:none;border-bottom:none;\n"
        //        ""));
        //    m_PasswordFrame->setFrameShape(QFrame::NoFrame);
        //    m_PasswordFrame->setFrameShadow(QFrame::Plain);
        //    m_PasswordFrame->setLineWidth(0);
        //}

        ////Password label
        //{
        //    m_PasswordLabel = new QLabel(m_PasswordFrame);
        //    m_PasswordLabel->setObjectName(QString::fromUtf8("Password_Label"));
        //    m_PasswordLabel->setGeometry(QRect(0, 50, 71, 41));
        //    m_PasswordLabel->setAlignment(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
        //    m_PasswordLabel->setText(QApplication::translate("LinkedOutClass", "Password:   ", nullptr));
        //}

        ////Password text input
        //{
        //    m_PasswordTextInput = new QLineEdit(m_PasswordFrame);
        //    m_PasswordTextInput->setObjectName(QString::fromUtf8("Password_Text_Input"));
        //    m_PasswordTextInput->setGeometry(QRect(70, 50, 291, 41));
        //    m_PasswordTextInput->setAcceptDrops(false);
        //    m_PasswordTextInput->setStyleSheet(QString::fromUtf8("border-style: solid; border-width: 2px; border-color: #000000;"));
        //    m_PasswordTextInput->setMaxLength(9999);
        //    m_PasswordTextInput->setAlignment(Qt::AlignCenter);
        //    m_PasswordTextInput->setPlaceholderText(QApplication::translate("LinkedOutClass", "Password", nullptr));
        //    m_PasswordTextInput->setEchoMode(QLineEdit::Password);
        //}

        ////Remember me checkbox
        //{
        //    m_RememberMeCheckbox = new QCheckBox(m_Frame);
        //    m_RememberMeCheckbox->setObjectName(QString::fromUtf8("Remember_Me_Checkbox"));
        //    m_RememberMeCheckbox->setGeometry(QRect(20, 320, 101, 17));
        //    m_RememberMeCheckbox->setStyleSheet(QString::fromUtf8("border:none;"));
        //    m_RememberMeCheckbox->setText(QApplication::translate("LinkedOutClass", "Remember Me", nullptr));
        //    m_RememberMeCheckbox->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        //}

        ////Captcha frame
        //{
        //    m_CaptchaFrame = new QFrame(m_Frame);
        //    m_CaptchaFrame->setObjectName(QString::fromUtf8("Captcha_Frame"));
        //    m_CaptchaFrame->setGeometry(QRect(0, 340, 380, 61));
        //    sizePolicy1.setHeightForWidth(m_CaptchaFrame->sizePolicy().hasHeightForWidth());
        //    m_CaptchaFrame->setSizePolicy(sizePolicy1);
        //    m_CaptchaFrame->setStyleSheet(QString::fromUtf8("border-radius:0;border-top:none;border-bottom:none;\n"
        //        ""));
        //    m_CaptchaFrame->setFrameShape(QFrame::NoFrame);
        //    m_CaptchaFrame->setFrameShadow(QFrame::Plain);
        //    m_CaptchaFrame->setLineWidth(0);
        //}
        //
        ////Captcha label
        //{
        //    m_CaptchaLabel = new QLabel(m_CaptchaFrame);
        //    m_CaptchaLabel->setObjectName(QString::fromUtf8("Captcha_Label"));
        //    m_CaptchaLabel->setGeometry(QRect(0, 10, 211, 41));
        //    m_CaptchaLabel->setStyleSheet(QString::fromUtf8("border-radius:0;border-top:none;border-bottom:none;font-style:italic;font-weight:bold;font-size:25px;border-right:none;\n"
        //        ""));
        //    
        //    m_CaptchaLabel->setAlignment(Qt::AlignCenter);
        //    m_CaptchaLabel->setText(QApplication::translate("LinkedOutClass", "Captcha: 5555", nullptr));
        //}

        ////Captcha text input
        //{
        //    m_CaptchaTextInput = new QLineEdit(m_CaptchaFrame);
        //    m_CaptchaTextInput->setObjectName(QString::fromUtf8("Captcha_Text_Input"));
        //    m_CaptchaTextInput->setGeometry(QRect(210, 10, 151, 41));
        //    m_CaptchaTextInput->setAcceptDrops(false);
        //    m_CaptchaTextInput->setStyleSheet(QString::fromUtf8("border-style: solid; border-width: 2px; border-color: #000000;"));
        //    m_CaptchaValidator = new QIntValidator;
        //    m_CaptchaTextInput->setValidator(m_CaptchaValidator);
        //    m_CaptchaTextInput->setMaxLength(4);
        //    m_CaptchaTextInput->setAlignment(Qt::AlignCenter);
        //    m_CaptchaTextInput->setPlaceholderText(QApplication::translate("LinkedOutClass", "Captcha", nullptr));
        //}

        ////To signup page label
        //{
        //    m_ToSignupLabel = new ClickableLabel(m_Frame);
        //    m_ToSignupLabel->setObjectName(QString::fromStdString("ToSignupLabel"));
        //    m_ToSignupLabel->setGeometry(QRect(0, 410, 380, 20));
        //    m_ToSignupLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        //    m_ToSignupLabel->setText(QApplication::translate("LinkedOutClass", "Don't have an account? Signup now!", nullptr));
        //    m_ToSignupLabel->setStyleSheet("border-top:none;border-bottom:none;border-radius:0px;");
        //}

        ////Login button
        //{
        //    m_LoginButton = new QPushButton(m_Frame);
        //    m_LoginButton->setObjectName(QString::fromUtf8("Login_Button"));
        //    m_LoginButton->setGeometry(QRect(115, 440, 151, 41));
        //    m_LoginButton->setAutoFillBackground(false);
        //    m_LoginButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
        //        "	border-style: solid; border-width: 2px; border-color: #000000;\n"
        //        "	border-radius: 20px;\n"
        //        "	background-color: rgb(86,152,144);\n"
        //        "}\n"
        //        "\n"
        //        "\n"
        //        "QPushButton:hover{\n"
        //        "	background-color: rgb(100,180,160);\n"
        //        "}\n"
        //        "QPushButton:disabled{\n"
        //        "background-color: rgb(120, 216, 192);"
        //        "}"));
        //    m_LoginButton->setFlat(true);
        //    m_LoginButton->setText(QApplication::translate("LinkedOutClass", "Login", nullptr));
        //    m_LoginButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        //}

        ////Code popup
        //{
        //    const int xCodePadding = 0;
        //    const int yCodePadding = 15;
        //    const int textWidth = 475;
        //    const int textHeight = 20;


        //    const int xInputPadding = 15;
        //    const int yInputPadding = 15;
        //    const int inputWidth = 100;
        //    const int inputHeight = 100;

        //    const int popupWidth = textWidth;

        //    const int popupHeight = yCodePadding + textHeight + yCodePadding + yInputPadding + inputHeight + yInputPadding;



        //    m_CodePopup = new PopupWindow(popupWidth,popupHeight,m_MainLayer->m_WindowCentralWidget);
        //    
        //    m_PopupCode = new QLabel(m_CodePopup);
        //    m_PopupCode->move(xCodePadding, yCodePadding);
        //    m_PopupCode->resize(textWidth, textHeight);
        //    m_PopupCode->setStyleSheet("font-style:italic;font-weight:bold;font-size:25px;");
        //    m_PopupCode->setAlignment(Qt::AlignCenter);
        // 
        //    for (int i = 0; i < 4; ++i) {
        //        m_UnitLengthInputs[i] = new UnitLengthInput(InputTypeBit_Digit, m_CodePopup);
        //        m_UnitLengthInputs[i]->resize(inputWidth, inputHeight);
        //        m_UnitLengthInputs[i]->move((i+1) * xInputPadding + i * inputWidth, popupHeight - yInputPadding - inputHeight);
        //        m_UnitLengthInputs[i]->setStyleSheet("QLineEdit { border: 1px solid #000000; }"
        //            "QLineEdit:focus { border: 1px solid #ff0000; }");
        //        m_UnitLengthInputs[i]->setAlignment(Qt::AlignCenter);
        //    }
        //}
        //m_MainLayer->m_LayersLayout->addWidget(m_MainFrame);

        //SetupLoginEvents();
	}


    void LoginLayer::OnInputChanged(const QString&) {
        m_LoginButton->setEnabled(UsernameHasInput() && PasswordHasInput() && CaptchaHasInput());
    }

    void LoginLayer::OnPopupInputChanged(const QString&)
    {
       /* if (!m_UnitLengthInputs[0]->text().isEmpty()
            && !m_UnitLengthInputs[1]->text().isEmpty()
            && !m_UnitLengthInputs[2]->text().isEmpty()
            && !m_UnitLengthInputs[3]->text().isEmpty()
            ) 
        {
            uint32_t num = 0;
            num += m_UnitLengthInputs[0]->text().toUInt() * 1000;
            num += m_UnitLengthInputs[1]->text().toUInt() * 100;
            num += m_UnitLengthInputs[2]->text().toUInt() * 10;
            num += m_UnitLengthInputs[3]->text().toUInt() * 1;

            std::string s = m_PopupCode->text().toStdString();

            if(num == m_PopupCode->text().toUInt()){
                m_CodePopup->hide();
                m_MainLayer->SwitchToUserInformation();
                m_IsPopupOpen = false;
            }
            else {
                m_MainLayer->m_MessageLayer->Error("Incorrect code");
                m_IsPopupOpen = false;
            }
            return;
        }
        m_IsPopupOpen = true;*/
    }

    bool LoginLayer::UsernameHasInput()
    {
        return !m_UsernameTextInput->text().isEmpty();
    }

    bool LoginLayer::PasswordHasInput()
    {
        return !m_PasswordTextInput->text().isEmpty();
    }

    bool LoginLayer::CaptchaHasInput()
    {
        return !m_CaptchaTextInput->text().isEmpty();
    }

	void LoginLayer::SetupLoginEvents() {
      /*  QObject::connect(m_ToSignupLabel, &ClickableLabel::clicked, [this]() {
            m_MainLayer->SwitchToSignup(true);
            });

        QObject::connect(m_CodePopup, &PopupWindow::opened, [this]() {
            uint32_t code = Random::GenerateUInt32(1000, 10000);
            m_PopupCode->setText(QString::number(code));
            m_UnitLengthInputs[0]->setText("");
            m_UnitLengthInputs[1]->setText("");
            m_UnitLengthInputs[2]->setText("");
            m_UnitLengthInputs[3]->setText("");
            });

        QObject::connect(m_CodePopup, &PopupWindow::closed, [this]() {
            m_MainLayer->SwitchToLogin(false);
            });

        QObject::connect(m_UnitLengthInputs[0], &UnitLengthInput::textChanged, LO_BIND_FN(OnPopupInputChanged));
        QObject::connect(m_UnitLengthInputs[1], &UnitLengthInput::textChanged, LO_BIND_FN(OnPopupInputChanged));
        QObject::connect(m_UnitLengthInputs[2], &UnitLengthInput::textChanged, LO_BIND_FN(OnPopupInputChanged));
        QObject::connect(m_UnitLengthInputs[3], &UnitLengthInput::textChanged, LO_BIND_FN(OnPopupInputChanged));

        QObject::connect(m_LoginButton, &QPushButton::clicked, [this]() {
            if (m_MainLayer->m_LastCaptchaNumber != m_CaptchaTextInput->text().toUInt()) {
                m_MainLayer->m_MessageLayer->Error(INCORRECT_CAPTCHA);
                CleanupInputs();
                return;
            }
            auto userData = GetUserDataFromLoginForm();
            if (userData.IsValid()) {
                int  ec = m_MainLayer->Login(userData, m_RememberMeCheckbox->checkState() == Qt::CheckState::Checked);
                if (ec == LoginErrorCodes_None)
                {
                    m_CodePopup->show();
                    m_IsPopupOpen = true;
                    return;
                }
                if ((ec & LoginErrorCodes_IncorrectPassword) || (ec & LoginErrorCodes_IncorrectUsername)) {
                    m_MainLayer->m_MessageLayer->Error(INCORRECT_USERNAME_OR_PASSWORD);
                }
            }
            else {
                m_MainLayer->m_MessageLayer->Error(INVALID_USERNAME_OR_PASSWORD);
            }
            m_MainLayer->SwitchToLogin(false);
            });


        QObject::connect(m_UsernameTextInput, &QLineEdit::textChanged, LO_BIND_FN(OnInputChanged));
        QObject::connect(m_PasswordTextInput, &QLineEdit::textChanged, LO_BIND_FN(OnInputChanged));
        QObject::connect(m_CaptchaTextInput, &QLineEdit::textChanged, LO_BIND_FN(OnInputChanged));*/

	}

    void LoginLayer::CleanAllInputs() {
       /* CleanupInputs();
        m_UsernameTextInput->setText("");
        m_RememberMeCheckbox->setChecked(false);*/
    }

    void LoginLayer::CleanupInputs() {
        m_CaptchaTextInput->setText("");
        m_PasswordTextInput->setText("");
        m_LoginButton->setEnabled(false);
    }

    UserInternalData LoginLayer::GetUserDataFromLoginForm() {
       
        std::string username = m_UsernameTextInput->text().toStdString();
        std::string password = m_PasswordTextInput->text().toStdString();

        UserInternalData ret;
        ret.Username = username;
        ret.Password = password;
        return ret;
    }


}