#pragma once

#include <string>


class QWindow;
class QMainWindow;

namespace LinkedOut {
	struct WindowProps {
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		WindowProps(const std::string& title = "", uint32_t width = 1280, uint32_t height = 720)
			:Title(title),Width(width),Height(height)
		{}
	};


	class Window {
	public:

		static Window* CreateMain(const WindowProps& props);

		static void DestroyMain();

		static Window* GetMainWindow() { return s_MainWindow; }
		QMainWindow* GetNativeHandle() { return m_NativeWindow; }

		const WindowProps& GetWindowProps()const { return m_Props; }


	private:
		Window(const WindowProps& props);
		~Window();
		WindowProps m_Props;
		QMainWindow* m_NativeWindow;
		static Window* s_MainWindow;
	};

}

