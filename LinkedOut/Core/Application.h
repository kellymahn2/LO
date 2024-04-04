#pragma once
#include "Core/Base.h"
#include "Core/Layer.h"
#include "DatabaseManager.h"
#include <QApplication>
#include <QTimer>
#include <vector>

#include "Layers/MainLayer.h"
namespace LinkedOut {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			return Args[index];
		}
	};
	struct ApplicationSpecification {
		std::string Name = "Kaidel Application";
		ApplicationCommandLineArgs CommandLineArgs;
	};


	class Application : QApplication {
	public:
		Application(const ApplicationSpecification& spec);
		~Application() {
			if (m_MainLayer)
				m_MainLayer->OnDetach();
		}

		void Run();



		void SetMainLayer(Ref<MainLayer> layer) {
			if (m_MainLayer)
				m_MainLayer->OnDetach();
			m_MainLayer = layer;
			layer->OnAttach();
		}

		static Application* Get();

		Ref<DatabaseManager> GetDatabase() const{ return m_Database; }
	private:

		void OnUpdate();

	private:


		bool m_ApplicationStopped = false;
		ApplicationSpecification m_Specification;
		Ref<MainLayer> m_MainLayer;

		QTimer* m_UpdateTimer;

		Ref<DatabaseManager> m_Database;


	};

	Application* CreateApplication(const ApplicationSpecification& spec);


}

