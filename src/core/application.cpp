#include "config.h"

using namespace Vorticity;

const std::string Console::defaultTitle = "Vorticity :: Debug Console";

Application::Application() : device(NULL), console(NULL)
{
	width			= 640;
	height			= 480;
	updateFrequency = 50;
	windowTitle		= "Vorticity";
	threaded		= true;
	fullscreen		= false;
}

Application::~Application()
{
	delete device;
	delete console;
	delete fontManager;
}
