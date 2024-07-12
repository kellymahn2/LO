#pragma once
#include "Core/Layer.h"
#include "Core/Account.h"
class QVBoxLayout;
class ClickableLabel;
class QString;
class PopupWindow;
class UnitLengthInput;
namespace LinkedOut {
	class MainLayer;
	class SignupLayer : public Layer{
	public:
		SignupLayer(MainLayer* mainLayer);
		~SignupLayer();

		void Show() override;
		void Hide() override;
		void OnUpdate() override;
		virtual void* GetMainFrame()const override {
			return m_MainFrame;
		}
	private:
		void SetupSignup();
		void SetupSignupEvents();

		void Cleanup();
		void CleanupInputs();
		void CleanAllInputs();


		void OnInputChanged(const QString&);
		void OnPopupInputChanged(const QString&);

		bool UsernameHasInput();
		bool PasswordHasInput();
		bool CaptchaHasInput();

		UserInternalData SignupLayer::GetUserDataFromSignupForm();
	private:
		MainLayer* m_MainLayer = nullptr;
		QWidget* m_MainWidget = nullptr;

		SignupLayerUI* m_LayerUI = nullptr;

		QFrame* m_MainFrame = nullptr;
		QFrame* m_Frame = nullptr;
		QFrame* m_UsernameFrame = nullptr;
		QLineEdit* m_UsernameTextInput = nullptr;
		QLabel* m_UsernameLabel = nullptr;
		QPushButton* m_Button = nullptr;
		QFrame* m_PasswordFrame = nullptr;
		QLineEdit* m_PasswordTextInput = nullptr;
		QLabel* m_PasswordLabel = nullptr;
		QLabel* m_Label = nullptr;
		ClickableLabel* m_ToLoginLabel = nullptr;
		QFrame* m_CaptchaFrame = nullptr;
		QLineEdit* m_CaptchaTextInput = nullptr;
		QValidator* m_CaptchaValidator = nullptr;
		QLabel* m_CaptchaLabel = nullptr;
		QVBoxLayout* m_MainLayout;
		PopupWindow* m_CodePopup = nullptr;
		QLabel* m_PopupCode = nullptr;
		UnitLengthInput* m_UnitLengthInputs[4] = { nullptr };
		bool m_IsShowing = false;
		bool m_IsPopupOpen = true;
		friend class MainLayer;
	};
}