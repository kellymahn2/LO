#pragma once
#include "Core/Layer.h"
#include <chrono>
#include<map>
class QVBoxLayout;
class QIcon;

namespace LinkedOut {
	class MainLayer;
	


	class MessageLayer : public Layer{
	public:


		MessageLayer(MainLayer* mainLayer);

		~MessageLayer();

		template<typename...Args>
		void Info(const std::string& format, Args&&...args) {
			ShowMessage(fmt::format(format, std::forward<Args>(args)...),MessageSeverity::Info);
		}

		template<typename...Args>
		void Warn(const std::string& format, Args&&...args) {
			ShowMessage(fmt::format(format, std::forward<Args>(args)...), MessageSeverity::Warn);
		}
		template<typename...Args>
		void Error(const std::string& format, Args&&...args) {
			ShowMessage(fmt::format(format, std::forward<Args>(args)...), MessageSeverity::Error);
		}
		

		void Show() override;
		void Hide() override;
		void OnUpdate() override;
	private:

		enum class MessageSeverity {
			Log,
			Info,
			Warn,
			Error,
		};

		struct Message {
			MessageSeverity Severity;
			float MessageDurationInMS;
			float MessageAliveDurationInMS;
			std::string MessageString;
			QFrame* MessageFrame;
			QLabel* MessageLabel;
			QPushButton* MessagePushButton;
			bool IsVisible = false;
		};

		void ShowMessage(const std::string& message, MessageSeverity severity);

		void DestroyMessage(Message& message);

		void RemoveExpiredMessages();
		void UpdateTimers();
	private:
		
		QFrame* m_MessagesFrame;
		QVBoxLayout* m_MessagesVerticalLayout;
		QIcon* m_MessageCloseIcon;
		QWidget* m_VerticalLayoutWidget;

		MainLayer* m_MainLayer;

		std::map<std::chrono::steady_clock::time_point,Message> m_Messages;

		bool m_IsShowing = true;

		std::chrono::steady_clock::time_point m_Last;
		std::chrono::steady_clock::time_point m_Now;

		friend class MainLayer;
	};

}

