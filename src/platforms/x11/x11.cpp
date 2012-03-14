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

int X11_Application::main(int argc, char **argv, Application *theApp)
{
	int retcode = 0;

	return retcode;
}


void	X11_Application::showWindow(bool s)
{

}
