#pragma once
#include "Core/Layer.h"

class QVBoxLayout;

namespace LinkedOut {


	class MainLayer;

	class SplashLayer : public Layer{
	public:
		SplashLayer(MainLayer* mainLayer);
		~SplashLayer();


		void OnUpdate() override;

		void Show() override;
		void Hide() override;
	private:
		void SetupSplash();

		void SetupSplashEvents();

		void Cleanup();

	private:
		MainLayer* m_MainLayer;


		QFrame* m_MainFrame;
		QFrame* m_Frame;
		QLabel* m_SplashWelcomeText;
		QLabel* m_SplashWelcomeImage;
		QPushButton* m_SplashSignUpButton;
		QPushButton* m_SplashLoginButton;


		bool m_IsShowing = false;

		friend class MainLayer;

	};


}