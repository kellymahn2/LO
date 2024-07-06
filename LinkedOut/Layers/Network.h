#pragma once
#include "Core/Layer.h"

#include "Core/Account.h"


namespace LinkedOut {
	class Network : public Layer{
	public:
		Network(const std::string& layerName)
			:Layer(layerName) 
		{}
		
	};
}