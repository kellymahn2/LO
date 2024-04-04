#include "Core/Application.h"

#include "Core/Window.h"


extern LinkedOut::Application* LinkedOut::CreateApplication(const LinkedOut::ApplicationSpecification& spec);

int main(int argc, char *argv[])
{
	LinkedOut::Application* application = LinkedOut::CreateApplication({ "Linked Out App",{argc,argv} });

	application->Run();

	delete application;
}
