#include "Window.h"
#include <qmainwindow.h>
#include <QWindow>
namespace LinkedOut {

	Window* Window::s_MainWindow = nullptr;

	Window::Window(const WindowProps& props)
		:m_Props(props)
	{
		m_NativeWindow = new QMainWindow();
		m_NativeWindow->setWindowTitle(QString::fromStdString(m_Props.Title));
		m_NativeWindow->resize(m_Props.Width, m_Props.Height);
		m_NativeWindow->show();
	}
	Window::~Window(){

	}
	void Window::DestroyMain() {
		delete s_MainWindow;
	}

	Window* Window::CreateMain(const WindowProps& props) {
		DestroyMain();
		s_MainWindow = new Window(props);
		return s_MainWindow;
	}
}

