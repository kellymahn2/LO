#include "Application.h"
#include "Layers/MainLayer.h"
#include "Window.h"
#include "Random.h"
#include <qmainwindow.h>
#include <mutex>
#define LO_BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
namespace LinkedOut {

	static Application* s_Application;
	Application::Application(const ApplicationSpecification& spec)
		:m_Specification(spec), QApplication((int&)spec.CommandLineArgs.Count,(char**)spec.CommandLineArgs.Args)
	{
		s_Application = this;
		Random::m_RandomEngine = std::mt19937(Random::m_RandomDevice());
		m_UpdateTimer = new QTimer((QApplication*)this);
		Window::CreateMain(WindowProps(spec.Name));
		m_Database = CreateRef<DatabaseManager>("../Database/LinkedOutUserData.db");
	}
	static std::mutex mutex;
	void Application::Run() {


		m_UpdateTimer->start(16);
		QObject::connect(m_UpdateTimer, &QTimer::timeout, [this]() {this->OnUpdate(); });


		exec();

		std::unique_lock<std::mutex> lock(mutex);
		m_ApplicationStopped = true;
		m_UpdateTimer->stop();
	}
	void Application::OnUpdate() {
		std::unique_lock<std::mutex> lock(mutex);
		if (m_ApplicationStopped)
			return;
		if (m_MainLayer)
			m_MainLayer->OnUpdate();
		
		Window::GetMainWindow()->GetNativeHandle()->repaint();
		Window::GetMainWindow()->GetNativeHandle()->update();

	}


	Application* CreateApplication(const ApplicationSpecification& spec) {
		Application* app = new LinkedOut::Application(spec);
		app->SetMainLayer(CreateRef<MainLayer>());
		return app;
	}

	Application* Application::Get() {
		return s_Application;
	}
}
