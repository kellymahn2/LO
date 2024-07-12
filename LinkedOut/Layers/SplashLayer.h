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
		virtual void* GetMainFrame()const override {
			return nullptr;
		}
	private:
		MainLayer* m_MainLayer;

		QWidget* m_MainWidget = nullptr;


		bool m_IsShowing = false;

		friend class MainLayer;

	};


}