#pragma once
#include "Base.h"

namespace LinkedOut {
	class Layer {
	public:

		Layer(const std::string& name)
			:m_Name(name)
		{}
		virtual ~Layer() = default;
		virtual void Show() = 0;
		virtual void Hide() = 0;

		virtual void* GetMainFrame()const = 0;

		virtual void OnUpdate() = 0;
		const std::string& GetLayerName()const { return m_Name; }
	protected:
		std::string m_Name;
	};


}