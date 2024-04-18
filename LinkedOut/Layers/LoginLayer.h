#pragma once
#include "Core/Layer.h"

class ClickableLabel;
class QString;
class PopupWindow;

class UnitLengthInput;
namespace LinkedOut{

    class MainLayer;
    
    class LoginLayer : public Layer{
	public:
        LoginLayer(MainLayer* mainLayer);
        ~LoginLayer();
        void Show()override;
        void Hide()override;
        void OnUpdate();
    private:

        void SetupLogin();
        void SetupLoginEvents();

        void Cleanup();

        UserData GetUserDataFromLoginForm();

        void CleanupInputs();

        void CleanAllInputs();

        void OnInputChanged(const QString&);
        void OnPopupInputChanged(const QString&);

        bool UsernameHasInput();
        bool PasswordHasInput();
        bool CaptchaHasInput();

	private:
        MainLayer* m_MainLayer;
        QWidget* m_CentralWidget = nullptr;
        QFrame* m_MainFrame = nullptr;
        QFrame* m_Frame = nullptr;
        QFrame* m_UsernameFrame = nullptr;
        QLineEdit* m_UsernameTextInput = nullptr;
        QLabel* m_UsernameLabel = nullptr;
        QPushButton* m_LoginButton = nullptr;
        QFrame* m_PasswordFrame = nullptr;
        QLineEdit* m_PasswordTextInput = nullptr;
        QLabel* m_PasswordLabel = nullptr;
        QCheckBox* m_RememberMeCheckbox = nullptr;
        QLabel* m_Label = nullptr;
        QFrame* m_CaptchaFrame = nullptr;
        QLineEdit* m_CaptchaTextInput = nullptr;
        QValidator* m_CaptchaValidator = nullptr;
        ClickableLabel* m_ToSignupLabel = nullptr;
        QLabel* m_CaptchaLabel = nullptr;
        PopupWindow* m_CodePopup = nullptr;
        QLabel* m_PopupCode = nullptr;
        UnitLengthInput* m_UnitLengthInputs[4] = { nullptr };


        bool m_IsShowing = false;
        bool m_IsPopupOpen = true;
        friend class MainLayer;
	};
}