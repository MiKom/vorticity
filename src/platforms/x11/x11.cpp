#include "config.cpp"
#include <vorticity/platforms/x11/x11.h>

using namespace Vorticity;

X11_Application::X11_Application() : Application()
{
	getCore()->setApplication(this);
}

X11_Application::~X11_Application()
{
}

bool X11_Application::initialize()
{
	//TODO: implement
	return true;
}

void X11_Application::shutdown()
{
	//TODO: implement
}



int X11_Application::main(int argc, char **argv, Application *theApp)
{
	int retcode = 0;

	return retcode;
}


void X11_Application::showWindow(bool s)
{
	//TODO: implement
}

int X11_Application::run()
{
	//TODO: implement
	return 0;
}

time_t X11_Application::getTime() const
{
	//TODO: implement
	return 0;
}

void X11_Application::showMessage(const std::string &msg)
{
	//TODO: implement
}

void X11_Application::showMessage(const std::string& title, const std::string& msg)
{
	//TODO: implement
}

void X11_Application::die(const std::string &msg, int retcode)
{
	if(msg.length() > 0) {
		showMessage(msg);
	}
	exit(retcode);
}

void X11_Application::die(const std::string& title, const std::string& msg, int retcode)
{
	showMessage(title, msg);
	exit(retcode);
}

bool X11_Application::keyPressed(const char k)
{
	//TODO: implement
	return false;
}

bool X11_Application::mouseButtonPressed(const int btn)
{
	//TODO: implement
	return false;
}

void X11_Application::getMousePosition(int &x, int &y)
{
	//TODO: implement
	x = 0;
	y = 0;
}
