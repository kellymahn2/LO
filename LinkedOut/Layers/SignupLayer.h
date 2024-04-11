#pragma once
#include "Core/Layer.h"
class QVBoxLayout;
class ClickableLabel;
class QString;
namespace LinkedOut {
	class MainLayer;
	class SignupLayer : public Layer{
	public:
		SignupLayer(MainLayer* mainLayer);
		~SignupLayer();

		void Show() override;
		void Hide() override;
		void OnUpdate() override;



	private:
		void SetupSignup();
		void SetupSignupEvents();

		void Cleanup();
		void CleanupInputs();
		void CleanAllInputs();


		void OnInputChanged(const QString&);

		bool UsernameHasInput();
		bool PasswordHasInput();
		bool CaptchaHasInput();

		UserData SignupLayer::GetUserDataFromSignupForm();
	private:
		MainLayer* m_MainLayer = nullptr;
		QWidget* m_CentralWidget = nullptr;
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

		bool m_IsShowing = false;

		friend class MainLayer;
	};
}